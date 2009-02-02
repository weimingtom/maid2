#include"decoder.h"

#include"../../source/auxiliary/mathematics.h"
#include"oggdecodernull.h"

/*

  ogg/theora/vorbisの内部構造の解説

  ogg はファイルフォーマットみたいなもので RIFF だと思うとわかりやすい

  v１つあたりが１画面
  s１つあたりがサウンド単位

  v0,v1,v2,v3    s0,s1,s2,s3




  それぞれの単位に ogg packet(p) がついてる

  p+v0,p+v1,p+v2,p+v3  p+s0,p+s1,p+s2,p+s3




  packet の塊が ogg page

  page[pv0,pv1,pv2,pv3]   page[ps0,ps1,ps2,ps3]




  ogg page を、パケット単位でアクセスできるようにしてるのが ogg stream

  stream.PageIn(page[pv0,pv1,pv2,pv3]);
  stream[0] == pv0;
  stream[1] == pv1;
  stream[2] == pv2;
  stream[3] == pv3;


  そのた細かいことは
  http://d.hatena.ne.jp/Ko-Ta/searchdiary?of=7&word=*[program,theora]
  を参照！
*/


using namespace Maid;

Decoder::Decoder()
:m_State(STATE_EMPTY)
,m_TheoraSerial(-1)
,m_VorbisSerial(-1)
{

}

void Decoder::Initialize()
{
  m_State = STATE_INITIALIZING;
  m_Thread.SetFunc( MakeThreadObject(&Decoder::ThreadFunction,this) );
  m_Thread.Execute();
}

unt Decoder::ThreadFunction( volatile ThreadController::BRIGEDATA& brige )
{
  m_Buffer.Initialize();

  //  oggコンテナになにがあるか(theora,vorbis等) を調べる
  //  theora,vorbis 以外はNULLにしちゃう
  //  ogg の構造上、各フォーマットデータは先頭にある。
  while( true )
  {
    if( m_Buffer.IsEnd() ) { break; }

    const OggPage& page = m_Buffer.GetCurrentPage();

    if( !page.IsBeginOfStream() ) { break; }

    const int serial = page.GetSerialNo();
    DATA& data = m_BindData[serial];

    {
      data.Stream.Initialize( serial );
      data.Stream.PageIn( page );

      const OggPacket& packet = data.Stream.GetCurrentPacket();
      ogg_packet& p = const_cast<ogg_packet&>(packet.Get());

      SPOGGDECODER pDec;

      {
        if( theora_packet_isheader(&p)==1 ) 
        {
          pDec.reset( new OggDecoderTheora );
          m_TheoraSerial = serial;
        }
        ef( vorbis_synthesis_idheader(&p)==1 )
        {
          pDec.reset( new OggDecoderVorbis );
          m_VorbisSerial = serial;
        }
        else { pDec.reset( new OggDecoderNULL ); }
      }
      pDec->Initialize();

      //  最初のページは１パケットしかない（と思う）のでここでループする必要はない
      pDec->Decode(packet,SPOGGBUFFER());

      data.pDecoder = pDec;
    }

    m_Buffer.NextPage();
  }

  const unt BEGINTIME = m_Timer.Get();

  m_State = STATE_WORKING;


  //  ではエンコードの開始
  while( true )
  {
    if( brige.IsExit ) { break; }

    {
      //  シーク命令があったら、ここでシークする


    }

    if( m_Buffer.IsEnd() )
    {
      continue;
    }

    const OggPage& page = m_Buffer.GetCurrentPage();

    BINDDATA::iterator ite = m_BindData.find(page.GetSerialNo());

    MAID_ASSERT( ite==m_BindData.end(), "仕様外のページが見つかりました" );

    DATA& dat = ite->second;

    dat.Stream.PageIn(page);

    while( true )
    {
      if( dat.Stream.IsPacketEmpty() ) { break; }

      while( true )
      {
        //  バッファがある程度あるなら、減るまで待機
        size_t count = 0;
        {
          dat.Mutex.Lock();
          count = dat.Buffer.size();
          dat.Mutex.Unlock();
        }

        if( count<5 ) { break; }
        else { ThreadController::Sleep(1); }
      }

      const OggPacket& packet = dat.Stream.GetCurrentPacket();

      SPOGGBUFFER pBuffer;

      dat.pDecoder->Decode(packet,pBuffer);

      if( pBuffer.get()!=NULL )
      {
        dat.Mutex.Lock();
        dat.Buffer.push_back(pBuffer);
        dat.Mutex.Unlock();
      }

      dat.Stream.NextPacket();
    }


    const unt now = m_Timer.Get();


    m_Buffer.NextPage();
  }

  m_State = STATE_FINALIZING;

  for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
  {
    DATA& dat = ite->second;

    dat.pDecoder->Finalize();
    dat.Stream.Finalize();
  }

  m_BindData.clear();


  m_Buffer.Finalize();

  m_State = STATE_EMPTY;
  return 0;
}


