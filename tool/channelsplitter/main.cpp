#include"../../source/config/define.h"
#include"../../source/framework/win32/shell.h"
#include"../../source/storage/packfilecreater.h"
#include"../../source/auxiliary/debug/warning.h"
#include"../../source/graphics/imagefile.h"

#include"../../source/storage/fileio/filereadnormal.h"
#include"../../source/storage/fileio/filewrite.h"
#include"../../source/storage/fileio/fileoperation.h"



using namespace Maid;


int main( int argc, char *argv[] )
{
  //  引数で指定されたファイルを抜き出す
  if( argc<2 ) { printf("引数がありません"); return 0; }

  {
    //  これはおまじないです。
    String::Initialize();
  }

  {
    const String cdir = String::GetDirectory(Shell::GetExeFileName());
    Shell::SetCurrentDirectory(cdir);
  }

  const String SourceFileName = String::ConvertSJIStoMAID(argv[1]);
  std::vector<unt08>  FileImage;

  {
    FileReadNormal  hFile( SourceFileName );
    {
      const IFileRead::OPENRESULT ret = hFile.Open();
      if( ret==IFileRead::OPENRESULT_ERROR ) { printf( "オープンに失敗" ); return 0; }
    }
  
    FileImage.resize( hFile.GetSize() );
    {
      const size_t ret = hFile.Read( &(FileImage[0]), FileImage.size() );
      if( ret!=FileImage.size() ) { printf( "読み込みに失敗" ); return 0; }
    }
  }

  if( !PNG::Check(FileImage) )
  {
    printf("PNGファイルではありません");
    return 0;
  }

  SurfaceInstance SourceSurface;

  {
    const FUNCTIONRESULT ret = PNG::Load(FileImage,SourceSurface);
    if( ret==FUNCTIONRESULT_ERROR ) { printf( "オープンに失敗" ); return 0; }
  }

  SurfaceInstance ColorSurface;
  SurfaceInstance AlphaSurface;

  {
    const SIZE2DI size = SourceSurface.GetSize();

    ColorSurface.Create( size, PIXELFORMAT_R08G08B08I );
    AlphaSurface.Create( size, PIXELFORMAT_P08X08R08G08B08I );

    for( int i=0; i<256; ++i )
    {
      const float v = BITCONVERT08Ito32F(i);
      COLOR_R32G32B32A32F col(v,v,v,v);
      AlphaSurface.SetIndex( i, col );
    }

    for( int y=0; y<size.h; ++y )
    {
      for( int x=0; x<size.w; ++x )
      {
        const POINT2DI pos(x,y);
        const COLOR_R32G32B32A32F col = SourceSurface.GetPixel(pos);
        const COLOR_R32G32B32A32F alpha(col.GetA(),col.GetA(),col.GetA(),col.GetA());

        ColorSurface.SetPixel( pos, col );
        AlphaSurface.SetPixel( pos, alpha );
      }
    }
  }

  const String base = SourceFileName.substr(0,SourceFileName.length()-(String::GetExtension(SourceFileName).length()+1));
  const String ColorFileName = base + MAIDTEXT(".bmp");
  const String AlphaFileName = base + MAIDTEXT("_alpha.bmp");

  {
    std::vector<unt08> FileImage;
    Bitmap::Save( ColorSurface, FileImage );

    FileWrite hFile;

    hFile.Open( ColorFileName, FileWrite::OPENOPTION_NEW );
    hFile.Write( &(FileImage[0]), FileImage.size() );
  }

  {
    std::vector<unt08> FileImage;
    Bitmap::Save( AlphaSurface, FileImage );

    FileWrite hFile;

    hFile.Open( AlphaFileName, FileWrite::OPENOPTION_NEW );
    hFile.Write( &(FileImage[0]), FileImage.size() );
  }

  return 0;
}