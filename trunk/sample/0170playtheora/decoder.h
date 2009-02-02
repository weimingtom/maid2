#ifndef decoder_h
#define decoder_h

#include"../../source/auxiliary/thread.h"
#include"../../source/auxiliary/timer.h"
#include"../../source/graphics/surface.h"

#include"oggcontainer.h"
#include"oggstream.h"
#include"ioggdecoder.h"
#include"oggdecodertheora.h"
#include"oggdecodervorbis.h"


#include<map>
#include<list>
#include<boost/smart_ptr.hpp>


class Decoder
{
public:
  Decoder();

  void Initialize();

private:

  OggContainer m_Buffer;

  struct DATA
  {
    OggStream     Stream;
    SPOGGDECODER  pDecoder;

    Maid::ThreadMutex Mutex;
    std::list<SPOGGBUFFER>  Buffer;
  };

  typedef std::map<int,DATA> BINDDATA;
  BINDDATA m_BindData;

  int m_TheoraSerial;
  int m_VorbisSerial;


  Maid::Timer m_Timer;

  enum STATE
  {
    STATE_EMPTY,
    STATE_INITIALIZING,
    STATE_WORKING,
    STATE_FINALIZING,
  };

  STATE m_State;

private:
  unt ThreadFunction( volatile Maid::ThreadController::BRIGEDATA& state );
  Maid::ThreadController  m_Thread;
};

#endif
