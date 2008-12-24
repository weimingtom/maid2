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
bool  Bitmap::Check( const std::vector<unt08>& FileImage )
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
void   Bitmap::Load( const std::vector<unt08>& FileImage, Surface& surface )
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
        return ;
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

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 読み込んであるサーフェスをビットマップとして保存する
/*!
 */
/*
void CBitmapLoader::Save( const mstring& FileName, const CTextureBufferMemory& src )
{
	ISurfaceBuffer& surf = const_cast<CTextureBufferMemory&>(src).GetSurface( 0 );

	Save( FileName, surf );

}

void   CBitmapLoader::Save( const mstring& FileName, ISurfaceBuffer& surf )
{
	SPSURFACEBUFFERINFO pInfo;

	surf.Lock( pInfo );

	const SIZE2DI ImageSize(pInfo->GetWidth(),pInfo->GetHeight());
	const PIXELFORMAT fmt = pInfo->GetPixelFormat();

	const int ImageWidthByte = ImageSize.w * GetPixelBPP(fmt) / 8;
	const int FilePitch = (ImageWidthByte+3)&(~3);
	const int PlaneByte   = FilePitch * ImageSize.h;
	const int PaletteByte = GetCLUTBPP(fmt) / 8 * GetCLUTLength(fmt);


	BITMAPFILEHEADER	BmpFile = {0};
	BITMAPINFOHEADER	BmpInfo = {0};

	BmpFile.bfType = 0x4D42;
	BmpFile.bfSize = sizeof(BmpFile)+sizeof(BmpInfo)+PaletteByte + PlaneByte;
	BmpFile.bfOffBits = sizeof(BmpFile)+sizeof(BmpInfo)+PaletteByte;

	BmpInfo.biSize  = sizeof(BmpInfo);
	BmpInfo.biWidth = ImageSize.w;
	BmpInfo.biHeight = ImageSize.h;
	BmpInfo.biPlanes = 1;
	BmpInfo.biBitCount = GetPixelBPP(fmt);
	BmpInfo.biCompression = BI_RGB;

	CFileWrite hFile;

	hFile.Open( FileName, CFileWrite::OPENOPTION_NEW );
	hFile.Write( &BmpFile, sizeof(BmpFile) );
	hFile.Write( &BmpInfo, sizeof(BmpInfo) );

	if( PaletteByte!=0 )
	{
		hFile.Write( pInfo->GetCLUT(), PaletteByte );
	}

	MySTL::vector<unt08> tmp( FilePitch - ImageWidthByte, 0x00 );


	//	ビットマップは上下逆
	for( int y=0; y<ImageSize.h; ++y )
	{
		hFile.Write( pInfo->GetLinePtr((ImageSize.h-1)-y), ImageWidthByte );

		if( !tmp.empty() )
		{
			hFile.Write( &(tmp[0]), (int)tmp.size() );
		}
	}

}
*/
  }
}