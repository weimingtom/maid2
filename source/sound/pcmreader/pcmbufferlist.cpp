#include"pcmbufferlist.h"

#include"../../auxiliary/macro.h"

#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"



namespace Maid
{

PCMBufferList::PCMBufferList( const PCMFORMAT& fmt )
  :m_Format(fmt)
  ,m_Position(0)
  ,m_BufferPosition(0)
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
  m_BufferPosition = 0;
}

void PCMBufferList::Clear()
{
  ThreadMutexLocker lock(m_Mutex);

  m_BufferList.clear();
  m_Position = 0;
  m_BufferPosition = 0;
}


size_t  PCMBufferList::Read( void* pDst, size_t size )
{
//  MAID_WARNING("pcmread:" << size );

  ThreadMutexLocker lock(m_Mutex);

//  MAID_WARNING( GetCurrentThreadId() << " PCMBufferList::Read():" << size );

  size_t read_length = size;
  unt08* pTarget = (unt08*)pDst;

  BUFFERLIST::iterator ite=m_BufferList.begin();

  while( true )
  {
    if( read_length==0 ) { break; }
    if( ite==m_BufferList.end() ) { break; }

    const BUFFERINFO& info = (*ite);
    const SPMEMORYBUFFER& pSrc = info.pBuffer;
    const size_t lim = pSrc->GetSize() - m_BufferPosition;  //  このバッファでまだ読んでない量

    if( read_length < lim )
    {
      ::memcpy( pTarget, pSrc->GetPointer(m_BufferPosition), read_length );

      m_BufferPosition += read_length;
      read_length = 0;
      break;
    }
    else
    {
      ::memcpy( pTarget, pSrc->GetPointer(m_BufferPosition), lim );
      pTarget += lim;
      read_length -= lim;

      m_BufferPosition = 0;
      ite = m_BufferList.erase(ite);
    }
  }
/*
  if( read_length != 0 )
  {
    //  足りない分は無音でも突っ込んでおく
    ZERO( pTarget, read_length );
  }
  m_Position += size;
*/
  m_Position += size-read_length;

  return size;
}

void    PCMBufferList::SetPosition( size_t Offset )
{
  m_Position = Offset;
}

size_t  PCMBufferList::GetPosition()		const
{
  return m_Position;
}

size_t  PCMBufferList::GetLength()			const
{
  ThreadMutexLocker lock(const_cast<PCMBufferList*>(this)->m_Mutex);

  size_t ret = m_Position;

  for( BUFFERLIST::const_iterator ite=m_BufferList.begin();
            ite!=m_BufferList.end(); ++ite )
  {
    ret += (*ite).pBuffer->GetSize();
  }

  ret -= m_BufferPosition;

  return ret;
}

PCMFORMAT PCMBufferList::GetFormat() const
{
  return m_Format;
}

void PCMBufferList::Create( size_t time, const void* pData, size_t Size )
{
  const size_t len = GetLength();  //  すでに存在しているデータの時間


  SPMEMORYBUFFER pBuf( new MemoryBuffer );

  if( time==~0 || len==time )
  {
    pBuf->Resize(Size);
    ::memcpy( pBuf->GetPointer(0), pData, Size );
  }
  else if( len<time )
  {
    const size_t sa = time - len;
    pBuf->Resize(Size+sa);
    ZERO( pBuf->GetPointer(0), sa );
    ::memcpy( pBuf->GetPointer(sa), pData, Size );
  }
  else if(len>time )
  {
    const size_t sa = len - time;
    pBuf->Resize(Size-sa);
    ::memcpy( pBuf->GetPointer(0), ((unt08*)pData)+sa, Size-sa );
  }



  ThreadMutexLocker lock(m_Mutex);
  m_BufferList.push_back( BUFFERINFO(time,pBuf) );
}


}

