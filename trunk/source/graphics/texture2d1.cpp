#include"texture2d.h"
#include"imagefile.h"
#include"../storage/filereadersync.h"
#include"../auxiliary/thread.h"
#include"../auxiliary/debug/warning.h"
#include"../storage/filewriter.h"

#include"surface.h"
#include"transiter.h"

#include"texture2dlocal.h"

namespace Maid
{
  namespace KEEPOUT
  {
    void tex2dFunction::Execute( const IJobInput& Input, IJobOutput& Output )
    {
      const tex2dInput&  in  = static_cast<const tex2dInput&>(Input);
      tex2dOutput& out = static_cast<tex2dOutput&>(Output);

      const tex2dInput::CREATECONFIG&  setting = in.Config;

      std::vector<SurfaceInstance> ImageSurface;
      {
        ImageSurface.reserve( 13 );  //  4096x4096 - 1x1 までの配列が 13 なのでそれぐらいあればいいでしょう
        const FUNCTIONRESULT ret = LoadImage( setting, ImageSurface );
        if( FUNCTIONRESULT_FAILE(ret) ) { MAID_WARNING( MAIDTEXT("失敗") << DebugString(setting) ) return ; }

        const int miplevel = CalcMipLevels( *(in.Core), setting, ImageSurface[0].GetSize() );

        if( in.Core->IsTextureMipMap() && ImageSurface.size()!=miplevel )
        {
          GenerateSublevel( ImageSurface, miplevel );
        }
      }


      //  これで最終的に作りたい画像データが ImageSurface に読み込んだデータが入った。
      //  次に各種設定から作成するテクスチャフォーマットを求める
      const PIXELFORMAT DstFormat = in.Core->FindFormatTexture2D( ImageSurface[0].GetPixelFormat(), false );
      if( DstFormat==PIXELFORMAT_NONE )
      {
        MAID_WARNING( MAIDTEXT("このフォーマットは対応していません ") << ImageSurface[0].GetPixelFormat() );
        return ; 
      }

      const SIZE2DI ImgSize = ImageSurface[0].GetSize();
      SIZE2DI TextureSize;
      SIZE2DI RealSize;

      in.Core->CalcTextureSize( ImgSize, RealSize, TextureSize );

      std::vector<SurfaceInstance> SubResourceSurface;

      {
        const int level = CalcMipLevels( *(in.Core), setting, TextureSize );
       
        SubResourceSurface.resize( level );  
        SIZE2DI NowSize = TextureSize;
        for( int i=0; i<level; ++i )
        {
          SubResourceSurface[i].Create( NowSize, DstFormat );
          NowSize.w /= 2; if( NowSize.w<1 ) { NowSize.w = 1; }
          NowSize.h /= 2; if( NowSize.h<1 ) { NowSize.h = 1; }
        }
        ConvertSubResource( ImageSurface, SubResourceSurface  );
      }

      //  求まったのでテクスチャを実際に作る
      Graphics::SPTEXTURE2D pTexture;
      {
        Graphics::CREATERETEXTURE2DPARAM param;

        param.Size = TextureSize;
        param.MipLevels = (int)SubResourceSurface.size();
        param.ArraySize = 1;
        param.Format = DstFormat;
        param.Usage = Graphics::RESOURCEUSAGE_DEFAULT;
        param.BindFlags = Graphics::RESOURCEBINDFLAG_MATERIAL;

        std::vector<Graphics::SUBRESOURCE> sub;
        
        {
          const int count = (int)SubResourceSurface.size();
          for( int i=0; i<count; ++i )
          {
            const Surface& surf = SubResourceSurface[i];
            Graphics::SUBRESOURCE res;
            res.pData = surf.GetPlanePTR();
            res.Pitch = surf.GetPitch();
            res.Slice = surf.GetPitch() * surf.GetSize().h;
            sub.push_back(res);
          }
        }

        pTexture = in.Core->GetDevice()->CreateTexture2D( param, &(sub[0]) );
        if( pTexture.get()==NULL ) {
          MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") << DebugString(setting) );
          return ; 
        }
      }

      Graphics::SPMATERIAL pMaterial;
      {
        pMaterial = in.Core->GetDevice()->CreateMaterial( pTexture, NULL );
        if( pMaterial.get()==NULL )
        {
          MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") << DebugString(setting) );
          return ; 
        }
      }
   
      out.pTexture  = pTexture;
      out.pMaterial = pMaterial;
      out.ImageSize   = ImgSize;
      out.CreateSize  = RealSize;
      out.TextureSize = TextureSize;
      out.ImageFormat = ImageSurface[0].GetPixelFormat();
    }

