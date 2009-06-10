#ifndef maid2_sound_soundobjectstatic_h
#define maid2_sound_soundobjectstatic_h

#include"../config/define.h"
#include"isoundobject.h"
#include"core/ibuffer.h"
#include"../auxiliary/memorybuffer.h"

namespace Maid {

  class SoundObjectPCMStatic : public ISoundObject
  {
  public:
    SoundObjectPCMStatic():ISoundObject(TYPE_STATIC){}

		void Initialize( const Sound::SPBUFFER& pBuffer, const SPMEMORYBUFFER& pData );

    void Update();
    void Play();
    void Stop();
    void SetPosition( double time );
    void SetVolume( double volume );
    void SetLoopState( bool IsLoop );
    bool IsPlay()const;
    double GetPosition() const;
    double GetVolume() const;

    const Sound::SPBUFFER& GetBuffer() const { return m_pBuffer; }
    const SPMEMORYBUFFER& GetData() const { return m_pData; }

  private:
    Sound::SPBUFFER  m_pBuffer;
    bool m_IsLoop;
    SPMEMORYBUFFER  m_pData;  //  生データ
  };



}


#endif
