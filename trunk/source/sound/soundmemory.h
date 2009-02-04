#ifndef maid2_sound_soundmemory_h
#define maid2_sound_soundmemory_h

#include"../config/define.h"
#include"../auxiliary/globalpointer.h"


#include"soundcore.h"
#include"pcmdecoder.h"
#include"common.h"


namespace Maid
{
  class SoundMemory
    : public GlobalPointer<SoundCore>
	{
	public:
		SoundMemory();
		virtual ~SoundMemory();

    void Create( const PCMFORMAT Format );
    void Destroy();

    void Play();
    void Stop();

    struct BUFFERINFO
    {
      void* pData;
    };

    void Map( BUFFERINFO& out, size_t Size );
    void Unmap();

    void SetVolume( float Volume );

    bool IsPlay() const;
    float GetVolume() const;

    bool IsEmpty() const;
  private:
    SPSOUNDOBJECTINFO m_pInfo;

    bool  m_IsPlay;
    float m_Volume;
  };
}