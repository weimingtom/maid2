//#define LOCAL_PROFILE

#include"streamdecoder.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/profile.h"
#include"../../auxiliary/debug/assert.h"

namespace Maid
{

StreamDecoder::StreamDecoder()
  :m_TargetTime(0)
{

}

StreamDecoder::~StreamDecoder()
{
  Finalize();
}

void StreamDecoder::Initialize( const Xiph::SPOGGSTREAM& pStream, const Xiph::SPCODEC& pCodec )
{
  Finalize();

  m_pStream = pStream;
  m_pCodec  = pCodec;
}

void StreamDecoder::Finalize()
{
  m_pStream.reset();
  m_pCodec.reset();
  m_TargetTime = 0;
}


void StreamDecoder::PageIn( const Xiph::OggPage& page )
{
  m_pStream->PageIn( page );
}



void StreamDecoder::BeginSeek( double TargetTime )
{
  MAID_ASSERT( TargetTime < 0, "負数は指定できません" << TargetTime );

  m_TargetTime = TargetTime;
}

bool StreamDecoder::IsSeeking() const
{
  return 0<m_TargetTime;
}

int  StreamDecoder::GetPacketCount() const
{
  return m_pStream->GetPacketCount();
}

bool  StreamDecoder::IsPacketEmpty() const
{
  return GetPacketCount()==0;
}


bool StreamDecoder::IsStreamEnd() const
{
  const bool end   = m_pStream->IsEnd();
  const bool empty = IsPacketEmpty();

  return end && empty;
}


void StreamDecoder::Update( STREAMSAMPLE& out )
{
  if( IsStreamEnd() ) { return; }
  if( IsPacketEmpty() ) { return; }


  Xiph::OggPacket packet;
  const bool IsSuccess = m_pStream->PacketOut(packet);
  if( !IsSuccess ) { return ; }

  if( !m_pCodec->IsSetupped() )
  {
    m_pCodec->Setup(packet);
    return ;
  }


  const double OldTime = m_pCodec->GetTime();
  const double SampleLen = m_pCodec->CalcPacketLength(packet);


  if( IsSeeking() )
  {
    //  シーク中の場合は追いつくまでスキップスキップ　らんらん・・・・るーぅぅぅぅ！
    const double Target = m_TargetTime;

    if( OldTime+SampleLen < Target )
    {
      m_pCodec->Skip( packet );
      return ;
    }

    //  追いついた。
    m_TargetTime = 0;
  }

  {
    SPSAMPLE pSample;
    m_pCodec->Decode( packet, pSample );

    if( pSample.get()!=NULL )
    {
      out.BeginTime = OldTime;
      out.EndTime   = OldTime+SampleLen;
      out.pSample   = pSample;
    }
  }
}

}