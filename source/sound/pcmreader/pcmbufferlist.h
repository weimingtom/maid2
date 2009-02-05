#ifndef maid2_sound_pcmreader_pcmbufferlist_h
#define maid2_sound_pcmreader_pcmbufferlist_h 

#include"../../config/define.h"
#include"../../config/typedef.h"
#include"ipcmreader.h"

#include<vector>
#include<list>

namespace Maid
{
  class PCMBufferList : public IPCMReader
  {
  public:
    PCMBufferList();

    INITIALIZERESULT Initialize();
    void Finalize();

    size_t  Read( void* pDst, size_t size );
    void    SetPosition( size_t Offset );
    size_t  GetPosition()		const;
    size_t  GetLength()			const;

    PCMFORMAT GetFormat() const;

    void* Create( size_t Size );

  private:
    typedef std::vector<unt08>  BUFFER;
    std::list<BUFFER> m_Buffer;

    size_t  m_Position;
  };
}

#endif