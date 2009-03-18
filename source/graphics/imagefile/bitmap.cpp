#include"bitmap.h"

#include"../../config/win32.h"
#include"../../auxiliary/debug/warning.h"


namespace Maid
{
  namespace Bitmap
  {

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定したデータがビットマップか調べる
/*!
    @param	FileImage	[i ]  データ

    @return ビットマップならtrue
            違うならfalse
*/
bool  Check( const std::vector<unt08>& FileImage )
{
  if( FileImage.size()<2 ) { return false; }

	return FileImage[0]=='B' &&  FileImage[1]=='M';
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ビットマップを変換する
/*!
    @param	FileImage	[i ]  ファイル名
    @param	dst       [i ]  転送先
 */
FUCTIONRESULT   Load( const std::vector<unt08>& FileImage, Surface& surface )
{
  MAID_ASSERT( !Check(FileImage), "ビットマップではありません" );

  const unt08* pCurrent = &(FileImage[0]);


	{
		//　ビットマップ形式のデータかチェック
	  BITMAPFILEHEADER*	pBmpFile = (BITMAPFILEHEADER*)pCurrent;
    if( pBmpFile->bfType!=0x4D42 ) { MAID_WARNING( MAIDTEXT("BITMAPFILEHEADER::bfType!=BM") ); }

    pCurrent += sizeof(BITMAPFILEHEADER);
	}


	{
		//　各種情報を収集する
	  BITMAPINFOHEADER*	pBmpInfo = (BITMAPINFOHEADER*)pCurrent;

	  SIZE2DI		PlaneSize;
	  PIXELFORMAT Format;
	  int			Pitch;
	  int			CLUTSize=0;

		PlaneSize.w = pBmpInfo->biWidth;
		PlaneSize.h = pBmpInfo->biHeight;

		switch( pBmpInfo->biBitCount )
		{
		case  8: { Format = PIXELFORMAT_P08X08R08G08B08I;	}break;
		case 24: { Format = PIXELFORMAT_R08G08B08I;			  }break;
		case 32: { Format = PIXELFORMAT_A08R08G08B08I;		}break;
		default: 
      {
        MAID_WARNING( MAIDTEXT("識別できないピクセルフォーマットです ") << pBmpInfo->biBitCount );
        return FUCTIONRESULT_ERROR;
      }break;
		}
		
		Pitch = pBmpInfo->biWidth * pBmpInfo->biBitCount/8;
		Pitch = (Pitch +3)&~3;					//	ビットマップのピッチは４の倍数である

    //	転送先の作成
    surface.Create( PlaneSize, Format, Pitch );
    pCurrent += sizeof(BITMAPINFOHEADER);
  }



	{	//	順番に転送していく。
		//	ビットマップファイルはパレット、ピクセルデータの順に並んでいるので
		//	そうやって読んでいく
    const int clutsize = GetCLUTbyte(surface.GetPixelFormat());

    if( clutsize!=0 )
    { //  まずパレット
      memcpy( surface.GetColorPTR(0), pCurrent, clutsize );
      pCurrent += clutsize;
    }

		//	ビットマップは上下反転してるので、それに気をつけながらコピーする
    const int height= surface.GetSize().h;
    const int pitch = surface.GetPitch();

		for( int y=0; y<height; ++y )
		{
      const int pos = (height-(y+1));
      memcpy( surface.GetLinePTR(pos), pCurrent, pitch );
      pCurrent += pitch;
		}
	}

  return FUCTIONRESULT_OK;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 読み込んであるサーフェスをビットマップとして保存する
/*!
 */
FUCTIONRESULT Save( const Surface& src, std::vector<unt08>& FileImage )
{
	const SIZE2DI ImageSize = src.GetSize();
	const PIXELFORMAT fmt = src.GetPixelFormat();

	const int ImagePitch = ImageSize.w * GetPixelBPP(fmt) / 8;
	const int FilePitch = (ImagePitch+3)&(~3);
	const int PlaneByte   = FilePitch * ImageSize.h;
	const int PaletteByte = GetCLUTbyte(fmt);


  const int filesize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + PaletteByte + PlaneByte;
  FileImage.resize( filesize );

  unt08* pCurrentPos = &(FileImage[0]);

  ZERO( pCurrentPos, filesize );

  {
	  BITMAPFILEHEADER*	pBmpFile = (BITMAPFILEHEADER*)pCurrentPos;

	  pBmpFile->bfType = 0x4D42;
	  pBmpFile->bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+PaletteByte + PlaneByte;
	  pBmpFile->bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+PaletteByte;

    pCurrentPos += sizeof(BITMAPFILEHEADER);
  }

  {
    BITMAPINFOHEADER*	pBmpInfo = (BITMAPINFOHEADER*)pCurrentPos;

	  pBmpInfo->biSize  = sizeof(BITMAPINFOHEADER);
	  pBmpInfo->biWidth = ImageSize.w;
	  pBmpInfo->biHeight = ImageSize.h;
	  pBmpInfo->biPlanes = 1;
	  pBmpInfo->biBitCount = GetPixelBPP(fmt);
	  pBmpInfo->biCompression = BI_RGB;

    pCurrentPos += sizeof(BITMAPINFOHEADER);
  }

	if( PaletteByte!=0 )
	{
    ::memcpy( pCurrentPos, src.GetCLUTPTR(), PaletteByte );
    pCurrentPos += PaletteByte;
  }

	//	ビットマップは上下逆
	for( int y=0; y<ImageSize.h; ++y )
	{
    ::memcpy( pCurrentPos, src.GetLinePTR((ImageSize.h-1)-y), ImagePitch );
    pCurrentPos += FilePitch;
	}

  return FUCTIONRESULT_OK;
}

  }
}