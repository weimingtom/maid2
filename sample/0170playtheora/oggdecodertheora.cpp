#include"oggdecodertheora.h"

#include"../../source/auxiliary/debug/assert.h"
#include"../../source/auxiliary/debug/warning.h"
#include"../../source/graphics/imagefile/bitmap.h"
#include"../../source/storage/fileio/filewrite.h"

using namespace Maid;


//	あるとベンリなマクロ
union INTORFLOAT
{
	INTORFLOAT( float32 _f ) : f(_f){}
	INTORFLOAT( int32 _n ) : n(_n){}
	int32 n;
	float32 f;
};

static const INTORFLOAT bias(((23+127)<<23) +(1<<22) );

//	float32 -> int32 のキャスト
static inline int f2i( float32 f )
{
	INTORFLOAT a(f);

	a.f += bias.f;
	a.n -= bias.n;

	return a.n;
}

OggDecoderTheora::OggDecoderTheora()
:m_MaxPostProcessLevel(-1)
,m_CurrentPostProcessLevel(-1)
{
  ZERO( &m_Info, sizeof(m_Info) );
  ZERO( &m_Comment, sizeof(m_Comment) );
  ZERO( &m_State, sizeof(m_State) );
}


void OggDecoderTheora::Initialize()
{
  theora_info_init( &m_Info );
  theora_comment_init( &m_Comment );
}

void OggDecoderTheora::Finalize()
{
  theora_clear(&m_State);
  theora_comment_clear(&m_Comment);
  theora_info_clear(&m_Info);
}

