#ifndef framework_xiph_decodertheora_h
#define framework_xiph_decodertheora_h


#include"../../config/define.h"
#include"../../auxiliary/macro.h"
#include"../../auxiliary/thread.h"

#include"idecoder.h"
#include"isamplecache.h"

#include <theora/theora.h>


namespace Maid { namespace Xiph {

  class BufferImage
    : public IBuffer
  {
  public:
    SIZE2DI       SizeY;        //  YUV の Y 平面の大きさ
    int           PitchY;       //  Y平面のピッチ
    std::vector<unt08>  BitsY;  //  Y平面のデータ

    SIZE2DI       SizeUV;       //  YUV の UV 平面の大きさ
    int           PitchUV;      //  UV平面のピッチ
    std::vector<unt08>  BitsU;  //  U平面のデータ
    std::vector<unt08>  BitsV;  //  V平面のデータ
  };

  typedef boost::shared_ptr<BufferImage> SPBUFFERIMAGE;


  class SampleCacheTheora
    : public ISampleCache
  {
  public:
    void Flush( double TargetTime, SPBUFFER& Out );
    bool IsFull() const;
  };


  class DecoderTheora
    : public IDecoder
  {
  public:
    DecoderTheora();
    ~DecoderTheora();
    virtual void Initialize( const OggPacket& FirstPacket );
    virtual void Finalize();
    virtual void Decode( const OggPacket& NewPacket, SPBUFFER& pOut );
    virtual bool IsSetupped() const;
    virtual bool IsInitialized() const;

    virtual double GetTime();

    virtual void BeginSeekMode( double TargetTime );

    const theora_info& GetInfo() const;

  private:

    void SetPPLevel( int level );
    void OnSetup( const OggPacket& NewPacket );
    void OnDecode( const OggPacket& NewPacket, SPBUFFER& pOut );
    SPBUFFERIMAGE CreateDecodedBuffer() const;

  private:
    theora_info      m_TheoraInfo;
    theora_comment   m_TheoraComment;
    theora_state     m_TheoraState;

    int m_MaxPostProcessLevel;
    int m_CurrentPostProcessLevel;

    enum STATE
    {
      STATE_EMPTY,  //  何もしてない
      STATE_INITIALIZING, //  初期化中
      STATE_DECODING,     //  デコード中
    };

    STATE m_State;

    double  m_DecodeStartTime;  //  デコードを開始する時間
  };

}}

#endif