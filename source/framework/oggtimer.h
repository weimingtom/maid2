#ifndef framework_oggtimer_h
#define framework_oggtimer_h

#include"../config/define.h"
#include"../auxiliary/timer.h"

namespace Maid
{
  class IOggTimer
  {
  public:
    IOggTimer():m_Time(0){}
    virtual ~IOggTimer(){}

    double Get() const { return m_Time; }

  protected:
    double m_Time;
  };


  class OggTimer
    :public IOggTimer
  {
  public:
    OggTimer();

    void Start();
    void Start( double time );
    void Stop();
    void Resume();

    void Update();

  private:
    unt   m_StartTime;
    unt   m_Offset;
    Timer m_Timer;
  };
}

#endif