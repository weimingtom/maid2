//#define LOCAL_PROFILE

#include"movieplayer.h"

#include"../../auxiliary/mathematics.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/profile.h"
#include"../../auxiliary/debug/assert.h"


namespace Maid {


//  ここの実装はゲーム側が呼び出す関数を書いておきます
//  デコードスレッドが使う関数は 2.cpp を参照


MoviePlayer::MoviePlayer()
  :m_State(STATE_EMPTY)
{

}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! プレイヤーの再生準備を行う
/*!
    @param  FileName [i ] 再生させるファイル名（今のところogg/vorbis/theora専用）
 */
void MoviePlayer::Initialize( const String& FileName )
{
  m_FileName = FileName;
  SetPosition(0);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 再生準備が整ったか問い合せる
/*!
    @return 準備OKならtrue
            まだならfalse
 */
bool MoviePlayer::IsStandby() const
{
  if( m_State==STATE_WORKING ) { return true; }
  return false;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイル情報を取得する
/*!
    @return  ファイル情報
 */
const MoviePlayer::FILEINFO& MoviePlayer::GetFileInfo() const
{
  MAID_ASSERT( !IsStandby(), "まだ準備ができていません" );
  return m_FileInfo;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在表示させるべき画像を取得する
/*!
    @param  time    [ o]  この画像が表示する時間
    @param  pOutput [ o]  表示する画像のデータ（）
    @return 準備OKならtrue
            まだならfalse
 */
void MoviePlayer::FlushImage( double& time, Movie::SPSAMPLEFRAME& pOutput )
{
  if( m_State!=STATE_WORKING ) { return ; }

  const double TargetTime = m_Timer.Get();

  Movie::DECODERSAMPLELIST sample;
  {
    m_pManager->FlushSample( DECODERID_FRAME1, TargetTime, sample );
  }

  if( sample.empty() ) { return ; }

  pOutput = boost::static_pointer_cast<Movie::SampleFrame>(sample.back().pSample);
  time = sample.back().BeginTime;
}

void MoviePlayer::FlushPCM( double& time, MemoryBuffer& Output )
{
  if( m_State!=STATE_WORKING ) { return ; }

  const double TargetTime = m_Timer.Get();

  Movie::DECODERSAMPLELIST SampleList;

  //  指定した時間の１秒先まであれば十分かな
  {
//      ThreadMutexLocker lock(m_ManagerMutex);
    m_pManager->FlushSample( DECODERID_PCM1, TargetTime+1.0, SampleList );
  }

  //  汲み取ったデータに過去のものがあったら切り取るようにする
  {
    for( Movie::DECODERSAMPLELIST::const_iterator ite=SampleList.begin(); ite!=SampleList.end(); )
    {
      const double end = ite->EndTime;
      if( end < TargetTime )  { ite = SampleList.erase(ite); }
      else                    { ++ite; }
    }

    if( SampleList.empty() ) { return ; }

    const Movie::DECODERSAMPLE& sample = SampleList.front();
    if( sample.BeginTime < TargetTime )
    {
      Movie::SamplePCM& buffer = static_cast<Movie::SamplePCM&>(*(sample.pSample));
      const int channels = buffer.Data.size();
      const double sa = TargetTime - sample.BeginTime;
      const double sps = (double)GetFileInfo().Pcm.Format.SamplesPerSecond;

      const size_t gomi = size_t(sps * sa);

      for( int i=0; i<channels; ++i )
      {
        Movie::SamplePCM::BUFFER& buf = buffer.Data[i];
        const size_t copycount = buf.size()-gomi;

        if( copycount!=0 ) { memmove( &buf[0], &buf[gomi], copycount*sizeof(float) ); }
        buf.resize( copycount );
      }
    }
  }
  time = SampleList.front().BeginTime;


  //  デコードバッファの全長を求める
  {
    size_t len = 0;
    for( Movie::DECODERSAMPLELIST::const_iterator ite=SampleList.begin(); ite!=SampleList.end(); ++ite )
    {
      const Movie::SamplePCM& buffer = static_cast<Movie::SamplePCM&>(*(ite->pSample));
      if( buffer.Data.empty() ) { continue; }
      const int channels = buffer.Data.size();

      len += buffer.Data[0].size() * channels * sizeof(int16);
    }

    if( len==0 ) { return; }

    Output.Resize(len);
    ZERO( Output.GetPointer(0), len );
  }

  {
    //  それじゃ順番に流していこうか！
    int16* dst = (int16*)Output.GetPointer(0);

    for( Movie::DECODERSAMPLELIST::const_iterator ite=SampleList.begin(); ite!=SampleList.end(); ++ite )
    {
      const Movie::SamplePCM& buffer = static_cast<Movie::SamplePCM&>(*(ite->pSample));
      if( buffer.Data.empty() ) { continue; }
      const size_t count = buffer.Data[0].size();
      const size_t channels = buffer.Data.size();

      for( size_t i=0; i<count; ++i )
      {
        for( size_t j=0; j<channels; ++j )
        {
          const float val_f = buffer.Data[j][i];
          int val = int(val_f*32767.0f + 0.5f);
          if(val> 32767){ val= 32767; }
          else if(val<-32768){ val=-32768; }

          *dst = val;
          ++dst;
        }
      }
    }
  }
}


void MoviePlayer::Play()
{
  MAID_ASSERT( !IsStandby(), "まだ準備ができていません" );
  m_Timer.Start();
}

void MoviePlayer::Stop()
{
  MAID_ASSERT( !IsStandby(), "まだ準備ができていません" );
  m_Timer.Stop();
}


void MoviePlayer::SetPosition( double time )
{
  m_Thread.Close();
  m_Timer.Stop();

  m_State = STATE_INITIALIZING;
  m_Timer.SetOffset( time );
  m_Thread.SetFunc( MakeThreadObject(&MoviePlayer::ThreadFunction,this) );
  m_Thread.Execute();
}

double MoviePlayer::GetPosition() const
{
  if( m_State==STATE_WORKING ) { return m_Timer.Get(); }
  return 0.0;
}

bool MoviePlayer::IsCacheFull() const
{
  return m_pManager->IsSampleFull();
}

bool MoviePlayer::IsEnd() const
{
  return m_State==STATE_END;
}


}