#ifndef maid2_sound_decoder_pcmdecodermemory_h
#define maid2_sound_decoder_pcmdecodermemory_h 

#include"../../config/define.h"
#include"../../config/typedef.h"
#include"ipcmdecoder.h"

namespace Maid
{
  class PCMDecoderMemory : public IPCMDecoder
  {
  public:
    PCMDecoderMemory( const void* pSrc, size_t size );

    INITIALIZERESULT Initialize();
    void Finalize();

    size_t  Read( void* pDst, size_t size );
    void    SetPosition( size_t Offset );
    size_t  GetPosition()		const;
    size_t  GetLength()			const;

    PCMFORMAT GetFormat() const;

    void Lock( void*& pData, size_t Size );

  private:
    typedef std::vector<unt08>  BUFFER;
    std::list<BUFFER> m_Buffer;

    size_t  m_Position;

  };
}

#endif