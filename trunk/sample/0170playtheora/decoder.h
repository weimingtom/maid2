#ifndef decoder_h
#define decoder_h

#include"../../source/auxiliary/thread.h"
#include"../../source/auxiliary/timer.h"
#include"../../source/graphics/surface.h"

#include"oggbuffer.h"
#include"oggstream.h"
#include"ioggdecoder.h"
#include"oggdecodertheora.h"
#include"oggdecodervorbis.h"


#include<map>
#include <boost/smart_ptr.hpp>


class Decoder
{
public:

  void Initialize();

private:
  unt ThreadFunction( volatile Maid::ThreadController::BRIGEDATA& state );

  struct FRAME
  {
    unt32   Frame;  //  この画像のフレーム値
    Maid::Surface Surf;   //  デコードされた画像

    enum STATE
    {
      STATE_EMPTY,    //  なにもない
      STATE_DECODING, //  作成中
      STATE_FINISH,   //  完成している
    };

    STATE State;
  };

  std::vector<FRAME>  m_Frame;

private:

  OggBuffer m_Buffer;

  struct DATA
  {
    OggStream     Stream;
    SPOGGDECODER  pDecoder;
  };

  typedef std::map<int,DATA> BINDDATA;
  BINDDATA m_BindData;

  boost::shared_ptr<OggDecoderTheora> m_pTheora;
  boost::shared_ptr<OggDecoderVorbis> m_pVorbis;


  Maid::Timer m_Timer;


private:
  Maid::ThreadController  m_Thread;
};

#endif
