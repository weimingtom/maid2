#ifndef decoder_h
#define decoder_h

#include"../../source/auxiliary/thread.h"
#include"../../source/graphics/surface.h"

#include <theora/theora.h>
#include <vorbis/codec.h>
#include <boost/smart_ptr.hpp>

#include"oggbuffer.h"
#include"oggstream.h"

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

  struct THEORA
  {
    theora_info      Info;
    theora_comment   Comment;
    theora_state     State;
  };
  THEORA m_Theora;
  boost::shared_ptr<OggStream> m_pTheoraStream;



  struct VORBIS
  {
    vorbis_info      Info;
    vorbis_comment   Comment;
    vorbis_dsp_state State;
    vorbis_block     Block;
  };

  VORBIS m_Vorbis;
  boost::shared_ptr<OggStream> m_pVorbisStream;
};

#endif
