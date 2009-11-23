#include"pcmbufferlist.h"

#include"../../auxiliary/macro.h"

#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"



namespace Maid
{

PCMBufferList::PCMBufferList( const PCMFORMAT& fmt )
  :m_Format(fmt)
  ,m_Position(0)
{

}

PCMBufferList::INITIALIZERESULT PCMBufferList::Initialize()
{
  Finalize();

  return INITIALIZERESULT_OK;
}

void PCMBufferList::Finalize()
{
  ThreadMutexLocker lock(m_Mutex);

  m_BufferList.clear();
  m_Position = 0;
}

size_t  PCMBufferList::Read( void* pDst, size_t size )
{
  ThreadMutexLocker lock(m_Mutex);

  size_t ReadSize = size;
  size_t NowPos   = GetPosition();
  unt08* pTarget  = (unt08*)pDst;

  for( BUFFERLIST::const_iterator ite=m_BufferList.begin(); ite!=m_BufferList.end(); ++ite )
  {
    //  全部読み込み終わっていたら、さっさと終了しておく
    if( ReadSize==0 ) { break; }

    const BUFFERINFO& info = *(*ite);
    const size_t begin = info.Time;
    const size_t end   = info.Time+info.Buffer.GetSize();

    //  指定時間の範囲前だったら次へ
    if( end < NowPos ) { continue; }
    //  指定時間の範囲後だったら(時間順にソートされているので)終了
    if( NowPos+ReadSize < begin ) { break; }

    if( begin<=NowPos && NowPos<end )
    {
      const size_t pos = NowPos - begin;
      const size_t len = std::min(ReadSize,info.Buffer.GetSize()-pos );

      info.Buffer.Get( pTarget, pos, len );

      pTarget += len;
      NowPos  += len;
      ReadSize-= len;
    }
  }

  const size_t totalread = size - ReadSize;
  m_Position += totalread;

  return totalread;
}

void    PCMBufferList::SetPosition( size_t Offset )
{
  ThreadMutexLocker lock(m_Mutex);
  m_Position = Offset;
}

size_t  PCMBufferList::GetPosition()		const
{
  return m_Position;
}

size_t  PCMBufferList::GetLength()			const
{
  ThreadMutexLocker lock(const_cast<PCMBufferList*>(this)->m_Mutex);

  if( m_BufferList.empty() ) { return m_Position; }

  const BUFFERINFO& info = *(*(m_BufferList.rbegin()));

  return info.Time + info.Buffer.GetSize();
}

PCMFORMAT PCMBufferList::GetFormat() const
{
  return m_Format;
}

void PCMBufferList::Create( size_t time, const void* pData, size_t Size )
{
  SPBUFFERINFO pInfo( new BUFFERINFO );

  pInfo->Time = time;
  pInfo->Buffer.Resize(Size);
  ::memcpy( pInfo->Buffer.GetPointer(0), pData, Size );

  ThreadMutexLocker lock(m_Mutex);
  m_BufferList.insert( pInfo );
}

void PCMBufferList::ClearData( size_t time )
{
  ThreadMutexLocker lock(m_Mutex);
  //  time 以前のデータを全部消す

  while( true )
  {
    if( m_BufferList.empty() ) { break; }

    BUFFERLIST::const_iterator ite=m_BufferList.begin();
    const BUFFERINFO& info = *(*ite);

    if( info.Time + info.Buffer.GetSize() < time )
    {
      m_BufferList.erase( ite );
    }else
    {
      break;
    }
  }
}


}

