#ifndef maid2_framework_movie_mediatimer_h
#define maid2_framework_movie_mediatimer_h

#include"../../config/define.h"
#include"../../auxiliary/timer.h"

namespace Maid
{
  class IMediaTimer
  {
  public:
    virtual ~IMediaTimer(){}

    virtual double Get() const=0;
  };


  class MediaTimer
    :public IMediaTimer
  {
  public:
    MediaTimer();

    void Start();
    void Stop();
    void SetOffset( double time );

    virtual double Get() const;

  private:
    unt     m_StartTime;
    double  m_Offset;
    Timer   m_Timer;
  };
}

#endif