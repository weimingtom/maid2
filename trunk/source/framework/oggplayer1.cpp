#include"oggplayer.h"

#include"../auxiliary/mathematics.h"
#include"../auxiliary/debug/warning.h"
#include"xiph/decodernull.h"



//  デバッグするときに便利なオプション
//  VSはプロセスと違うスレッドのステップ実行でフリーズすることがあるんだよね。
//#define TESTMODE

using namespace Maid;


//  ここの実装はゲーム側が呼び出す関数を書いておきます
//  デコードスレッドが使う関数は 2.cpp を参照


OggPlayer::OggPlayer()
:m_State(STATE_EMPTY)
,m_TheoraSerial(DECODER_EMPTY)
,m_VorbisSerial(DECODER_EMPTY)
,m_SeekPosition(-1.0)
{

}

void OggPlayer::Initialize( const String& FileName )
{
  m_State = STATE_INITIALIZING;

  m_FileName = FileName;


#ifdef TESTMODE
  ThreadController::BRIGEDATA brige;
  brige.IsExit = false;
  brige.IsExecute = false;
  brige.IsAssert = false;
  ThreadFunction( brige );
#else
  m_Thread.SetFunc( MakeThreadObject(&OggPlayer::ThreadFunction,this) );
  m_Thread.Execute();

#endif
}

bool OggPlayer::IsInitialized() const
{
  if( m_State==STATE_WORKING ) { return true; }
  if( m_State==STATE_SEEKING ) { return true; }

  return false;
}

bool OggPlayer::IsSeeking() const
{
  return m_State==STATE_SEEKING;
}



const OggPlayer::FILEINFO& OggPlayer::GetFileInfo() const
{
  MAID_ASSERT( !IsInitialized(), "まだ初期化されていません" );
  return m_FileInfo;
}

void OggPlayer::FlushImage( double TargetTime, Xiph::SPBUFFERIMAGE& pOutput )
{
  if( !IsInitialized() ) { return ; }
  if( m_TheoraSerial==DECODER_EMPTY ) { return ; }

  Xiph::SAMPLELIST SampleList;

  {
    DATA& dat = *(m_BindData[m_TheoraSerial]);

    Xiph::SampleCache& cache = dat.Cache;

    cache.Pop( TargetTime, SampleList );
  }

  if( SampleList.empty() ) { return ; }

  pOutput = boost::static_pointer_cast<Xiph::BufferImage>(SampleList.back().pBuffer);
}

void OggPlayer::FlushPCM( double TargetTime, Buffer& Output )
{
  if( !IsInitialized() ) { return ; }
  if( m_VorbisSerial==DECODER_EMPTY ) { return ; }

  Xiph::SAMPLELIST SampleList;

  {
    DATA& dat = *(m_BindData[m_VorbisSerial]);

    Xiph::SampleCache& cache = dat.Cache;

    cache.Pop( TargetTime, SampleList );
  }

  if( SampleList.empty() ) { return ; }

  //  デコードバッファの全長を求める
  {
    size_t len = 0;
    for( Xiph::SAMPLELIST::const_iterator ite=SampleList.begin(); ite!=SampleList.end(); ++ite )
    {
      const Xiph::BufferPCM& buffer = static_cast<Xiph::BufferPCM&>(*(ite->pBuffer));
      if( buffer.Data.empty() ) { continue; }
      const int channels = buffer.Data.size();

      len += buffer.Data[0].size() * channels * sizeof(int16);
    }

    //  拾ったサンプルが足りないこともあるので、そのときは追加する
    {
      const Xiph::SAMPLE& last = SampleList.back();
      const double zero_len = TargetTime - last.EndTime;
      if( 0 < zero_len )
      {
        len += m_FileInfo.Pcm.Format.CalcLength(zero_len);
      }
    }

    if( len==0 ) { return; }
    Output.Resize(len);
    ZERO( Output.GetPointer(0), len );
  }



  {
    //  それじゃ順番に流していこうか！
    int16* dst = (int16*)Output.GetPointer(0);

    for( Xiph::SAMPLELIST::const_iterator ite=SampleList.begin(); ite!=SampleList.end(); ++ite )
    {
      const Xiph::BufferPCM& buffer = static_cast<Xiph::BufferPCM&>(*(ite->pBuffer));
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

void OggPlayer::Seek( double time )
{
  MAID_ASSERT( m_State==STATE_EMPTY, "初期化してください" );
  MAID_ASSERT( m_State==STATE_INITIALIZING, "初期化中です" );
  MAID_ASSERT( m_State==STATE_FINALIZING, "後始末してます" );

  m_SeekPosition = time;
  m_State = STATE_SEEKING;
  //  ここで設定したシーク状態は
  //  バッファが溜まるまで維持されます
}
