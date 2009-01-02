#ifndef maid2_sound_soundobjectnull_h
#define maid2_sound_soundobjectnull_h

#include"../config/define.h"
#include"isoundobject.h"

namespace Maid {

  class SoundObjectNULL : public ISoundObject
  {
  public:
    SoundObjectNULL():ISoundObject(TYPE_NULL){}
    void Update(){}
    void Play(){}
    void Stop(){}
    void SetPosition( float time ){}
    void SetVolume( float volume ){}
    void SetJumpPoint( const std::vector<JUMPPOINT>& list ){}
    void SetLoopState( bool IsLoop ){}
  };

}


#endif
