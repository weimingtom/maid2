#include"custompcmreader.h"

namespace Maid
{
void CustomPCMReader::Initialize( const SPPCMREADER& pReader )
{
  Initialize( pReader, JUMPPOINTLIST() );
}

//! デコーダの初期化
/*!
    @param  pReader [i ]  デコーダ（Initialize() は呼んであること)
    @param  list    [i ]  デコード順番が書いてあるリスト

    @return 初期化に成功したら INITIALIZERESULT_OK
            失敗なら INITIALIZERESULT_ERROR
 */
void CustomPCMReader::Initialize( const SPPCMREADER& pReader, const JUMPPOINTLIST& list )
{
  m_pReader = pReader;
  m_JumpPointList = list;
  m_Position = 0;

  if( m_JumpPointList.empty() ) 
  {
    m_Length = m_pReader->GetLength(); 
  }else
  {
    //  ジャンプリストを見ていって
    //  ・奥に進むなら計算する
    //  ・ひとつでも手前に飛ぶなら最大サイズ（終わりがない）
    m_Length = m_pReader->GetLength();
    for( int i=0; i<(int)list.size(); ++i )
    {
      const JUMPPOINT& p = list[i];

      if( p.Boot < p.Jump ) { m_Length -= p.Jump-p.Boot; }
      else { m_Length = ~0; break; }
    }
  }
}

void  CustomPCMReader::Finalize()
{
  m_pReader.reset();
  m_JumpPointList = JUMPPOINTLIST();
}


size_t  CustomPCMReader::Read( void* pDst, size_t size )
{
  //  現在の位置から +size までに m_JumpPointList にぶつかるところがないか調べる
  size_t pos  = m_pReader->GetPosition();
  size_t lim  = size;

  unt08* pNowDst = (unt08*)pDst;

  while( true )
  {
    if( lim==0 ) { break; }
    if( m_pReader->GetLength() <= m_pReader->GetPosition() ) { break; }

    bool IsHit = false;
    for( int i=0; i<(int)m_JumpPointList.size(); ++i )
    {
      const JUMPPOINT& p = m_JumpPointList[i];
      //  間に入ってなかったら関係ない
      if( !(pos <= p.Boot && p.Boot < pos+lim) ) { continue; }

      const size_t readsize = m_pReader->Read( pNowDst, p.Boot - pos );
      m_pReader->SetPosition( p.Jump );
      pNowDst += readsize;

      lim -= readsize;
      IsHit = true;
      break;
    }

    if( !IsHit ) 
    {
      const size_t readsize = m_pReader->Read( pNowDst, lim );
      pNowDst += readsize;
      lim -= readsize;
    }
  }

  m_Position += size-lim;

  return size-lim;
}

void  CustomPCMReader::SetPosition( size_t pos )
{
  size_t len = pos;
  size_t nowpos = 0;

  while( true )
  {
    if( len==0 ) { break; }

    bool IsHit = false;
    for( int i=0; i<(int)m_JumpPointList.size(); ++i )
    {
      const JUMPPOINT& p = m_JumpPointList[i];
      //  間に入ってなかったら関係ない
      if( !(nowpos <= p.Boot && p.Boot < nowpos+len) ) { continue; }

      len    -= p.Boot - nowpos;
      nowpos  = p.Boot;
      break;
    }

    if( !IsHit ) 
    {
      nowpos += len;
      len = 0;
    }
  }

  m_pReader->SetPosition( nowpos );
  m_Position = pos;
}

size_t  CustomPCMReader::GetPosition()  const
{
  return m_Position;
}

size_t  CustomPCMReader::GetLength()  const
{
  return m_Length;
}

PCMFORMAT CustomPCMReader::GetFormat()  const
{
  return m_pReader->GetFormat();
}

const CustomPCMReader::JUMPPOINTLIST& CustomPCMReader::GetJumpList()const
{
  return m_JumpPointList;
}


}

