#ifndef decoder_h
#define decoder_h

#include"../../source/auxiliary/thread.h"
#include"../../source/graphics/surface.h"

#include"oggbuffer.h"
#include"oggstream.h"
#include"ioggdecoder.h"


#include<map>


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
  Maid::ThreadController  m_Thread;

  OggBuffer m_Buffer;

  struct DATA
  {
    OggStream     Stream;
    SPOGGDECODER  pDecoder;
  };

  std::map<int,DATA> m_BindData;

};

#endif
