#include"texture2d.h"
#include"imagefile.h"
#include"../storage/filereadersync.h"
#include"../auxiliary/thread.h"
#include"../auxiliary/debug/warning.h"

#include"surface.h"
#include"transiter.h"

namespace Maid
{
  namespace KEEPOUT
  {
  void tex2dFunction::Execute( const IJobInput& Input, IJobOutput& Output )
  {
    const tex2dInput&  in  = static_cast<const tex2dInput&>(Input);
    tex2dOutput& out = static_cast<tex2dOutput&>(Output);

    Surface src;

    {
      FileReaderSync  hFile;

      {
        const bool ret = hFile.Open( in.FileName );
        if( ret ) { MAID_WARNING( MAIDTEXT("オープンに失敗") << in.FileName ); return ; }
      }

      std::vector<unt08> image( hFile.GetSize() );

      {
        const bool ret = hFile.Read( &(image[0]), image.size() );
        if( ret ) { MAID_WARNING( MAIDTEXT("読み込みに失敗") << in.FileName ); return ; }
      }

      { //  ファイルフォーマットを調べて読み込み開始
        if( Bitmap::Check( image ) ) { Bitmap::Load( image, src ); }
         #pragma  COMPILERMSG( "TODO:のこり jpeg,png,psd,gif,tga...ぐらいか？" )
      }

      if( src.IsEmpty() ) { MAID_WARNING( MAIDTEXT("展開に失敗") << in.FileName ); return ; 
      }
    }

    //  これで src に読み込んだデータが入った。
    //  次はハードウェアで使えるフォーマットに変換する

    const PIXELFORMAT DstFormat = in.Core->FindFormatTexture2D( src.GetPixelFormat(), false );
    if( DstFormat==PIXELFORMAT_NONE )
    {
      MAID_WARNING( MAIDTEXT("このフォーマットは対応していません ") << src.GetPixelFormat() << in.FileName );
      return ; 
    }

    const SIZE2DI TexSize= in.Core->CalcTextureSize( src.GetSize() );
 
    Surface dst;
    dst.Create( TexSize, DstFormat );
    Transiter::Fill( COLOR_R32G32B32A32F(0,0,0,0), dst );

    #pragma  COMPILERMSG( "TODO:当面は拡大縮小しないけど、そのうち書く。REGZAの超解像度技術とか使えたら面白い" )

    for( int y=0; y<src.GetSize().h; ++y )
    {
      for( int x=0; x<src.GetSize().w; ++x )
      {
        dst.SetPixel( POINT2DI(x,y), src.GetPixel( POINT2DI(x,y) ) );
      }
    }

    //  これで作るテクスチャが完成
    //  あとはデバイスにやってもらう
    Graphics::SPTEXTURE2D pTexture;

    {
      Graphics::CREATERETEXTURE2DPARAM param;

      param.Size = TexSize;
      param.MipLevels = 0;
      param.ArraySize = 0;
      param.Format = DstFormat;
      param.Usage = Graphics::RESOURCEUSAGE_IMMUTABLE;
      param.BindFlags = Graphics::RESOURCEBINDFLAG_MATERIAL;
      param.MiscFlags = Graphics::RESOURCEMISCFLAG_GENERATE_MIPS;

      Graphics::SUBRESOURCE Data;
      Data.pData = dst.GetLinePTR(0);
      Data.Pitch = dst.GetPitch();
      pTexture = in.Core->GetDevice()->CreateTexture2D( param, &Data );
    }

    if( pTexture.get()==NULL )
    {
        MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") << in.FileName );
        return ; 
    }

    Graphics::CREATEMATERIALPARAM param;
    param.Format = DstFormat;
    param.Dimension =  Graphics::CREATEMATERIALPARAM::DIMENSION_TEXTURE2D;

    out.pMaterial = in.Core->GetDevice()->CreateMaterial( pTexture, param );
    out.ImageSize   = src.GetSize();
    out.CreateSize  = out.ImageSize;
    out.TextureSize = TexSize;
  }

}


Texture2D::CACHE::INFOMAP  Texture2D::CACHE::s_InfoMap;

void Texture2D::LoadFile( const String& FileName )
{
  IMaterial::Clear();
  m_Cache.Start( KEEPOUT::tex2dInput(FileName, GlobalPointer<GraphicsCore>::Get() ) );
}

const SIZE2DI& Texture2D::GetImageSize() const
{
  MAID_ASSERT( IsLoading(), "処理中" );
  return m_Cache.GetOutput().ImageSize;
}

const SIZE2DI& Texture2D::GetCreateSize() const
{
  MAID_ASSERT( IsLoading(), "処理中" );
  return m_Cache.GetOutput().CreateSize;
}

const SIZE2DI& Texture2D::GetTextureSize() const
{
  MAID_ASSERT( IsLoading(), "処理中" );
  return m_Cache.GetOutput().TextureSize;
}


bool Texture2D::IsLoading()const
{
  //  ポインタが存在してるならすでに作ってる
  if( IMaterial::Get().get()!=NULL ) { return false; }

  //  処理が終わるまでは忙しい
  if( m_Cache.IsExecuting() ) { return true; }

  const_cast<Texture2D*>(this)->IMaterial::Set( m_Cache.GetOutput().pMaterial );
  return false;

}

}