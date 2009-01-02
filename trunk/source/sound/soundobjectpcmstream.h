#ifndef maid2_sound_soundobjectstream_h
#define maid2_sound_soundobjectstream_h

#include"../config/define.h"
#include"isoundobject.h"
#include"core/ibuffer.h"
#include"pcmdecoder/ipcmdecoder.h"

namespace Maid {

  class SoundObjectPCMStream : public ISoundObject
  {
  public:
    SoundObjectPCMStream():ISoundObject(TYPE_STREAM){}

		void Initialize( const Sound::SPBUFFER& pBuffer, const SPPCMDECODER& pDecoder );

    void Update();
    void Play();
    void Stop();
    void SetPosition( float time );
    void SetVolume( float volume );
    void SetJumpPoint( const std::vector<JUMPPOINT>& list );
    void SetLoopState( bool IsLoop );

  private:
    Sound::SPBUFFER m_pBuffer;
		SPPCMDECODER    m_pDecoder;
    std::vector<JUMPPOINT>  m_JumpList;
    bool  m_IsLoopPlay;
  };



}


#endif