    int tex2dFunction::CalcMipLevels( const GraphicsCore& core, const tex2dInput::CREATECONFIG& setting, const SIZE2DI& size ) const
    {
      int level = core.CalcMipLevels( size );
      {
        if( !core.IsTextureMipMap() ) { level = 1; }
        else
        {
          tex2dInput::CREATECONFIG::const_iterator ite = setting.find(ELEMENT_MIPMAPLEVEL);
          if( ite!=setting.end() )
          {
            const int val = String::AtoI( ite->second );
            if( 0!=val ) { level = std::min(level,val); }
          }
        }
      }
      return level;
    }


    FUNCTIONRESULT tex2dFunction::ConvertSubResource( const std::vector<SurfaceInstance>& SrcImage, std::vector<SurfaceInstance>& DstImage  )
    {
      //  転送先が１枚しかない場合はミップマップがなということなので、無駄な領域の塗りつぶしをしない
      bool mipfill = DstImage.size()>1;

      for( int i=0; i<(int)DstImage.size(); ++i )
      {
        const Surface& src = i<(int)SrcImage.size()? SrcImage[i] : SrcImage.back();
              Surface& dst = DstImage[i];

        const SIZE2DI SrcSize = src.GetSize();
        const SIZE2DI DstSize = dst.GetSize();

        if( SrcSize.w <= DstSize.w && SrcSize.h <= DstSize.h )
        { //  画像より作成するサイズの方が大きいなら普通に転送
          const RECT2DI src_rc( POINT2DI(0,0), SrcSize );
          const RECT2DI dst_rc( POINT2DI(0,0), SrcSize );
          Transiter::Copy(src, src_rc, dst, dst_rc);
        }else
        { //  小さいなら縮小して転送
          #pragma  COMPILERMSG( "TODO:バイキュービックとかフィルタ選べるとカッコイイなぁ。" )

          const SIZE2DI tmp( std::min(SrcSize.w,DstSize.w) ,  std::min(SrcSize.h,DstSize.h) );
          const RECT2DI src_rc( POINT2DI(0,0), SrcSize );
          const RECT2DI dst_rc( POINT2DI(0,0), tmp );

          Transiter::Average( src, src_rc, dst, dst_rc );
        }

        if( mipfill )
        {
          //  塗りつぶしてない部分を隅っこの色で塗る
          for( int y=0; y<DstSize.h; ++y )
          {
            const COLOR_R32G32B32A32F src_color = dst.GetPixel( POINT2DI(SrcSize.w-1,y) );
            for( int x=SrcSize.w; x<DstSize.w; ++x ) 
            {
              const POINT2DI d(x,y);
              dst.SetPixel( d, src_color );
            }
          }
          for( int y=SrcSize.h; y<DstSize.h; ++y )
          {
            const int w = dst.GetWidthByte();

            ::memcpy( dst.GetLinePTR(y), dst.GetLinePTR(SrcSize.h-1), w );
          }
        }
      }
      return FUNCTIONRESULT_OK;
    }

    void tex2dFunction::GenerateSublevel( std::vector<SurfaceInstance>& target, int level )
    {
      MAID_ASSERT( target[0].IsEmpty(), "最初のレベルは作ってください" );

      int sublevel = 0;

      {
        const SIZE2DI& size = target[0].GetSize();
        int p = std::max( size.w, size.h );
        while( true )
        {
          if( p==0 ) { break; }
          p >>= 1;
          ++sublevel;
        }
      }

      sublevel = std::min(sublevel,level);

      target.resize( sublevel + 1 );

      for( int i=0; i<sublevel; ++i )
      {
        const SurfaceInstance& src = target[i];
        SurfaceInstance& dst = target[i+1];

        const SIZE2DI src_size = src.GetSize();
        const SIZE2DI dst_size = SIZE2DI(std::max(src_size.w/2,1), std::max(src_size.h/2,1) );

        dst.Create( dst_size, src.GetPixelFormat() );

        for( int dst_y=0; dst_y<dst_size.h; ++dst_y ) {
          for( int dst_x=0; dst_x<dst_size.w; ++dst_x ) { 

            const int x = dst_x*2;
            const int y = dst_y*2;

            float r=0;
            float g=0;
            float b=0;
            float a=0;
            int count = 0;

            if( x<src_size.w && y<src_size.h )
            {
              const COLOR_R32G32B32A32F c = src.GetPixel( POINT2DI(x  ,y  ) );
              r += c.GetR(); g += c.GetG(); b += c.GetB(); a += c.GetA();
              ++count;
            }

            if( x+1<src_size.w && y<src_size.h )
            {
              const COLOR_R32G32B32A32F c = src.GetPixel( POINT2DI(x+1  ,y  ) );
              r += c.GetR(); g += c.GetG(); b += c.GetB(); a += c.GetA();
              ++count;
            }
            if( x <src_size.w && y+1<src_size.h )
            {
              const COLOR_R32G32B32A32F c = src.GetPixel( POINT2DI(x   ,y+1  ) );
              r += c.GetR(); g += c.GetG(); b += c.GetB(); a += c.GetA();
              ++count;
            }
            if( x+1 <src_size.w && y+1<src_size.h )
            {
              const COLOR_R32G32B32A32F c = src.GetPixel( POINT2DI(x+1   ,y+1  ) );
              r += c.GetR(); g += c.GetG(); b += c.GetB(); a += c.GetA();
              ++count;
            }

            const float inv_div = 1.0f / float(count);
            r *= inv_div; g *= inv_div; b *= inv_div; a *= inv_div;
            dst.SetPixel( POINT2DI(dst_x,dst_y), COLOR_R32G32B32A32F(r,g,b,a) );
          }
        }
      }
    }

