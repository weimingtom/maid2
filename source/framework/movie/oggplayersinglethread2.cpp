#include"oggplayersinglethread.h"

#include"../auxiliary/mathematics.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/debug/profile.h"


#include"oggplayerlocal.h"

namespace Maid {

//  ここの実装はデコードスレッドが使う関数を書いておきます
//  ゲーム側が使う関数は 1.cpp を参照


//  シーク命令が出ているか？
bool OggPlayerSingleThread::IsSetSeek()const
{
  //  シーク中は新しいシーク命令を受け付けないので、m_State!=STATE_SEEKING の判定は問題ない
  return 0.0 <= m_SeekPosition && m_State!=STATE_SEEKING;
}



void OggPlayerSingleThread::Update()
{
  PlayDecode();
}



void OggPlayerSingleThread::InitializeStream( const String& FileName )
{
  m_Buffer.Initialize( FileName );


  //  oggコンテナになにがあるか(theora,vorbis等) を調べる
  //  theora,vorbis 以外はNULLにしちゃう
  //  ogg の構造上、各フォーマットデータは先頭にある。
  while( true )
  {
    if( m_Buffer.IsEnd() ) { break; }

    const Xiph::OggPage& page = m_Buffer.GetCurrentPage();

    if( !page.IsBeginOfStream() ) { break; }


    {
      const int serial = page.GetSerialNo();
      Xiph::SPOGGSTREAM pStream( new Xiph::OggStream );
      Xiph::SPDECODER   pDecoder;
      Xiph::SPSAMPLECACHECHECKER pChecker;

      pStream->Initialize( serial );
      pStream->PageIn( page );

      Xiph::OggPacket packet;
      
      pStream->PacketOut( packet );

      {
        ogg_packet& p = const_cast<ogg_packet&>(packet.Get());
        if( theora_packet_isheader(&p)==1 ) 
        {
          boost::shared_ptr<Xiph::DecoderTheora> pTheora( new Xiph::DecoderTheora );
          pTheora->Initialize(packet);

          FILEINFO& Info = m_FileInfo;

          Info.IsImage = true;
          const theora_info& th = pTheora->GetInfo();

          FILEINFO::IMAGE& img = Info.Image;
          img.EncodedSize = SIZE2DI(th.width, th.height);
          img.DisplaySize = SIZE2DI(th.frame_width,th.frame_width);
          img.DisplayOffset = POINT2DI( th.offset_x, th.offset_y );

          img.FpsNumerator = th.fps_numerator;
          img.FpsDenominator = th.fps_denominator;
          img.AspectNumerator = th.aspect_numerator;
          img.AspectDenominator = th.aspect_numerator;

          switch( th.pixelformat )
          {
          case OC_PF_444: { img.PixelFormat = FILEINFO::IMAGE::PIXELFORMAT_YUV444; }break;
          case OC_PF_422: { img.PixelFormat = FILEINFO::IMAGE::PIXELFORMAT_YUV422; }break;
          case OC_PF_420: { img.PixelFormat = FILEINFO::IMAGE::PIXELFORMAT_YUV420; }break;
          default: { MAID_ASSERT( true, "不明" ); }break;
          }
          m_TheoraSerial = serial;

          pDecoder = pTheora;
          pChecker.reset( new Xiph::SampleCacheCheckerCount(FRAMECOUNT) );
        }
        ef( vorbis_synthesis_idheader(&p)==1 )
        {
          boost::shared_ptr<Xiph::DecoderVorbis> pVorbis( new Xiph::DecoderVorbis );
          pVorbis->Initialize(packet);

          FILEINFO& Info = m_FileInfo;
          Info.IsPCM = true;

          const vorbis_info& vi = pVorbis->GetInfo();

          FILEINFO::PCM& pcm = Info.Pcm;

          pcm.Format.SamplesPerSecond = vi.rate;
          pcm.Format.Channels = vi.channels;
          pcm.Format.BitPerSamples = 16;

          m_VorbisSerial = serial;
          pDecoder = pVorbis;

          pChecker.reset( new Xiph::SampleCacheCheckerTime(SAOUNDTIME) );
        }
      }

      if( pDecoder.get()!=NULL )
      {
        //  最初のページは１パケットしかないのでここでループする必要はない
        SPDECODER pDec( new OggDecoderSingleThread(m_Timer) );
        pDec->Initialize(pStream,pDecoder,pChecker);
        m_BindData[serial] = pDec;
      }
    }
    m_Buffer.NextPage();
  }

  m_Timer.SetOffset(0);
}




void OggPlayerSingleThread::PlayDecode()
{
  bool IsLoop = true;
  while( IsLoop )
  {
#if 0
    if( IsSetSeek() )
    {
      const double target = m_SeekPosition;
      m_State = STATE_SEEKING;
      m_Timer.Stop();
      const double now = m_Timer.Get();

      if( now < target )
      { //  未来に飛ぶときは時間を変更するだけ
        for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
        {
          OggDecoder& decoder = *(ite->second);
          decoder.Seek(target);
        }
        m_Timer.SetOffset(m_SeekPosition);
      }else
      { //  過去に飛ぶときはいったん終了して、再度先頭からシークする
        break;
      }
    }
#endif

    bool IsAllCacheFull = true;

    //  すべてのデコーダに対して
    //  ・ページ待ちしてるとこにページを送る
    //  ・キャッシュがいっぱいになるまでデコードする
    for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
    {
      OggDecoderSingleThread& decoder = *(ite->second);
      if( decoder.IsStreamEnd()   ){ continue; }
      if( decoder.IsPacketEmpty() ){ PageSeek(ite->first); }
      if( decoder.IsCacheFull()   ){ continue; }
      decoder.Update();
      IsAllCacheFull = false;
    }

    //  全部あまってたらお休みする
    if( IsAllCacheFull )
    {
      m_State = STATE_WORKING;
      break;
    }
  }
}

void OggPlayerSingleThread::PageSeek( int Serial )
{
  while( true )
  {
    if( m_Buffer.IsEnd() ) { return; }

    while( true )
    {
      const Xiph::OggPage& page = m_Buffer.GetCurrentPage();

      const int PageSerial = page.GetSerialNo();

      BINDDATA::iterator ite = m_BindData.find( PageSerial );

      if( ite!=m_BindData.end() )
      {
        OggDecoderSingleThread& decoder = *(ite->second);
        //  ストリームが終わってたらいらないよな
        if( decoder.IsStreamEnd() ) { return ; }
        decoder.PageIn( page );
      }
      m_Buffer.NextPage();

      if( PageSerial==Serial && (0<page.GetPacketCount()) ) { return; }
    }
  }
}

bool OggPlayerSingleThread::IsCacheFull( int DecoderSerial ) const
{
  if( DecoderSerial==DECODER_EMPTY ) { return true; }

  BINDDATA::const_iterator ite = m_BindData.find(DecoderSerial);
  if( ite==m_BindData.end() ) { return true; }

  OggDecoderSingleThread& decoder = *(ite->second);
  if( decoder.IsStreamEnd() ) { return true; }

  return decoder.IsCacheFull();
}

bool OggPlayerSingleThread::IsEnd( int DecoderSerial )const
{
  if( DecoderSerial==DECODER_EMPTY ) { return true; }

  BINDDATA::const_iterator ite = m_BindData.find(DecoderSerial);
  if( ite==m_BindData.end() ) { return true; }

  OggDecoderSingleThread& decoder = *(ite->second);

  return decoder.IsStreamEnd();
}


void OggPlayerSingleThread::FinalizeStream()
{
  m_TheoraSerial = DECODER_EMPTY;
  m_VorbisSerial = DECODER_EMPTY;

  m_BindData.clear();
  m_Buffer.Finalize();
}



}