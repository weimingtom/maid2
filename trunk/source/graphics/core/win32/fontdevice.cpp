#include"fontdevice.h"
#include"../../../config/win32.h"
#include"../../../auxiliary/debug/warning.h"

#include"../../transiter.h"

//  フォントの列挙
// http://msdn.microsoft.com/ja-jp/library/cc428521.aspx


namespace Maid { namespace Graphics {


static int CALLBACK EnumFontFamExProc(
      CONST LOGFONT* lpelfe ,
			CONST TEXTMETRIC* lpntme ,
			DWORD FontType, LPARAM lParam)
{
  std::vector<IFontDevice::FONTINFO>* pInfoList = (std::vector<IFontDevice::FONTINFO>*)lParam;

  //  縦書きはスルー
  if( lpelfe->lfFaceName[0]=='@' ) { return TRUE; }

  IFontDevice::FONTINFO info;
  info.Name = String::ConvertUNICODEtoMAID(lpelfe->lfFaceName);

  pInfoList->push_back( info );
  return TRUE;
}

void FontDevice::Initialize()
{
  //  なにもしない。
}


void FontDevice::GetFontList( std::vector<FONTINFO>& List )
{
  MAID_ASSERT( !List.empty(), "empty()ではありません" );

  LOGFONT logfont;
  logfont.lfCharSet = DEFAULT_CHARSET;

  EnumFontFamiliesEx( NULL, &logfont, EnumFontFamExProc, (LPARAM)&List, 0 );
}


class FontWindows : public IFont
{
public:
  FontWindows( const String& name, const SIZE2DI& size, HFONT font )
    :IFont(name,size),m_Font(font){}

  ~FontWindows()
  {
    DeleteObject(m_Font);
  }

  HFONT GetHFONT() const { return m_Font; }

private:
  HFONT m_Font;
};


SPFONT FontDevice::CreateFont( const String& name, const SIZE2DI& size )
{
  const std::wstring unicodename = String::ConvertMAIDtoUNICODE(name);

	const HFONT	  hFont = ::CreateFontW( 
			size.h,     // フォントの高さ（論理単位）０ならデフォルト 
			size.w,			// 幅。０なら、高さにあった幅 
			0,          // スクリーンの表示画面とＸ軸の角度の１０倍
			0,
			FW_REGULAR,		// 太さ 
			FALSE,	      //イタリック体
			FALSE,				//アンダーライン
			FALSE,				//打ち消し線
			DEFAULT_CHARSET,			//文字セット
			OUT_DEFAULT_PRECIS,		//出力精度
			CLIP_DEFAULT_PRECIS,	//クリッピング精度
			NONANTIALIASED_QUALITY,				//出力品質
			FIXED_PITCH | FF_MODERN,	//ピッチとファミリー
			unicodename.c_str()
      );

  return SPFONT( new FontWindows(name,size,hFont) );
}

String FontDevice::GetDefaultFontName()const
{
  const String name = MAIDTEXT("ＭＳ ゴシック");
  return name;
}

void FontDevice::Rasterize( const SPFONT& pFont, unt32 FontCode, const COLOR_R32G32B32A32F& Color, SurfaceInstance& Dst )
{
  //  HBITMAP にテキストを打って、それの色からフォントを作成する

  // GetGlyphOutline を使う方法でもいいかもしれないけど
  // たまに矩形より大きいデータがかえってくることあるんだよね・・・

  SIZE2DI FontSize = pFont->GetSize();

  if( !String::IsHankaku(FontCode) ) { FontSize.w *= 2; }

  HBITMAP hBitmap = NULL;
  void*   pPlane  = NULL;
  const int bpp = 32;
  const int pitch = (FontSize.w*bpp/8+3)&~3;
  const int dib_w = pitch / (bpp/8);

  {
	  BITMAPINFOHEADER dib;

	  ZERO( &dib, sizeof(dib) );
	  dib.biSize = sizeof(dib);
	  dib.biWidth = dib_w;
	  dib.biHeight = -(int)FontSize.h;
	  dib.biPlanes = 1;
	  dib.biBitCount = 32;

	  HDC hDC = ::CreateCompatibleDC(NULL);

	  if( hDC==NULL ) { MAID_WARNING( MAIDTEXT("hDC==NULL") ); return ; }
	  hBitmap = CreateDIBSection( hDC, (BITMAPINFO *)&dib, DIB_RGB_COLORS, (void **)&pPlane, NULL, 0 );
	  ::DeleteDC(hDC);
	  if( hBitmap==NULL ) { MAID_WARNING( MAIDTEXT("DIB確保に失敗") );  return ; }

    //  とりあえず真っ黒にしておく
    unt08* p = (unt08*)pPlane;
    for( int i=0; i<FontSize.h; ++i ) 
    {
      ::memset( p, 0, pitch );
      p += pitch;
    }
  }

  {
    const HFONT   hFont = static_cast<FontWindows*>(pFont.get())->GetHFONT();
	  const HDC     hDC	       = ::CreateCompatibleDC(NULL);
	  const HFONT   hOldFont   = (HFONT)::SelectObject( hDC, hFont );
	  const HBITMAP hOldBitmap = (HBITMAP)::SelectObject( hDC, hBitmap );
	  //	↓の設定をすることで、ドットの打たれたピクセルが 0x00000001 になる
	  ::SetTextColor( hDC, 0x00010000 );
	  ::SetBkColor  ( hDC, RGB(0,0,0) );
	  ::SetBkMode   ( hDC, OPAQUE);

	  {
      String tmp;
      tmp += FontCode;
		  const std::wstring str = String::ConvertMAIDtoUNICODE( tmp );

		  ::TextOut( hDC, 0, 0, str.c_str(), (int)str.length() );
	  }
	  ::SelectObject( hDC, hOldBitmap );
	  ::SelectObject( hDC, hOldFont );
	  ::DeleteDC( hDC );
  }

  //  0x00000001 になっているピクセルが打たれたピクセルなので
  //  それを見つけたら 白くする
  Dst.Create( FontSize, PIXELFORMAT_A08R08G08B08I );
  Transiter::Fill( COLOR_R32G32B32A32F(1,1,1,0), Dst );

  for( int y=0; y<FontSize.h; ++y )
  {
    const unt32* pLine = (unt32*)(((unt08*)pPlane) + pitch*y);

    for( int x=0; x<FontSize.w; ++x )
    {
      const unt32 col = pLine[x];

      if( col!=0 ) { Dst.SetPixel(POINT2DI(x,y),Color); }
    }
  }

  ::DeleteObject( hBitmap );
}



}}


