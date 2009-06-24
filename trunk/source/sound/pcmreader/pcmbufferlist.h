#ifndef maid2_sound_pcmreader_pcmbufferlist_h
#define maid2_sound_pcmreader_pcmbufferlist_h 

#include"../../config/define.h"
#include"../../auxiliary/memorybuffer.h"
#include"../../auxiliary/thread.h"
#include"ipcmreader.h"

#include<vector>
#include<list>

namespace Maid
{
  class PCMBufferList
    : public IPCMReader
  {
  public:
    PCMBufferList( const PCMFORMAT& fmt );

    INITIALIZERESULT Initialize();
    void Finalize();

    size_t  Read( void* pDst, size_t size );
    void    SetPosition( size_t Offset );
    size_t  GetPosition()		const;
    size_t  GetLength()			const;

    PCMFORMAT GetFormat() const;

    void Create( const void* pData, size_t Size );

    void Clear();

  private:
    ThreadMutex m_Mutex;

    typedef std::list<SPMEMORYBUFFER> BUFFERLIST;
    BUFFERLIST m_BufferList;

    const PCMFORMAT m_Format;
    size_t  m_Position;
    size_t  m_BufferPosition;
  };

  typedef	boost::shared_ptr<PCMBufferList>	SPPCMBUFFERLIST;

}

#endif