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

  DATA& dat = *(m_BindData[m_TheoraSerial]);

  Xiph::SampleCache& cache = dat.Cache;

  while( true )
  {
    if( cache.IsEmpty() ) { break; }

    if( TargetTime < cache.GetBeginTime() ) { break; }


    Xiph::SAMPLE sample;

    cache.PopFront( sample );

    bool ok = false;

    if( sample.BeginTime==sample.EndTime ) { ok = sample.BeginTime <=TargetTime; }
    else { ok = (sample.BeginTime<=TargetTime && TargetTime<sample.EndTime); }

    if( ok )
    {
      pOutput = boost::static_pointer_cast<Xiph::BufferImage>(sample.pBuffer);
      break;
    }
  }
}

void OggPlayer::FlushPCM( double TargetTime, Buffer& Output )
{
/*
  if( !IsInitialized() ) { return ; }
  if( m_VorbisSerial==DECODER_EMPTY ) { return ; }

  Xiph::SAMPLELIST SampleList;

  Output.Initialize();

  {
    DATA& dat = *(m_BindData[m_VorbisSerial]);
    dat.Cache.PopAll( SampleList );
    if( SampleList.empty() ) { return; }
  }

  { //  指定した時間より古いバッファは全部捨てる
    while( true )
    {
      if( SampleList.empty() ) { break; }
      if( SampleList.front().EndTime < TargetTime ) 
      {
        SampleList.pop_front();
      }else
      {
        break;
      }
    }
  }
  if( SampleList.empty() ) { return ; }

  {
    //  time がバッファの BeginTime で始まってるわけないので
    //  そのギャップを入れる
    const PCMFORMAT& fmt = m_Format;
    const float sa = float(TargetTime - BeginTime);
    const size_t len = size_t(double(fmt.SamplesPerSecond) * sa +0.5);

    //SPBUFFERPCM
    if( 0<len )
    {
      SPBUFFERPCM pBuff( new BufferPCM );

      pBuf->Data.resize( fmt.Channels );

      for( int i=0; i<fmt.Channels; ++i )
      {
        BufferPCM::BUFFER& buf = pBuf->Data[i];

        buf.resize( len )
        ZERO( &(buf[0]), len );
      }
      bufferlist.push_front(pBuff); //  ほほいと挿入
    }
  }

/*
  {
    Xiph::SPBUFFER pTmp; 
    pCache->Flush( time, pTmp );

    pOutput = boost::static_pointer_cast<Xiph::BufferPCM>(pTmp);
  }
*/
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