void OggDecoderTheora::Update( const OggPacket& NewPacket )
{
  const ogg_int64_t no = NewPacket.GetPacketNo();
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  if( no<3 )
  {
    //  theora パケットは 0,1,2 がヘッダ

    const int ret = theora_decode_header(&m_Info,&m_Comment,&packet);
    if( ret<0 )  { MAID_WARNING( "theora_decode_header" ); return; }

    //  ヘッダパケットを全部処理したら初期化しておく
    if( no==2 )
    {
      theora_decode_init(&m_State,&m_Info);

      //  ポストプロセスレベルを最大にしておく
      //  ポストプロセスレベルってデコードの精度ってことでいいのかな？
      int pp;

      theora_control(&m_State,TH_DECCTL_GET_PPLEVEL_MAX,&pp, sizeof(pp));
      m_MaxPostProcessLevel=pp;
      m_CurrentPostProcessLevel=pp;
      theora_control(&m_State,TH_DECCTL_SET_PPLEVEL,&pp,sizeof(pp));
    }
  }
  else
  {
    //  そのほかはフレームデータ

    {
      /*HACK: This should be set after a seek or a gap, but we might not have
         a granulepos for the first packet (we only have them for the last
         packet on a page), so we just set it as often as we get it.
        To do this right, we should back-track from the last packet on the
         page and compute the correct granulepos for the first packet after
         a seek or a gap.*/
      //  訳がよくわからんけどシークした直後とかに呼ぶのかな？
      ogg_int64_t pos = NewPacket.GetGranulePosition();
      if( pos >= 0 )
      {
        theora_control(&m_State, TH_DECCTL_SET_GRANPOS, &pos, sizeof(pos));
      }
    }


    unt32 begin = TIMER.Get();
    unt32 end0 = begin;
    unt32 end1 = begin;

    const int ret = theora_decode_packetin(&m_State,&packet);
    if( ret==0 )
    {
      //  成功したんで、デコードする
      //  yuv の変換はここみてね
      //  http://vision.kuee.kyoto-u.ac.jp/~hiroaki/firewire/yuv.html
      //  http://d.hatena.ne.jp/Ko-Ta/20090124

      const SIZE2DI FrameSize = SIZE2DI(m_Info.frame_width,m_Info.frame_height);
      const SIZE2DI RealSize  = SIZE2DI(m_Info.width, m_Info.height );
      const theora_pixelformat fmt = m_Info.pixelformat;
      const ogg_int64_t gra = NewPacket.GetGranulePosition();
      const double time = theora_granule_time( &m_State, gra );

      boost::shared_ptr<Surface> pFrame( new Surface );
      {
        pFrame.reset( new Surface );
        yuv_buffer src;


        theora_decode_YUVout(&m_State, &src);
        end0 = TIMER.Get();

        //  パケット上は FrameSize の大きさで入っているけど
        //  実際は RealSize の大きさで作らないといけない
        pFrame->Create( FrameSize, PIXELFORMAT_X08R08G08B08I );


        const float inv219 = 1.0f / 219.0f;
        const float inv224 = 1.0f / 224.0f;

        //  いろいろパフォーマンスがいりそうなので、直接メモリに書き込む
        for( int y_y=0; y_y<src.y_height; ++y_y )
        {
          const int uv_y = y_y * src.uv_height / src.y_height ;

          const unsigned char* pSrcPixelY = src.y + src.y_stride  * y_y;
          const unsigned char* pSrcPixelU = src.u + src.uv_stride * uv_y;
          const unsigned char* pSrcPixelV = src.v + src.uv_stride * uv_y;
          unt08* pDstPixel = (unt08*)pFrame->GetLinePTR( m_Info.offset_y + y_y );
          pDstPixel += m_Info.offset_x * 4;

          const unt32 offset = (src.uv_width << 8) / src.y_width;
          unt32 fix_uv_x = 0;

          for( int y_x=0; y_x<src.y_width; ++y_x )
          {
            const unt32 uv_x = fix_uv_x >> 8;

            // theoraのYUVは デジダル信号なので Y(16～235)UV(16～240) の範囲になっているらしい
            //  それを全部 Y(0～1)UV(-0.5～0.5) にスケールする
            int y = pSrcPixelY[y_x ];
            int u = pSrcPixelU[uv_x];
            int v = pSrcPixelV[uv_x];

            y=(y-16)*298 >> 8;    //Y成分を16～235→0～256に伸縮
            u=(u-128)*291 >>8;    //U成分をCbに変換
            v=(v-128)*291 >>8;    //V成分をCrに変換
            int r = (y*256        +v*358) >> 8; //R = Y            + 1.40200V
            int g = (y*256 -u*88  -v*183) >> 8; //G = Y - 0.34414U - 0.71414V
            int b = (y*256 +u*454       ) >> 8; //B = Y + 1.77200U
            //0～255の閾値処理
            if( r<   0  ) { r=0; }
            ef(   255<r ) { r = 255; }
            if( g<   0  ) { g=0; }
            ef(   255<g ) { g = 255; }
            if( b<   0  ) { b=0; }
            ef(   255<b ) { b = 255; }
 
            unt08* dst = pDstPixel + (y_x*4);

            dst[0] = r;
            dst[1] = g;
            dst[2] = b;

/*
            const unsigned char y = pSrcPixelY[y_x ];
            const unsigned char u = pSrcPixelU[uv_x];
            const unsigned char v = pSrcPixelV[uv_x];
            const float Y = float(int(y)-16 )*inv219;
            const float U = float(int(u)-128)*inv224;
            const float V = float(int(v)-128)*inv224;

            const float R = Y +         0*U +  1.40200f*V;
            const float G = Y + -0.34414f*U + -0.71414f*V;
            const float B = Y +  1.77200f*U +         0*V;

            const unt08 r = (unt08)f2i(R*255.0f);
            const unt08 g = (unt08)f2i(G*255.0f);
            const unt08 b = (unt08)f2i(B*255.0f);

            unt08* dst = pDstPixel + (y_x*4);

            dst[0] = r;
            dst[1] = g;
            dst[2] = b;
*/
            fix_uv_x += offset;
          }
        }
      }

      if( pFrame.get()!=NULL )
      {
        m_Frame.push_back( FRAME(pFrame,gra,time) );

      end1 = TIMER.Get();

      MAID_WARNING( "time:" << (end0-begin) << " " << (end1-end0) );

      std::vector<unt08> FileImage;

      Bitmap::Save( *pFrame, FileImage );

      static int no;
      String str = String::PrintFormat( "hoge%04d.bmp", no );
      
      ++no;

      {
        FileWrite hFile;

        hFile.Open( str, FileWrite::OPENOPTION_NEW );
        hFile.Write( &(FileImage[0]), FileImage.size() );

      }

      }

    }
  }
}
