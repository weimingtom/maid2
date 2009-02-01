#include"decoder.h"

#include"../../source/auxiliary/mathematics.h"


using namespace Maid;

/* Helper; just grab some more compressed bitstream and sync it for
   page extraction */
int buffer_data(FILE *in,ogg_sync_state *oy){
  char *buffer=ogg_sync_buffer(oy,4096);
  int bytes=fread(buffer,1,4096,in);
  ogg_sync_wrote(oy,bytes);
  return(bytes);
}



/* never forget that globals are a one-way ticket to Hell */
/* Ogg and codec state for demux/decode */
ogg_sync_state   oy;
ogg_page         og;
ogg_stream_state vo;
ogg_stream_state to;
theora_info      ti;
theora_comment   tc;
theora_state     td;
vorbis_info      vi;
vorbis_dsp_state vd;
vorbis_block     vb;
vorbis_comment   vc;

int              theora_p=0;
int              vorbis_p=0;
int              stateflag=0;
int          audiofd=-1;

/* single frame video buffering */
int          videobuf_ready=0;
ogg_int64_t  videobuf_granulepos=-1;
double       videobuf_time=0;

/* single audio fragment audio buffering */
int          audiobuf_fill=0;
int          audiobuf_ready=0;
ogg_int16_t *audiobuf = NULL;
ogg_int64_t  audiobuf_granulepos=0; /* time position of last sample */

long         audiofd_totalsize=-1;
int          audiofd_fragsize;      /* read and write only complete fragments
                                       so that SNDCTL_DSP_GETOSPACE is
                                       accurate immediately after a bank
                                       switch */
ogg_int64_t  audiofd_timer_calibrate=-1;

static void open_audio(){
//  int format=AFMT_S16_NE; /* host endian */
  int channels=vi.channels;
  int rate=vi.rate;

  audiofd = 100;
  audiofd_fragsize=sizeof(ogg_int16_t)*1024*1024;
  audiofd_totalsize=5*audiofd_fragsize;

  audiobuf=(ogg_int16_t*)malloc(audiofd_fragsize);
}

static void audio_close(void){
    free(audiobuf);
}



/* call this only immediately after unblocking from a full kernel
   having a newly empty fragment or at the point of DMA restart */
void audio_calibrate_timer(int restart){

  SYSTEMTIME systime;
  GetLocalTime(&systime);

  ogg_int64_t current_sample;
  ogg_int64_t new_time;

  new_time=systime.wSecond*1000+systime.wMilliseconds;

  if(restart){
    current_sample=audiobuf_granulepos-audiobuf_fill/2/vi.channels;
  }else
    current_sample=audiobuf_granulepos-
      (audiobuf_fill+audiofd_totalsize-audiofd_fragsize)/2/vi.channels;

  new_time-=1000*current_sample/vi.rate;

  audiofd_timer_calibrate=new_time;
}

/* get relative time since beginning playback, compensating for A/V
   drift */
double get_time(){
  static ogg_int64_t last=0;
  static ogg_int64_t up=0;
  ogg_int64_t now;

  SYSTEMTIME systime;
  GetLocalTime(&systime);
  now=systime.wSecond*1000+systime.wMilliseconds;

  if(audiofd_timer_calibrate==-1)audiofd_timer_calibrate=last=now;

  if(audiofd<0){
    /* no audio timer to worry about, we can just use the system clock */
    /* only one complication: If the process is suspended, we should
       reset timing to account for the gap in play time.  Do it the
       easy/hack way */
    if(now-last>1000)audiofd_timer_calibrate+=(now-last);
    last=now;
  }

  if(now-up>200){
    double timebase=(now-audiofd_timer_calibrate)*.001;
    int hundredths=timebase*100-(long)timebase*100;
    int seconds=(long)timebase%60;
    int minutes=((long)timebase/60)%60;
    int hours=(long)timebase/3600;

//    fprintf(stderr,"   Playing: %d:%02d:%02d.%02d                       \r",
//            hours,minutes,seconds,hundredths);
    up=now;
  }

  return (now-audiofd_timer_calibrate)*.001;

}


/* write a fragment to the OSS kernel audio API, but only if we can
   stuff in a whole fragment without blocking */
