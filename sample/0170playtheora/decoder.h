#ifndef decoder_h
#define decoder_h

#include"../../source/auxiliary/thread.h"
#include"../../source/graphics/surface.h"

#include <theora/theora.h>
#include <vorbis/codec.h>

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

  int pp_level_max;
  int pp_level;
  int pp_inc;
  int i,j;
  ogg_packet op;

  FILE *infile;

  int frames;
  int dropped;

};

#endif
