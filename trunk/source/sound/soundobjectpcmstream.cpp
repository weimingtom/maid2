#include"soundobjectpcmstream.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/mathematics.h"


namespace Maid {

  /*!
      @class  SoundObjectPCMStream soundobjectpcmstream.h
      @brief  ストリーム書き込みで再生するクラス
  */

void SoundObjectPCMStream::Initialize( const Sound::SPBUFFER& pBuffer, const SPPCMREADER& pDecoder, const SPBUFFER& pData )
{
  m_pBuffer = pBuffer;
  m_pDecoder= pDecoder;
  m_pData   = pData;
  m_IsLoopPlay = false;
  m_NowPlayPosition = 0;
  m_PrevBufferPosition = 0;
  m_WrittedBufferPosition = 0;

  UpdateBuffer();
}

void SoundObjectPCMStream::Update()
{
  const size_t bufferpos = m_pBuffer->GetPlayPosition();
  const size_t decoderpos= m_pDecoder->GetPosition();
  const size_t decoderlen= m_pDecoder->GetLength();
  m_NowPlayPosition += CalcLength( m_PrevBufferPosition, bufferpos );
  m_PrevBufferPosition = bufferpos;


  //  再生した量がデコーダーより長かったら、全部再生されたことになる
  if( decoderlen <= m_NowPlayPosition )
  {
    if( m_IsLoopPlay )
    {
      m_NowPlayPosition -= decoderlen;
    }else
    {
      if( m_pBuffer->IsPlay() ) { m_pBuffer->Stop(); }
    }
  }
  else
  { //  そうでないならバッファの更新なんだけど、ある程度まとめてやる
    const size_t sa = decoderpos - m_NowPlayPosition;
    if( sa < m_pBuffer->GetParam().Length/3 )
    {
      UpdateBuffer(); 
    }
  }
}

void SoundObjectPCMStream::Play()
{
  if( m_pDecoder->GetLength() <= m_NowPlayPosition ) { return; }
  m_pBuffer->Play(true);
}

void SoundObjectPCMStream::Stop()
{
  m_pBuffer->Stop();
}

void SoundObjectPCMStream::SetPosition( float time )
{
  const Sound::CREATEBUFFERPARAM& param = m_pBuffer->GetParam();
  const size_t pos = param.Format.CalcLength(time);
  const size_t writeok = m_pBuffer->GetWritePosition();

  m_pDecoder->SetPosition( pos );
  m_WrittedBufferPosition = m_pBuffer->GetWritePosition();
  UpdateBuffer(); 
  m_pBuffer->SetPosition( writeok );
  m_PrevBufferPosition = writeok;
  m_NowPlayPosition = pos;
}

void SoundObjectPCMStream::SetVolume( float volume )
{
  //  強さから デシベルに変換する
  // http://e-words.jp/w/E38387E382B7E38399E383AB.html

  const float db = 20.0f * Math<float>::log10(volume);

  m_pBuffer->SetVolume(db);
}

void SoundObjectPCMStream::SetJumpPoint( const std::vector<JUMPPOINT>& list )
{
  m_JumpList = list;
}

void SoundObjectPCMStream::SetLoopState( bool IsLoop )
{
  m_IsLoopPlay = IsLoop;
}

bool SoundObjectPCMStream::IsPlay()const
{
  return m_pBuffer->IsPlay();
}

float SoundObjectPCMStream::GetPosition() const
{
  const size_t pos = m_NowPlayPosition + CalcLength( m_PrevBufferPosition, m_pBuffer->GetPlayPosition() );

  if( m_pDecoder->GetLength() <= m_NowPlayPosition ) { return 0; }

  return m_pBuffer->GetParam().Format.CalcTime(pos);
}

float SoundObjectPCMStream::GetVolume() const
{
  //  デシベルから 強さに変換する
  // http://e-words.jp/w/E38387E382B7E38399E383AB.html

  const float db = m_pBuffer->GetVolume();

  if( db < -100.0f ) { return 0; }

  //  db = 20.0f * Math<float>::log10(ret);
  //  db/20.0f = Math<float>::log10(ret);
  // 10 の db/20.0f 乗が ret 

  return Math<float>::pow(10,db/20.0f);
}

void SoundObjectPCMStream::UpdateBuffer()
{
  const size_t writepos  = m_WrittedBufferPosition;
  const size_t DecodeLen = m_pBuffer->GetParam().Length / 2;

  //  リングバッファにデータを流すのはしんどいので
  //  いったん作ってからやる
  std::vector<unt08> tmp(DecodeLen);

  {
    const size_t len = m_pDecoder->Read( &(tmp[0]), DecodeLen );
    if( len<DecodeLen )
    { //  デコードが末端までいったら
      //  ループ再生か否かで、バッファの接続を決める
      if( m_IsLoopPlay )  
      {
        m_pDecoder->SetPosition(0);
        m_pDecoder->Read( &(tmp[len]), DecodeLen-len );
      }
      else
      {
        ZERO( &(tmp[len]), DecodeLen-len );
      }
    }
  }

  //  そんでもってながしこむ～
  {
    Sound::IBuffer::LOCKDATA dat;
    m_pBuffer->Lock( writepos, DecodeLen, dat );
    memcpy( dat.pData1, &(tmp[0]), dat.Data1Length );
    if( dat.pData2!=NULL ) { memcpy( dat.pData2, &(tmp[dat.Data1Length]), dat.Data2Length ); }
    m_pBuffer->Unlock( dat );
  }

  m_WrittedBufferPosition += DecodeLen;
  m_WrittedBufferPosition %= m_pBuffer->GetParam().Length;
}

size_t SoundObjectPCMStream::CalcLength( size_t prev, size_t now )const
{
  size_t ret;
  
  if( prev<=now ) { ret = now-prev; }
  else { ret = m_pBuffer->GetParam().Length - prev; }

  return ret;
}

}


