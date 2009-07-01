#ifndef maid2_sound_pcmwriter_h
#define maid2_sound_pcmwriter_h


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"


#include"soundcore.h"
#include"pcmreader/pcmbufferlist.h"
#include"common.h"


namespace Maid {

  class PCMWriter
    : public GlobalPointer<SoundCore>
	{
	public:
		PCMWriter();
		virtual ~PCMWriter();

    void Initialize( const PCMFORMAT& fmt, float BufferSize );
    void Finalize();

    void Play();
    void Stop();

    bool IsPlay() const;

    void SetVolume( float Volume );
    float GetVolume() const;

    void Write( const void* pData, size_t Size );
    void Write( double time, const void* pData, size_t Size );

    bool IsInitialized() const;

  private:
    SPSOUNDOBJECTINFO m_pInfo;

    boost::shared_ptr<PCMBufferList>  m_pBuffer;

    bool  m_IsPlay;
    float m_Volume;
    float m_Position;
  };
}


#endif