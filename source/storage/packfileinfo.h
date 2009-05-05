#ifndef maid2_storage_packfileinfo_h
#define maid2_storage_packfileinfo_h


#include"../config/define.h"
#include"../config/typedef.h"

namespace Maid
{
  namespace PACKFILE
  {

	  enum COMPRESSTYPE
	  {
		  COMPRESSTYPE_NONE=0,	//!<	無圧縮
		  COMPRESSTYPE_ZLIB,		//!<	ZLib での圧縮
	  };

    struct HEADER
    {
		  unt32	FileCode;	//!<	'MPF2'

		  enum VERSION
		  {
			  VERSION_0x0100=0x0100,
		  };

		  VERSION	Version;		//!<	ファイルバージョン

		  unt32	InfoSizeComp;	//!<	アーカイブ情報が入っているデータの長さ（圧縮中のサイズ）
		  unt32	InfoSize;     //!<	アーカイブ情報が入っているデータの長さ（展開後のサイズ）
		  COMPRESSTYPE CompressType;		//!<	圧縮形式
		  unt32	ArchiveOffset;	//!<	アーカイブデータがあるファイルの先頭からのオフセット

      int DiveCount;        //  アーカイバが分割されている個数
    };

    struct INFO
    {
		  unt32 StructSize;		//!<	この構造体の大きさ（バイト単位）
		  unt64 Position;     //!<	指定ファイルの位置（アーカイブデータの先頭からのオフセット）
		  unt32 CompressSize;	//!<	アーカイブ内のファイルサイズ
		  unt32 FileSize;     //!<	実際のファイルサイズ

		  COMPRESSTYPE CompressType;		//!<	圧縮形式

      wchar_t FileName[1];   //!<	このファイル名( StructSize - sizeof(FILEINFO) 分ある）すべて小文字になっています
    };

  }

}
#endif
