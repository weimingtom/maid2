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

		void Initialize( const Sound::SPBUFFER& pBuffer, const SPPCMDECODER& pDecoder, const boost::shared_ptr<std::vector<unt08> >& pData );

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
    const boost::shared_ptr<std::vector<unt08> >& GetData() const { return m_pData; }
    const SPPCMDECODER& GetDecoder() const { return m_pDecoder; }

  private:
    void UpdateBuffer();
    size_t CalcLength( size_t prev, size_t now )const;

  private:
    Sound::SPBUFFER m_pBuffer;
		SPPCMDECODER    m_pDecoder;
    boost::shared_ptr<std::vector<unt08>> m_pData;
    std::vector<JUMPPOINT>  m_JumpList;
    bool  m_IsLoopPlay;

    size_t  m_NowPlayPosition;    //  再生位置
    size_t  m_PrevBufferPosition; //  前回の Update() 時にあった 再生位置
    size_t  m_WrittedBufferPosition;
  };
}


#endif

