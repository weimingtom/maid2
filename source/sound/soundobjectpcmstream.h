#ifndef maid2_sound_soundobjectstream_h
#define maid2_sound_soundobjectstream_h

#include"../config/define.h"
#include"isoundobject.h"
#include"core/ibuffer.h"
#include"pcmreader/ipcmreader.h"
#include"pcmreader/custompcmreader.h"
#include"../auxiliary/memorybuffer.h"

namespace Maid {

  class SoundObjectPCMStream : public ISoundObject
  {
  public:
    SoundObjectPCMStream():ISoundObject(TYPE_STREAM){}

		void Initialize( const Sound::SPBUFFER& pBuffer, const SPCUSTOMPCMREADER& pDecoder, const SPMEMORYBUFFER& pData );

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
    const SPCUSTOMPCMREADER& GetCustomPCMReader() const { return m_pDecoder; }

  private:
    void UpdateBuffer();
    size_t CalcLength( size_t prev, size_t now )const;
    size_t CalcUpdateScape()const;
    void SetPositionByte( size_t pos );


  private:
    Sound::SPBUFFER     m_pBuffer;
		SPCUSTOMPCMREADER   m_pDecoder;
    SPMEMORYBUFFER      m_pData;
    bool  m_IsLoopPlay;

    size_t  m_NowPlayPosition;    //  再生位置
    size_t  m_PrevBufferPosition; //  前回の Update() 時にあった 再生位置
    size_t  m_WrittedBufferPosition;
    size_t  m_DecodedSize;  //  今現在デコードしてある長さ m_pDecoder->GetLength() を超えることができる
  };
}


#endif

