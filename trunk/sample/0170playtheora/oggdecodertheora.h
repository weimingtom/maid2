#ifndef oggdecodertheora_h
#define oggdecodertheora_h


#include"../../source/config/define.h"
#include"../../source/auxiliary/macro.h"
#include"../../source/graphics/surface.h"
#include"../../source/auxiliary/timer.h"

#include"ioggdecoder.h"

#include <theora/theora.h>
#include <list>
#include <boost/smart_ptr.hpp>

class OggDecoderTheora : public IOggDecoder
{
public:
  OggDecoderTheora();
  virtual void Initialize();
  virtual void Update( const OggPacket& NewPacket );
  virtual void Finalize();

private:
  theora_info      m_Info;
  theora_comment   m_Comment;
  theora_state     m_State;

  int m_MaxPostProcessLevel;
  int m_CurrentPostProcessLevel;


  struct FRAME
  {
    FRAME( const boost::shared_ptr<Maid::Surface>& p, ogg_int64_t gra, double t)
      :pFrame(p), GranulePosition(gra), Time(t)
    {}
    boost::shared_ptr<Maid::Surface> pFrame;
    const ogg_int64_t GranulePosition;
    const double  Time;
  };

  std::list<FRAME>  m_Frame;

  Maid::Timer TIMER;

};


#endif