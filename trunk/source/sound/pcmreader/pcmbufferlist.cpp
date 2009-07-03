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
  ThreadMutexLocker lock(m_Mutex);

  size_t ReadSize = 0;
  size_t NowPos  = GetPosition();
  size_t BufferPos = m_BufferPosition;
  unt08* pTarget = (unt08*)pDst;


  while( true )
  {
    //  データがない場合はその場で終了
    if( ReadSize==size ) { break; }
    if( m_BufferList.empty() ) { break; }

    const BUFFERINFO& info = m_BufferList.front();
    const size_t InfoTime = info.Time + BufferPos;
    const size_t LimSize  = size - ReadSize;

    if( NowPos < InfoTime )
    {
      //  データが未来にしかないなら、無音をはさむ
      const size_t muon = std::min(LimSize,InfoTime - NowPos);
//  MAID_WARNING( "Read() 無音 " << muon );
      ZERO( pTarget, muon );
      pTarget += muon;
      NowPos  += muon;
      ReadSize+= muon;
    }
    else if( InfoTime < NowPos )
    {
      const size_t sa = NowPos - InfoTime;
      const size_t MaxByte = info.pBuffer->GetSize()-BufferPos;
//  MAID_WARNING( "Read() スキップ sa " << sa << " MaxByte " << MaxByte );
      if( MaxByte <= sa )
      {
        m_BufferList.pop_front();
        BufferPos = 0;
      }else
      {
        BufferPos += sa;
      }
    }
    else
    {
      //  info.pBuffer[BufferPos] からコピー開始
      const size_t MaxByte = info.pBuffer->GetSize()-BufferPos;
      const size_t p = std::min( LimSize, MaxByte );

//  MAID_WARNING( "Read() コピー " << p );
      ::memcpy( pTarget, info.pBuffer->GetPointer(BufferPos), p );
      pTarget += p;
      NowPos  += p;
      ReadSize+= p;
      BufferPos += p;

      if( BufferPos==info.pBuffer->GetSize() )
      {
        m_BufferList.pop_front();
        BufferPos = 0;
      }
    }
  }

//  MAID_WARNING( "Read() end pos " << NowPos );

  m_Position = NowPos;
  m_BufferPosition = BufferPos;

  return ReadSize;
}

void    PCMBufferList::SetPosition( size_t Offset )
{
  ThreadMutexLocker lock(m_Mutex);

//  MAID_WARNING( "SetPosition() " << Offset << " now " << m_Position );

  if( m_Position==Offset ) { return ; }

  //  過去に進む場合は全消去
  if( Offset < m_Position )
  {
    m_BufferList.clear();
  }
  else
  {
    //  新しい時間のデータがあるなら、そこまで削る
    for( BUFFERLIST::iterator ite=m_BufferList.begin(); ite!=m_BufferList.end(); )
    {
      BUFFERINFO& info = *ite;
      const size_t begin = info.Time;
      const size_t end   = begin + info.pBuffer->GetSize();

      if( begin<=Offset && Offset<end )
      {
        const size_t pos = Offset - begin;
        void* pDst = info.pBuffer->GetPointer(0);
        void* pSrc = info.pBuffer->GetPointer(pos);
        const size_t size = info.pBuffer->GetSize() - pos;

        ::memmove( pDst, pSrc, size );
        info.Time = Offset;
        info.pBuffer->Resize(size);
        break;
      }else
      {
        m_BufferList.pop_front();
      }
    }
  }

  m_Position = Offset;
  m_BufferPosition = 0;
}

size_t  PCMBufferList::GetPosition()		const
{
  return m_Position;
}

size_t  PCMBufferList::GetLength()			const
{
  ThreadMutexLocker lock(const_cast<PCMBufferList*>(this)->m_Mutex);

  if( m_BufferList.empty() ) { return m_Position; }

  const BUFFERINFO& info = m_BufferList.back();

  return info.Time + info.pBuffer->GetSize();
}

PCMFORMAT PCMBufferList::GetFormat() const
{
  return m_Format;
}

void PCMBufferList::Create( size_t time, const void* pData, size_t Size )
{
  ThreadMutexLocker lock(m_Mutex);

  SPMEMORYBUFFER pBuf( new MemoryBuffer );

  pBuf->Resize(Size);
  ::memcpy( pBuf->GetPointer(0), pData, Size );

  m_BufferList.push_back( BUFFERINFO(time,pBuf) );
}


}