    String tex2dFunction::DebugString( const tex2dInput::CREATECONFIG& Element )const
    {
      String ret;

      for( tex2dInput::CREATECONFIG::const_iterator ite=Element.begin();
            ite!=Element.end(); ++ite )
      {
        ret += MAIDTEXT("<") + ite->first + MAIDTEXT(":") + ite->second + MAIDTEXT(">");
      }
      return ret;
    }
  }


Texture2D::CACHE::INFOMAP  Texture2D::CACHE::s_InfoMap;
ThreadMutex  Texture2D::CACHE::s_Mutex;

void Texture2D::LoadFile( const String& FileName )
{
  LoadFile( FileName, 0 );
}

void Texture2D::LoadFile( const String& FileName, int MipmapLevel )
{
  KEEPOUT::tex2dInput::CREATECONFIG com;

  if( FileName[0]=='<' )
  {
    ReadConvertSetting( FileName, com );

    com[ELEMENT_MIPMAPLEVEL] = String::PrintFormat("%0d",MipmapLevel);
  }else
  {
    com[ELEMENT_COLOR]       = FileName;
    com[ELEMENT_MIPMAPLEVEL] = String::PrintFormat("%0d",MipmapLevel);
  }

  SendCommand( com );
  m_LoadText = FileName;
}

void Texture2D::LoadCommand( const String& Command )
{
  KEEPOUT::tex2dInput::CREATECONFIG com;
  ReadConvertSetting( Command, com );
  SendCommand( com );
  m_LoadText = Command;
}




void Texture2D::Delete()
{
  m_Cache.Reset();
  m_LoadText = String();
}


bool Texture2D::IsLoading()const
{
  //  ポインタが存在してるならすでに作ってる
  if( Texture2DBase::IsSetupped() ) { return false; }

  //  処理が終わるまでは忙しい
  if( m_Cache.IsExecuting() ) { return true; }

  const KEEPOUT::tex2dOutput& out = m_Cache.GetOutput();

  const_cast<Texture2D*>(this)->Setup( out.pTexture, out.pMaterial, out.ImageSize, out.CreateSize, out.ImageFormat );

  return false;

}

const SIZE2DI& Texture2D::GetImageSize()const
{
  return m_Cache.GetOutput().ImageSize;
}

bool Texture2D::IsEmpty() const
{
  return m_Cache.IsEmpty();
}

String Texture2D::GetLoadText() const
{
  if( IsEmpty() ) { return String(); }

  return m_LoadText;
}

//! このインスタンスを使用する準備ができているか？
/*!
    @return  使えるなら true
 */
bool Texture2D::IsSetupped() const
{
  if( IsEmpty() ) { return false; }
  if( IsLoading() ) { return false; }

  return true;
}



void Texture2D::SendCommand( const KEEPOUT::tex2dInput::CREATECONFIG& Command )
{
  Texture2DBase::Clear();
  if( Command.empty() ) { return ; }
  m_Cache.Start( KEEPOUT::tex2dInput(Command, GlobalPointer<GraphicsCore>::Get() ) );
}



void Texture2D::ReadConvertSetting( const String& Command, KEEPOUT::tex2dInput::CREATECONFIG& out )
{
  KEEPOUT::tex2dInput::CREATECONFIG& Element = out;

  if( Command[0]!='<' ) { Element[ELEMENT_COLOR] = Command; } 
  else
  {
    unt32 begin = 0;
    unt32 len = 0;
    while( true )
    {
      if( Command.length() <= begin+len ) { break; }

      const unt32 c = Command[begin+len];
      ++len;
      if( c=='>' )
      {
        String tag = Command.substr(begin,len);
        String ele;
        String value;

        ReadName( tag, ele, value );
        Element[ele] = value;

        begin += len;
        len = 0;
      }
    }
  }
}


void Texture2D::ReadName( const String& Tag, String& Element, String& Value )
{
  MAID_ASSERT( Tag[0]!='<', "解析するファイル名が不正です" );

  int pos = 1;

  while( true )
  {
    const unt32 c = Tag[pos];

    ++pos;
    if( c==':' ){ break; }

    Element += c;
  }

  while( true )
  {
    const unt32 c = Tag[pos];

    ++pos;
    if( c=='>' ) { break; }

    Value += c;
  }

  Element = String::ToLower(Element);
}

}