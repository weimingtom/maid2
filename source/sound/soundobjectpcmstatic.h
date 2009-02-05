#ifndef maid2_sound_soundobjectstatic_h
#define maid2_sound_soundobjectstatic_h

#include"../config/define.h"
#include"isoundobject.h"
#include"core/ibuffer.h"
#include"../auxiliary/buffer.h"

namespace Maid {

  class SoundObjectPCMStatic : public ISoundObject
  {
  public:
    SoundObjectPCMStatic():ISoundObject(TYPE_STATIC){}

		void Initialize( const Sound::SPBUFFER& pBuffer, const SPBUFFER& pData );

    void Update();
    void Play();
    void Stop();
    void SetPosition( float time );
    void SetVolume( float volume );
    void SetJumpPoint( const std::vector<JUMPPOINT>& list );
    void SetLoopState( bool IsLoop );
    bool IsPlay()const;
    float GetPosition() const;
    float GetVolume() const;

    const Sound::SPBUFFER& GetBuffer() const { return m_pBuffer; }
    const SPBUFFER& GetData() const { return m_pData; }

  private:
    Sound::SPBUFFER  m_pBuffer;
    bool m_IsLoop;
    SPBUFFER  m_pData;  //  生データ
  };



}


#endif
