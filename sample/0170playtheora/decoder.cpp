#include"decoder.h"

#include"../../source/auxiliary/mathematics.h"

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


void Decoder::Initialize()
{
  m_Frame.resize(1);

  m_Thread.SetFunc( MakeThreadObject(&Decoder::ThreadFunction,this) );
  m_Thread.Execute();
}

unt Decoder::ThreadFunction( volatile ThreadController::BRIGEDATA& brige )
{
  m_IsTheora = false;
  m_IsVorbis = false;

  m_Buffer.Initialize();


  theora_info_init( &m_Theora.Info );
  theora_comment_init( &m_Theora.Comment );

  vorbis_info_init( &m_Vorbis.Info );
  vorbis_comment_init( &m_Vorbis.Comment );


  //  theora があるか？　vorbis があるか？ を調べる
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

      const OggPacket& packet = data.Stream.GetCurrentPacket().Get();
      ogg_packet& p = const_cast<ogg_packet&>(packet.Get());

      SPOGGDECODER pDec;

      if( theora_packet_isheader(&p)==1 )
      {
        boost::shared_ptr<TheoraDecoder> pDec( new TheoraDecoder );
        pDec->Initialize();

        data.pDecoder = pDec;
      }
      ef( vorbis_synthesis_idheader(&p)==1 )
      {
        boost::shared_ptr<TheoraDecoder> pDec( new TheoraDecoder );
        pDec->Initialize();

        data.pDecoder = pDec;
      }
      else
      {

      }

    }
/*
    {
      boost::shared_ptr<OggStream> pStream( new OggStream );

      pStream->Initialize( page.GetSerialNo() );
      pStream->PageIn( page );

      ogg_packet& packet = pStream->GetCurrentPacket().Get();

      if( theora_packet_isheader(packet)==1 )
      {
        const int ret = theora_decode_header( &m_Theora.Info, &m_Theora.Comment, &packet );
        if( ret>=0 ) { m_pTheoraStream = pStream; }
        else         { MAID_WARNING( "theoraパケットだけど初期化に失敗" ); }
      }
      ef( vorbis_synthesis_idheader(packet)==1 )
      {
        const int ret = vorbis_synthesis_headerin( &m_Vorbis.Info, &m_Vorbis.Comment, &packet );
        if( ret>=0 ) { m_pVorbisStream = pStream; }
        else         { MAID_WARNING( "vorbisパケットだけど初期化に失敗" ); }
      }
    }
*/
    m_Buffer.NextPage();
  }

  {
    //  次に theoraヘッダ、vorbisヘッダを検索して、プログラム側に初期化する
    //  theoraヘッダ、vorbisヘッダ ともに３パケットある
    int theora_count=0;
    int vorbis_count=0;

    //  ストリームがないなら最初から終わっていることにしてしまう
    if( m_pTheoraStream.get()==NULL ) { theora_count=3; }
    if( m_pVorbisStream.get()==NULL ) { vorbis_count=3; }

    while( true )
    {
      if( m_Buffer.IsEnd() ) { break; }
      if( theora_count==3 && vorbis_count==3 ) { break; }

      const OggPage& page = m_Buffer.GetCurrentPage();







      m_Buffer.NextPage();
    }
  }




