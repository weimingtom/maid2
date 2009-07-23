#include"texture2d.h"
#include"imagefile.h"
#include"../storage/filereadersync.h"
#include"../auxiliary/thread.h"
#include"../auxiliary/debug/warning.h"
#include"../storage/filewriter.h"

#include"surface.h"
#include"transiter.h"


namespace Maid
{
  void ReadName( const String& Tag, String& Element, String& Value )
  {
    MAID_ASSERT( Tag[0]!='<', "解析するファイル名が不正です" );

    int pos = 1;

    while( true )
    {
      const unt32 c = Tag[pos];

      ++pos;
      if( c==':' )
      {
        break;
      }

      Element += c;
    }

    while( true )
    {
      const unt32 c = Tag[pos];

      ++pos;
      if( c=='>' )
      {
        break;
      }

      Value += c;
    }

    Element = String::ToLower(Element);
  }


  FUNCTIONRESULT LoadImageFile( const String& filename, std::vector<SurfaceInstance>& dst )
  {
    FileReaderSync  hFile;

    {
      const bool ret = hFile.Open( filename );
      if( ret ) { MAID_WARNING( MAIDTEXT("オープンに失敗") ); return FUNCTIONRESULT_ERROR; }
    }

    std::vector<unt08> image( hFile.GetSize() );

    {
      const size_t ret = hFile.Read( &(image[0]), image.size() );
      if( ret!=image.size() ) { MAID_WARNING( MAIDTEXT("読み込みに失敗") ); return FUNCTIONRESULT_ERROR; }
    }

    { //  ファイルフォーマットを調べて読み込み開始

      if( Bitmap::Check( image ) ) 
      {
        dst.resize(1);
        const FUNCTIONRESULT ret = Bitmap::Load( image, dst[0] ); 
        if( FUNCTIONRESULT_FAILE(ret) ) { MAID_WARNING( MAIDTEXT("bitmap失敗") ); return FUNCTIONRESULT_ERROR; }
      }
      else if( PNG::Check( image ) ) 
      {
        dst.resize(1);
        const FUNCTIONRESULT ret = PNG::Load( image, dst[0] ); 
        if( FUNCTIONRESULT_FAILE(ret) ) { MAID_WARNING( MAIDTEXT("png失敗") ); return FUNCTIONRESULT_ERROR; }
      }
      else if( Jpeg::Check( image ) ) 
      {
        dst.resize(1);
        const FUNCTIONRESULT ret = Jpeg::Load( image, dst[0] ); 
        if( FUNCTIONRESULT_FAILE(ret) ) { MAID_WARNING( MAIDTEXT("jpeg失敗") ); return FUNCTIONRESULT_ERROR; }
      }

      #pragma  COMPILERMSG( "TODO:のこり psd,gif,tga...ぐらいか？" )
    }

    return FUNCTIONRESULT_OK;
  }



  namespace KEEPOUT
  {

#define ELEMENT_COLOR MAIDTEXT("color")
#define ELEMENT_ALPHA MAIDTEXT("alpha")

    FUNCTIONRESULT tex2dFunction::LoadImage( const String& filename, std::vector<SurfaceInstance>& dst )
    {
      //  まずはファイル名を分解するところから
      //  一文字目が< で始まってるなら、合成ファイルと判断する

      std::map<String,String> Element;

      if( filename[0]!='<' ) { Element[ELEMENT_COLOR] = filename; } 
      else
      {
        unt32 begin = 0;
        unt32 len = 0;
        while( true )
        {
          if( filename.length() <= begin+len ) { break; }

          const unt32 c = filename[begin+len];
          ++len;
          if( c=='>' )
          {
            String tag = filename.substr(begin,len);
            String ele;
            String value;

            ReadName( tag, ele, value );
            Element[ele] = value;

            begin += len;
            len = 0;
          }
        }
      }

      const std::map<String,String>::const_iterator IteColor = Element.find(ELEMENT_COLOR);
      const std::map<String,String>::const_iterator IteAlpha = Element.find(ELEMENT_ALPHA);

      //  カラーとアルファが両方ある場合は
      //  カラーのビット数に合わせてアルファを合成する
      if( IteColor!=Element.end() && IteAlpha!=Element.end() )
      {
        std::vector<SurfaceInstance> col;
        std::vector<SurfaceInstance> alp;

        LoadImageFile( IteColor->second, col );
        LoadImageFile( IteAlpha->second, alp );

        MAID_ASSERT( col.size()!=alp.size(), "レイヤの数があいません" );

        const size_t count = col.size();
        dst.resize( count );

        for( size_t i=0; i<count; ++i )
        {
          SurfaceInstance& src_c = col[i];
          SurfaceInstance& src_a = alp[i];
          SurfaceInstance& d = dst[i];
          const SIZE2DI     size = src_c.GetSize();
          const PIXELFORMAT fmt  = ConvertAlphaFormat(src_c.GetPixelFormat());

          MAID_ASSERT( src_a.GetPixelFormat()!=PIXELFORMAT_P08X08R08G08B08I && src_a.GetPixelFormat()!=PIXELFORMAT_P08A08R08G08B08I,
            "アルファ画像はインデックス付でお願いします" );

          d.Create( size, fmt );

          for( int y=0; y<size.h; ++y )
          {
            for( int x=0; x<size.w; ++x )
            {
              const POINT2DI p(x,y);
                    COLOR_R32G32B32A32F pixel = src_c.GetPixel(p);
              const float alpha = BITCONVERT08Ito32F(*(unt08*)src_a.GetPixelPTR(p));
              pixel.SetA( pixel.GetA()*alpha );

              d.SetPixel( p, pixel );
            }
          }
        }
      }
      else if( IteColor!=Element.end() )
      {
        //  カラーだけの場合
        LoadImageFile( IteColor->second, dst );
      }

      return FUNCTIONRESULT_OK;
    }

  }
}