﻿//#define LOCAL_PROFILE

#include"oggplayersinglethread.h"

#include"../auxiliary/mathematics.h"
#include"../auxiliary/debug/warning.h"

#include"../auxiliary/debug/profile.h"

#include"oggplayerlocal.h"


using namespace Maid;


OggPlayerSingleThread::OggPlayerSingleThread()
:m_State(STATE_EMPTY)
,m_TheoraSerial(DECODER_EMPTY)
,m_VorbisSerial(DECODER_EMPTY)
,m_SeekPosition(-1.0)
{

}

OggPlayerSingleThread::~OggPlayerSingleThread()
{
  Finalize();
}

void OggPlayerSingleThread::Initialize( const String& FileName )
{
  m_State = STATE_INITIALIZING;

  m_FileName = FileName;

  InitializeStream( m_FileName );
}

void OggPlayerSingleThread::Finalize()
{
  FinalizeStream();
  m_FileName.clear();
}


bool OggPlayerSingleThread::IsInitialized() const
{
  if( m_State==STATE_WORKING ) { return true; }
  if( m_State==STATE_SEEKING ) { return true; }

  return false;
}

bool OggPlayerSingleThread::IsSeeking() const
{
  const bool ret = m_State==STATE_SEEKING || 0.0 <= m_SeekPosition;

  return ret ;
}



const OggPlayerSingleThread::FILEINFO& OggPlayerSingleThread::GetFileInfo() const
{
  MAID_ASSERT( !IsInitialized(), "まだ初期化されていません" );
  return m_FileInfo;
}

void OggPlayerSingleThread::FlushImage( Xiph::SPBUFFERIMAGE& pOutput )
{
  if( !IsInitialized() ) { return ; }
  if( m_TheoraSerial==DECODER_EMPTY ) { return ; }

  const double TargetTime = m_Timer.Get();

  MAID_PROFILE();

  Xiph::SAMPLELIST SampleList;

  int size = 0;
  {
    OggDecoderSingleThread& dat = *(m_BindData[m_TheoraSerial]);

    size = dat.PopCache( TargetTime, SampleList );
  }

  if( SampleList.empty() ) { return ; }

  pOutput = boost::static_pointer_cast<Xiph::BufferImage>(SampleList.back().pBuffer);
}

void OggPlayerSingleThread::FlushPCM( Buffer& Output )
{
  if( !IsInitialized() ) { return ; }
  if( m_VorbisSerial==DECODER_EMPTY ) { return ; }

  const double TargetTime = m_Timer.Get();

  MAID_PROFILE();

  Xiph::SAMPLELIST SampleList;

  //  指定した時間の１秒先まであれば十分かな
  {
    OggDecoderSingleThread& dat = *(m_BindData[m_VorbisSerial]);

    dat.PopCache( TargetTime+SAOUNDTIME/2, SampleList );
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

void OggPlayerSingleThread::Play()
{
  m_Timer.Start();
}


void OggPlayerSingleThread::Seek( double time )
{
  MAID_ASSERT( m_State==STATE_EMPTY, "初期化してください" );
  MAID_ASSERT( m_State==STATE_INITIALIZING, "初期化中です" );
  MAID_ASSERT( m_State==STATE_FINALIZING, "後始末してます" );
  MAID_ASSERT( m_State==STATE_SEEKING, "シーク中はダメです" );

  m_SeekPosition = time;
  //  ここで設定したシーク状態は
  //  バッファが溜まるまで維持されます
}

bool OggPlayerSingleThread::IsCacheFull() const
{
  return IsCacheFull(m_TheoraSerial) && IsCacheFull(m_VorbisSerial);
}

bool OggPlayerSingleThread::IsEnd() const
{
  return IsEnd(m_TheoraSerial) && IsEnd(m_VorbisSerial);
}

double OggPlayerSingleThread::GetPosition() const
{
  return m_Timer.Get();
}