#if 0
    infile = ::fopen( "nc4259.ogv", "rb" );
  
    frames = 0;
    dropped = 0;


      /* start up Ogg stream synchronization layer */
      ogg_sync_init(&oy);

      /* init supporting Vorbis structures needed in header parsing */
      vorbis_info_init(&vi);
      vorbis_comment_init(&vc);

      /* init supporting Theora structures needed in header parsing */
      theora_comment_init(&tc);
      theora_info_init(&ti);

      /* Ogg file open; parse the headers */
      /* Only interested in Vorbis/Theora streams */
      while(!stateflag){
        int ret=buffer_data(infile,&oy);
        if(ret==0)break;

        while(ogg_sync_pageout(&oy,&og)>0){
          ogg_stream_state test;

          /* is this a mandated initial header? If not, stop parsing */
          if(!ogg_page_bos(&og)){
            /* don't leak the page; get it into the appropriate stream */
            queue_page(&og);
            stateflag=1;
            break;
          }

          ogg_stream_init(&test,ogg_page_serialno(&og));
          ogg_stream_pagein(&test,&og);
          ogg_stream_packetout(&test,&op);

          /* identify the codec: try theora */
          if(!theora_p && theora_decode_header(&ti,&tc,&op)>=0){
            /* it is theora */
            memcpy(&to,&test,sizeof(test));
            theora_p=1;
          }else if(!vorbis_p && vorbis_synthesis_headerin(&vi,&vc,&op)>=0){
            /* it is vorbis */
            memcpy(&vo,&test,sizeof(test));
            vorbis_p=1;
          }else{
            /* whatever it is, we don't care about it */
            ogg_stream_clear(&test);
          }
        }
        /* fall through to non-bos page parsing */
      }

      /* we're expecting more header packets. */
      while((theora_p && theora_p<3) || (vorbis_p && vorbis_p<3)){
        int ret;

        /* look for further theora headers */
        while(theora_p && (theora_p<3) && (ret=ogg_stream_packetout(&to,&op))){
          if(ret<0){
 //           fprintf(stderr,"Error parsing Theora stream headers; "
 //            "corrupt stream?\n");
 //           exit(1);
          }
          if(theora_decode_header(&ti,&tc,&op)){
 //           fprintf(stderr,"Error parsing Theora stream headers; "
 //            "corrupt stream?\n");
 //           exit(1);
          }
          theora_p++;
        }

        /* look for more vorbis header packets */
        while(vorbis_p && (vorbis_p<3) && (ret=ogg_stream_packetout(&vo,&op))){
          if(ret<0){
//            fprintf(stderr,"Error parsing Vorbis stream headers; corrupt stream?\n");
//            exit(1);
          }
          if(vorbis_synthesis_headerin(&vi,&vc,&op)){
//            fprintf(stderr,"Error parsing Vorbis stream headers; corrupt stream?\n");
//            exit(1);
          }
          vorbis_p++;
          if(vorbis_p==3)break;
        }

        /* The header pages/packets will arrive before anything else we
           care about, or the stream is not obeying spec */

        if(ogg_sync_pageout(&oy,&og)>0){
          queue_page(&og); /* demux into the appropriate stream */
        }else{
          int ret=buffer_data(infile,&oy); /* someone needs more data */
          if(ret==0){
//            fprintf(stderr,"End of file while searching for codec headers.\n");
//            exit(1);
          }
        }
      }

      /* and now we have it all.  initialize decoders */
      if(theora_p){
        theora_decode_init(&td,&ti);
//        printf("Ogg logical stream %lx is Theora %dx%d %.02f fps",
//               to.serialno,ti.width,ti.height,
//               (double)ti.fps_numerator/ti.fps_denominator);
/*
        switch(ti.pixelformat){
          case OC_PF_420: printf(" 4:2:0 video\n"); break;
          case OC_PF_422: printf(" 4:2:2 video\n"); break;
          case OC_PF_444: printf(" 4:4:4 video\n"); break;
          case OC_PF_RSVD:
          default:
//           printf(" video\n  (UNKNOWN Chroma sampling!)\n");
           break;
        }
*/
        if(ti.width!=ti.frame_width || ti.height!=ti.frame_height)
//          printf("  Frame content is %dx%d with offset (%d,%d).\n",
//               ti.frame_width, ti.frame_height, ti.offset_x, ti.offset_y);
        report_colorspace(&ti);
        dump_comments(&tc);
        theora_control(&td,TH_DECCTL_GET_PPLEVEL_MAX,&pp_level_max,
         sizeof(pp_level_max));
        pp_level=pp_level_max;
        theora_control(&td,TH_DECCTL_SET_PPLEVEL,&pp_level,sizeof(pp_level));
        pp_inc=0;
      }else{
        /* tear down the partial theora setup */
        theora_info_clear(&ti);
        theora_comment_clear(&tc);
      }
      if(vorbis_p){
        vorbis_synthesis_init(&vd,&vi);
        vorbis_block_init(&vd,&vb);
//        fprintf(stderr,"Ogg logical stream %lx is Vorbis %d channel %ld Hz audio.\n",
//                vo.serialno,vi.channels,vi.rate);
      }else{
        /* tear down the partial vorbis setup */
        vorbis_info_clear(&vi);
        vorbis_comment_clear(&vc);
      }

      /* open audio */
      if(vorbis_p)open_audio();

      /* open video */
      if(theora_p)open_video();





  stateflag=0; /* playback has not begun */
  while(!brige.IsExit){

    /* we want a video and audio frame ready to go at all times.  If
       we have to buffer incoming, buffer the compressed data (ie, let
       ogg do the buffering) */
    while(vorbis_p && !audiobuf_ready){
      int ret;
      float **pcm;

      /* if there's pending, decoded audio, grab it */
      if((ret=vorbis_synthesis_pcmout(&vd,&pcm))>0){
        int count=audiobuf_fill/2;
        int maxsamples=(audiofd_fragsize-audiobuf_fill)/2/vi.channels;
        for(i=0;i<ret && i<maxsamples;i++)
          for(j=0;j<vi.channels;j++){
            int val=int(pcm[j][i]*32767.f);
            if(val>32767)val=32767;
            if(val<-32768)val=-32768;
            audiobuf[count++]=val;
          }
        vorbis_synthesis_read(&vd,i);
        audiobuf_fill+=i*vi.channels*2;
        if(audiobuf_fill==audiofd_fragsize)audiobuf_ready=1;
        if(vd.granulepos>=0)
          audiobuf_granulepos=vd.granulepos-ret+i;
        else
          audiobuf_granulepos+=i;

      }else{

        /* no pending audio; is there a pending packet to decode? */
        if(ogg_stream_packetout(&vo,&op)>0){
          if(vorbis_synthesis(&vb,&op)==0) /* test for success! */
            vorbis_synthesis_blockin(&vd,&vb);
        }else   /* we need more data; break out to suck in another page */
          break;
      }
    }

    while(theora_p && !videobuf_ready){
      /* theora is one in, one out... */
      if(ogg_stream_packetout(&to,&op)>0){

        if(pp_inc){
          pp_level+=pp_inc;
          theora_control(&td,TH_DECCTL_SET_PPLEVEL,&pp_level,
           sizeof(pp_level));
          pp_inc=0;
        }
        /*HACK: This should be set after a seek or a gap, but we might not have
           a granulepos for the first packet (we only have them for the last
           packet on a page), so we just set it as often as we get it.
          To do this right, we should back-track from the last packet on the
           page and compute the correct granulepos for the first packet after
           a seek or a gap.*/
        if(op.granulepos>=0){
          theora_control(&td,TH_DECCTL_SET_GRANPOS,&op.granulepos,
           sizeof(op.granulepos));
        }
        if(theora_decode_packetin(&td,&op)==0){
          videobuf_granulepos=td.granulepos;
          videobuf_time=theora_granule_time(&td,videobuf_granulepos);
          frames++;

          /* is it already too old to be useful?  This is only actually
             useful cosmetically after a SIGSTOP.  Note that we have to
             decode the frame even if we don't show it (for now) due to
             keyframing.  Soon enough libtheora will be able to deal
             with non-keyframe seeks.  */

          if(videobuf_time>=get_time())
            videobuf_ready=1;
          else{
            /*If we are too slow, reduce the pp level.*/
            pp_inc=pp_level>0?-1:0;
            dropped++;
          }
        }

      }else
        break;
    }

    if(!videobuf_ready && !audiobuf_ready && feof(infile))break;

    if(!videobuf_ready || !audiobuf_ready){
      /* no data yet for somebody.  Grab another page */
      buffer_data(infile,&oy);
      while(ogg_sync_pageout(&oy,&og)>0){
        queue_page(&og);
      }
    }

    /* If playback has begun, top audio buffer off immediately. */
    if(stateflag) audio_write_nonblocking();

    /* are we at or past time for this video frame? */
    if(stateflag && videobuf_ready && videobuf_time<=get_time()){
      video_write();
      videobuf_ready=0;
    }
#if 0
    if(stateflag &&
       (audiobuf_ready || !vorbis_p) &&
       (videobuf_ready || !theora_p) &&
       !brige.IsExit){
      /* we have an audio frame ready (which means the audio buffer is
         full), it's not time to play video, so wait until one of the
         audio buffer is ready or it's near time to play video */

      /* set up select wait on the audiobuffer and a timeout for video */
      struct timeval timeout;
      fd_set writefs;
      fd_set empty;
      int n=0;

      FD_ZERO(&writefs);
      FD_ZERO(&empty);
      if(audiofd>=0){
        FD_SET(audiofd,&writefs);
        n=audiofd+1;
      }

      if(theora_p){
        double tdiff;
        long milliseconds;
        tdiff=videobuf_time-get_time();
        /*If we have lots of extra time, increase the post-processing level.*/
        if(tdiff>ti.fps_denominator*0.25/ti.fps_numerator){
          pp_inc=pp_level<pp_level_max?1:0;
        }
        else if(tdiff<ti.fps_denominator*0.05/ti.fps_numerator){
          pp_inc=pp_level>0?-1:0;
        }
        milliseconds=tdiff*1000-5;
        if(milliseconds>500)milliseconds=500;
        if(milliseconds>0){
          timeout.tv_sec=milliseconds/1000;
          timeout.tv_usec=(milliseconds%1000)*1000;

          n=select(n,&empty,&writefs,&empty,&timeout);
          if(n)audio_calibrate_timer(0);
        }
      }else{
        select(n,&empty,&writefs,&empty,NULL);
      }
    }
#endif
    /* if our buffers either don't exist or are ready to go,
       we can begin playback */
    if((!theora_p || videobuf_ready) &&
       (!vorbis_p || audiobuf_ready))stateflag=1;
    /* same if we've run out of input */
    if(feof(infile))stateflag=1;

  }

  /* tear it all down */

  audio_close();

  if(vorbis_p){
    ogg_stream_clear(&vo);
    vorbis_block_clear(&vb);
    vorbis_dsp_clear(&vd);
    vorbis_comment_clear(&vc);
    vorbis_info_clear(&vi);
  }
  if(theora_p){
    ogg_stream_clear(&to);
    theora_clear(&td);
    theora_comment_clear(&tc);
    theora_info_clear(&ti);
  }
  ogg_sync_clear(&oy);

  if(infile && infile!=stdin)fclose(infile);
#endif

  m_Buffer.Finalize();

  return 0;
}


