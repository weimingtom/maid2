//#define LOCAL_PROFILE

#include"oggdecoder.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/profile.h"
#include"../../auxiliary/debug/assert.h"

#include"../decodersamplecache.h"

namespace Maid { namespace Movie { namespace Xiph {

OggDecoder::OggDecoder( const SPOGGSTREAM& pStream, const SPCODEC& pCodec )
  :m_pStream(pStream)
  ,m_pCodec(pCodec)
  ,m_SkipTargetTime(-1)
{

}

void OggDecoder::Initialize()
{
}

void OggDecoder::Finalize()
{
  m_pStream.reset();
  m_pCodec.reset();
}

void OggDecoder::AddSource( const SPSTORAGESAMPLE& buffer )
{
  const OggPage& page = *boost::shared_static_cast<OggPage>(buffer);
  m_pStream->PageIn( page );
}

void OggDecoder::FlushSample( double time, DECODERSAMPLELIST& pOut )
{
  DecoderSampleCache tmp;

  while( true )
  {
    if( IsPacketEmpty() ) { break; }

    DECODERSAMPLE sample;
    UpdateTime( sample );
    if( sample.pSample.get()!=NULL )
    {
      tmp.PushBack( sample );
    }
  }

  tmp.Pop( time, pOut );
}

bool OggDecoder::IsSourceFull() const
{
  //  パケットがない＝＝もっと欲しい
  //  パケットがいる＝＝まだいらない
  return IsPacketEmpty();
}


bool OggDecoder::IsSampleFull() const
{
  //  そとのクラスからサンプルを取得する(FlushSample())時にサンプルを作るので
  //  パケットがあれば満杯
  return !IsPacketEmpty();
}

bool OggDecoder::IsDecodeEnd() const
{
  const bool ret = m_pStream->IsEnd();

  return ret;
}


void OggDecoder::UpdateTime( DECODERSAMPLE& out )
{
  if( IsStreamEnd() ) { return; }
  if( IsPacketEmpty() ) { return; }

  OggPacket packet;
  const bool IsSuccess = m_pStream->PacketOut(packet);
  if( !IsSuccess ) { return ; }

  const double OldTime   = m_pCodec->GetTime();
  const double SampleLen = m_pCodec->CalcPacketLength(packet);


  if( IsSkipMode() )
  {
    if( OldTime < m_SkipTargetTime )
    {
      m_pCodec->Skip( packet );
      return ;
    }else
    {
      m_SkipTargetTime = -1;
    }
  }

  {
    SPSAMPLE pSample;
    m_pCodec->Decode( packet, pSample );

    if( pSample.get()!=NULL )
    {
      out.BeginTime = OldTime;
      out.EndTime   = m_pCodec->GetTime();
      out.pSample   = pSample;
    }
  }
}


int  OggDecoder::GetPacketCount() const
{
  return m_pStream->GetPacketCount();
}

bool  OggDecoder::IsPacketEmpty() const
{
  return GetPacketCount()==0;
}

bool OggDecoder::IsStreamEnd() const
{
  const bool end   = m_pStream->IsEnd();
  const bool empty = IsPacketEmpty();

  return end && empty;
}

double OggDecoder::GetTime() const
{
  return m_pCodec->GetTime();
}

void OggDecoder::BeginSkipMode( double targettime )
{
  MAID_ASSERT( targettime<0.0, "過去には指定できません" );
  m_SkipTargetTime = targettime;

}

bool OggDecoder::IsSkipMode() const
{
  return 0.0<=m_SkipTargetTime;
}



}}}