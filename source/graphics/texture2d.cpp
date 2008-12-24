#include"texture2d.h"
#include"imagefile.h"
#include"../storage/filereadersync.h"
#include"../auxiliary/thread.h"
#include"../auxiliary/debug/warning.h"

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

      if( Bitmap::Check( image ) ) { Bitmap::Load( image, src ); }

      if( src.IsEmpty() )
      {
        MAID_WARNING( MAIDTEXT("展開に失敗") << in.FileName );
        return ; 
      }
    }

    //  これで src に読み込んだデータが入った。
    //  次はハードウェアで使えるピクセルフォーマットに変換する



/*
    virtual SPTEXTURE2D CreateTexture2D( const CREATERETEXTURE2DPARAM& param, const SUBRESOURCE* data )=0;
  struct CREATERETEXTURE2DPARAM
  {
    SIZE2DI Size;
    int MipLevels;
    int ArraySize;      //  何枚あるか？
    PIXELFORMAT Format; //  ピクセルフォーマット
    SAMPLEDESC  Sample;
    RESOURCEUSAGE Usage;
    unt   BindFlags;
    unt   CPUAccessFlags;
    unt   MiscFlags;

    CREATERETEXTURE2DPARAM()
      :Size(0,0)
      ,MipLevels(0)
      ,ArraySize(0)
      ,Format(PIXELFORMAT_NONE)
      ,Usage(RESOURCEUSAGE_DEFAULT)
      ,BindFlags(0)
      ,CPUAccessFlags(0)
      ,MiscFlags(0)
    {}
  };
*/

  }

  }


Texture2D::CACHE::INFOMAP  Texture2D::CACHE::s_InfoMap;

void Texture2D::LoadFile( const String& FileName )
{
  ITexture2D::Clear();
  m_Cache.Start( KEEPOUT::tex2dInput(FileName, *(GlobalPointer<Graphics::Core>::Get()) ) );
}

bool Texture2D::IsCompiling()const
{
  //  ポインタが存在してるならすでに作ってる
  if( ITexture2D::Get().get()!=NULL ) { return false; }

  //  処理が終わるまでは忙しい
  if( m_Cache.IsExecuting() ) { return true; }

  const_cast<Texture2D*>(this)->ITexture2D::Set( m_Cache.GetOutput().pTexture );
  return false;

}

}