﻿#ifndef maid2_framework_streamdecoder_h
#define maid2_framework_streamdecoder_h

#include"../../config/define.h"
#include"../../auxiliary/thread.h"

#include"xiph/oggstream.h"
#include"xiph/icodec.h"
#include"mediatimer.h"
#include"isample.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  class StreamDecoder
  {
  public:
    StreamDecoder();
    ~StreamDecoder();

    void Initialize( const Xiph::SPOGGSTREAM& pStream, const Xiph::SPCODEC& pCodec );
    void Finalize();
    void PageIn( const Xiph::OggPage& page );

    void BeginSeek( double TargetTime );
    bool IsSeeking() const;

    void Update( STREAMSAMPLE& out );

    int  GetPacketCount() const;
    bool IsStreamEnd() const;
    bool IsPacketEmpty() const;


  private:
    Xiph::SPOGGSTREAM  m_pStream;
    Xiph::SPCODEC      m_pCodec;

    double    m_TargetTime; //  シーク時に目標とする時間
  };

  typedef boost::shared_ptr<StreamDecoder> SPSTREAMDECODER;

}

#endif