#include"profile.h"

#include"../../config/win32.h"
#include"../thread/win32/threadmutex.h"
#include"../thread/win32/threadcontroller.h"
#include<map>
#include<stack>
#include<stdio.h>

namespace Maid
{
  class Profile::Impl
  {
  public:
    Impl( const std::string& FileName, int Line );
    ~Impl();
    static void PrintLog( const std::string& FileName );


  private:

    LARGE_INTEGER m_BeginTime;
    std::string   m_MapIndex;


    struct DATA
    {
      DATA():ClockTotal(0),CallCount(0),ClockMax(0),ClockMin(~0){}
      unt64 ClockTotal; //  クロックの合計
      unt   CallCount;  //  呼び出し回数
      unt   ClockMax;   //  最大値
      unt   ClockMin;   //  最小値
    };

    typedef std::map<std::string,DATA> PROFILEMAP;

    struct THREADDATA
    {
      THREADDATA():PrevCallTime(0){}
      PROFILEMAP  Profile;
      unt64       PrevCallTime; //  直前にかかった関数の時間
    };

    typedef std::map<unt,THREADDATA> THREADMAP;



    static THREADMAP   s_ThreadMap;
    static ThreadMutex s_Mutex;
  };

Profile::Impl::THREADMAP  Profile::Impl::s_ThreadMap;
ThreadMutex Profile::Impl::s_Mutex;

  Profile::Impl::Impl( const std::string& FileName, int Line )
  {
    char buf [1024];
    sprintf( buf, "line:%d %s", Line, FileName.c_str() );
    m_MapIndex = buf;


    {
      ThreadMutexLocker lock(s_Mutex);
      
      const unt id = ThreadController::GetCurrentID();

      s_ThreadMap[id].PrevCallTime = 0;
    }

    QueryPerformanceCounter( &m_BeginTime );
  }

  Profile::Impl::~Impl()
  {
    LARGE_INTEGER EndTime;
    QueryPerformanceCounter( &EndTime );

    //  この値は関数全部の時間なので
    //  直前にプロファイルされている時間をさっぴく
    const unt64 TotalTime = EndTime.QuadPart - m_BeginTime.QuadPart;

    {
      ThreadMutexLocker lock(s_Mutex);
      const unt id = ThreadController::GetCurrentID();
      THREADDATA& threaddata = s_ThreadMap[id];
      DATA& dat = threaddata.Profile[m_MapIndex];

      const unt64 PrevTime = threaddata.PrevCallTime;
      const unt64 time = TotalTime - PrevTime;

      dat.ClockTotal += time;
      dat.CallCount  += 1;

      if( dat.ClockMax < time ) { dat.ClockMax = time; }
      if( time!=0 && time < dat.ClockMin ) { dat.ClockMin = time; } //  0 はちょっとｗ

      threaddata.PrevCallTime = TotalTime;
    }
  }

  void Profile::Impl::PrintLog( const std::string& FileName )
  {
    unt64 Freq = 0;
    {
      LARGE_INTEGER val;
      if( QueryPerformanceFrequency( &val )==FALSE) { return ; }
      Freq = val.QuadPart;
    }

    unt64 TotalClock = 0;

    {
      for( THREADMAP::const_iterator ite=s_ThreadMap.begin(); ite!=s_ThreadMap.end(); ++ite )
      {
        for( PROFILEMAP::const_iterator ite2=ite->second.Profile.begin(); ite2!=ite->second.Profile.end(); ++ite2 )
        {
          TotalClock += ite2->second.ClockTotal;
        }
      }

    }

    FILE* hFile = fopen( FileName.c_str(), "w" );

    for( THREADMAP::const_iterator ite=s_ThreadMap.begin(); ite!=s_ThreadMap.end(); ++ite )
    {
      unt64 ThreadClock = 0;

      //  スレッド単位で処理時間のおおい順にソートする
      //  ついでにスレッド合計時間を計算する
      typedef std::multimap<unt64,PROFILEMAP::const_iterator> SORTOBJECT;
      SORTOBJECT Sort;
      for( PROFILEMAP::const_iterator ite2=ite->second.Profile.begin(); ite2!=ite->second.Profile.end(); ++ite2 )
      {
        Sort.insert( std::make_pair(ite2->second.ClockTotal,ite2 ) );

        ThreadClock += ite2->second.ClockTotal;
      }

      fprintf( hFile, "thread %d------------------------\n", ite->first );

      for( SORTOBJECT::reverse_iterator ite2=Sort.rbegin(); ite2!=Sort.rend(); ++ite2 )
      {
        const std::string& name = ite2->second->first;
        const unt64 total = ite2->second->second.ClockTotal;
        const unt count = ite2->second->second.CallCount;
        const unt min = ite2->second->second.ClockMin;
        const unt max = ite2->second->second.ClockMax;

        const double time = double(total*10000/Freq) / 10000.0;

        const double norm = double(total*100*10000/TotalClock) / 10000.0;
        const double threadnorm = double(total*100*10000/ThreadClock) / 10000.0;
        const double average = threadnorm / double(count);
        const double average_t = time / double(count);

        const double min_t = double(min)/double(Freq);
        const double max_t = double(max)/double(Freq);

        fprintf( hFile, "use:%.3fsec\t%.3f%%(%.3f%%)\tcall:%d\tavg:%fsec\t%f%%\tminmax:%f-%f\t%s\n", time, threadnorm, norm, count, average_t, average, min_t, max_t, name.c_str() );

      }
    }

    fclose(hFile);
  }


  Profile::Profile( const std::string& FileName, int Line )
    :m_pImpl( new Impl(FileName,Line) )
  {

  }

  void Profile::PrintLog( const std::string& FileName )
  {
    Impl::PrintLog( FileName );
  }


}