void audio_write_nonblocking(void){

  if(audiobuf_ready){
//    audio_buf_info info;
    long bytes;

//    ioctl(audiofd,SNDCTL_DSP_GETOSPACE,&info);
//    bytes=info.bytes;
    bytes=0;
    if(bytes>=audiofd_fragsize){
      if(bytes==audiofd_totalsize)audio_calibrate_timer(1);

      while(1){
      //  bytes=write(audiofd,audiobuf+(audiofd_fragsize-audiobuf_fill),
      //              audiofd_fragsize);

        if(bytes>0){

          if(bytes!=audiobuf_fill){
            /* shouldn't actually be possible... but eh */
            audiobuf_fill-=bytes;
          }else
            break;
        }
      }

      audiobuf_fill=0;
      audiobuf_ready=0;

    }
  }
}


RECT2DI rect;
static void open_video(void){

  rect.x = 0;
  rect.y = 0;
  rect.w = ti.frame_width;
  rect.h = ti.frame_height;

}


static void video_write(void){
  int i;
  yuv_buffer yuv;
  int crop_offset;
  theora_decode_YUVout(&td,&yuv);


  /* let's draw the data on a SDL screen (*screen) */
  /* deal with border stride */
  /* reverse u and v for SDL */
  /* and crop input properly, respecting the encoded frame rect */
  /* problems may exist for odd frame rect for some encodings */
/*
  crop_offset=ti.offset_x+yuv.y_stride*ti.offset_y;
  for(i=0;i<yuv_overlay->h;i++)
    memcpy(yuv_overlay->pixels[0]+yuv_overlay->pitches[0]*i,
           yuv.y+crop_offset+yuv.y_stride*i,
           yuv_overlay->w);
  crop_offset=(ti.offset_x/2)+(yuv.uv_stride)*(ti.offset_y/2);
  for(i=0;i<yuv_overlay->h/2;i++){
    memcpy(yuv_overlay->pixels[1]+yuv_overlay->pitches[1]*i,
           yuv.v+crop_offset+yuv.uv_stride*i,
           yuv_overlay->w/2);
    memcpy(yuv_overlay->pixels[2]+yuv_overlay->pitches[2]*i,
           yuv.u+crop_offset+yuv.uv_stride*i,
           yuv_overlay->w/2);
  }
*/

}

/* dump the theora (or vorbis) comment header */
static int dump_comments(theora_comment *tc){
  int i, len;
  char *value;
  FILE *out=stdout;

//  fprintf(out,"Encoded by %s\n",tc->vendor);
  if(tc->comments){
//    fprintf(out, "theora comment header:\n");
    for(i=0;i<tc->comments;i++){
      if(tc->user_comments[i]){
        len=tc->comment_lengths[i];
        value=(char*)malloc(len+1);
        memcpy(value,tc->user_comments[i],len);
        value[len]='\0';
//        fprintf(out, "\t%s\n", value);
        free(value);
      }
    }
  }
  return(0);
}

/* Report the encoder-specified colorspace for the video, if any.
   We don't actually make use of the information in this example;
   a real player should attempt to perform color correction for
   whatever display device it supports. */
static void report_colorspace(theora_info *ti)
{
    switch(ti->colorspace){
      case OC_CS_UNSPECIFIED:
        /* nothing to report */
        break;;
      case OC_CS_ITU_REC_470M:
//        fprintf(stderr,"  encoder specified ITU Rec 470M (NTSC) color.\n");
        break;;
      case OC_CS_ITU_REC_470BG:
//        fprintf(stderr,"  encoder specified ITU Rec 470BG (PAL) color.\n");
        break;;
      default:
//        fprintf(stderr,"warning: encoder specified unknown colorspace (%d).\n",
//            ti->colorspace);
        break;;
    }
}

/* helper: push a page into the appropriate steam */
/* this can be done blindly; a stream won't accept a page
                that doesn't belong to it */
static int queue_page(ogg_page *page){
  if(theora_p)ogg_stream_pagein(&to,page);
  if(vorbis_p)ogg_stream_pagein(&vo,page);
  return 0;
}

static void usage(void){
//  fprintf(stderr,
//          "Usage: player_example <file.ogv>\n"
//          "input is read from stdin if no file is passed on the command line\n"
//          "\n"
//  );
}


void Decoder::Initialize()
{
  m_Frame.resize(1);

  m_Thread.SetFunc( MakeThreadObject(&Decoder::ThreadFunction,this) );
  m_Thread.Execute();
}

unt Decoder::ThreadFunction( volatile ThreadController::BRIGEDATA& brige )
{


    infile = ::fopen( "nc4259.ogv", "rb" );
  
    frames = 0;
    dropped = 0;


      /* start up Ogg stream synchronization layer */
    //  oy はストリームバッファと思うとわかりやすい
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


  return 0;
}


