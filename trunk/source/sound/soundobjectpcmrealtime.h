#ifndef maid2_sound_soundobjectpcmrealtime_h
#define maid2_sound_soundobjectpcmrealtime_h

#include"../config/define.h"
#include"isoundobject.h"
#include"core/ibuffer.h"
#include"pcmreader/ipcmreader.h"
#include"pcmreader/pcmbufferlist.h"

namespace Maid {

  class SoundObjectPCMRealTime
    : public ISoundObject
  {
  public:
    SoundObjectPCMRealTime();

		void Initialize( const Sound::SPBUFFER& pBuffer, const SPPCMBUFFERLIST& pDecoder );

    void Update();
    void Play();
    void Stop();
    void SetPosition( double time );
    void SetVolume( double volume );
    void SetLoopState( bool IsLoop );
    bool IsPlay()const;
    double GetPosition() const;
    double GetVolume() const;

 
  private:
    void UpdateBuffer();
    size_t CalcLength( size_t prev, size_t now )const;
    size_t CalcUpdateScape()const;

  private:
    Sound::SPBUFFER m_pBuffer;
		SPPCMBUFFERLIST m_pDecoder;

    size_t m_OldPosition;   //  前回のバッファの位置（バイト単位）
    size_t m_PlayPosition;  //  Playしてからの合計再生位置（バイト単位）
    size_t m_WritePosition; //  すでに書き込んだ合計位置（バイト単位）

/*
    size_t  m_TotalPlayPosition;      //  Playしてからの合計再生位置（バイト単位）
    size_t  m_PrevBufferPosition;     //  前回のバッファの位置（バイト単位）
    size_t  m_WrittedBufferPosition;  //  次に書き込むバッファの位置（バイト単位）
*/
  };
}


#endif

