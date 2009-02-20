//#define LOCAL_PROFILE

#include"movieplayer.h"

#include"../../auxiliary/mathematics.h"
#include"../../auxiliary/debug/warning.h"

#include"../../auxiliary/debug/profile.h"

#include"movieplayerlocal.h"


namespace Maid {


//  ここの実装はゲーム側が呼び出す関数を書いておきます
//  デコードスレッドが使う関数は 2.cpp を参照


MoviePlayer::MoviePlayer()
:m_State(STATE_EMPTY)
,m_TheoraSerial(DECODER_EMPTY)
,m_VorbisSerial(DECODER_EMPTY)
,m_SeekPosition(-1.0)
{

}

void MoviePlayer::Initialize( const String& FileName )
{
  m_State = STATE_INITIALIZING;

  m_FileName = FileName;


  m_Thread.SetFunc( MakeThreadObject(&MoviePlayer::ThreadFunction,this) );
  m_Thread.Execute();
  m_Thread.SetProcesserMask( 1 );
}

bool MoviePlayer::IsInitialized() const
{
  if( m_State==STATE_WORKING ) { return true; }
  if( m_State==STATE_SEEKING ) { return true; }

  return false;
}

bool MoviePlayer::IsSeeking() const
{
  const bool ret = m_State==STATE_SEEKING || 0.0 <= m_SeekPosition;

  return ret ;
}



const MoviePlayer::FILEINFO& MoviePlayer::GetFileInfo() const
{
  MAID_ASSERT( !IsInitialized(), "まだ初期化されていません" );
  return m_FileInfo;
}

void MoviePlayer::FlushImage( SPSAMPLEIMAGE& pOutput )
{
  if( !IsInitialized() ) { return ; }
  if( m_TheoraSerial==DECODER_EMPTY ) { return ; }

  const double TargetTime = m_Timer.Get();

  MAID_PROFILE();

  STREAMSAMPLELIST SampleList;

  int size = 0;
  {
    IStreamDecoderMultiThread& dat = *(m_BindData[m_TheoraSerial]);

    size = dat.PopCache( TargetTime, SampleList );
  }

  if( SampleList.empty() ) { return ; }

  pOutput = boost::static_pointer_cast<SampleImage>(SampleList.back().pSample);
}

void MoviePlayer::FlushPCM( Buffer& Output )
{
  if( !IsInitialized() ) { return ; }
  if( m_VorbisSerial==DECODER_EMPTY ) { return ; }

  const double TargetTime = m_Timer.Get();

  MAID_PROFILE();

  STREAMSAMPLELIST SampleList;

  //  指定した時間の１秒先まであれば十分かな
  {
    IStreamDecoderMultiThread& dat = *(m_BindData[m_VorbisSerial]);

    dat.PopCache( TargetTime+SAOUNDTIME/2, SampleList );
  }

  if( SampleList.empty() ) { return ; }

  //  デコードバッファの全長を求める
  {
    size_t len = 0;
    for( STREAMSAMPLELIST::const_iterator ite=SampleList.begin(); ite!=SampleList.end(); ++ite )
    {
      const SamplePCM& buffer = static_cast<SamplePCM&>(*(ite->pSample));
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

    for( STREAMSAMPLELIST::const_iterator ite=SampleList.begin(); ite!=SampleList.end(); ++ite )
    {
      const SamplePCM& buffer = static_cast<SamplePCM&>(*(ite->pSample));
      if( buffer.Data.empty() ) { continue; }
      const int channels = buffer.Data.size();
      const int count = buffer.Data[0].size();

      for( int i=0; i<count; ++i )
      {
        for( int j=0; j<channels; ++j )
        {
          const float val_f = buffer.Data[j][i];
          int val = int(val_f*32767.0f + 0.5f);
          if(val> 32767){ val= 32767; }
          ef(val<-32768){ val=-32768; }

          *dst = val;
          ++dst;
        }
      }
    }
  }
}

void MoviePlayer::Play()
{
  m_Timer.Start();
}


void MoviePlayer::Seek( double time )
{
  MAID_ASSERT( m_State==STATE_EMPTY, "初期化してください" );
  MAID_ASSERT( m_State==STATE_INITIALIZING, "初期化中です" );
  MAID_ASSERT( m_State==STATE_FINALIZING, "後始末してます" );
  MAID_ASSERT( m_State==STATE_SEEKING, "シーク中はダメです" );

  m_SeekPosition = time;
  //  ここで設定したシーク状態は
  //  バッファが溜まるまで維持されます
}

bool MoviePlayer::IsCacheFull() const
{
  return IsCacheFull(m_TheoraSerial) && IsCacheFull(m_VorbisSerial);
}

bool MoviePlayer::IsEnd() const
{
  return IsEnd(m_TheoraSerial) && IsEnd(m_VorbisSerial);
}

double MoviePlayer::GetPosition() const
{
  return m_Timer.Get();
}


}