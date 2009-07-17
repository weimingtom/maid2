#ifndef maid2_storage_packfileinfo_h
#define maid2_storage_packfileinfo_h


#include"../config/define.h"
#include"../config/typedef.h"
#include"../auxiliary/macro.h"
#include"../auxiliary/string.h"

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
		  unt64	ArchiveOffset;	//!<	アーカイブデータがあるファイルの先頭からのオフセット
		  COMPRESSTYPE CompressType;		//!<	アーカイブ情報が入っているデータの圧縮形式

      int DiveCount;        //  アーカイバが分割されている個数
    };

    struct FILEINFO
    {
	    unt64 Position;     //!<	指定ファイルの位置（アーカイブデータの先頭からのオフセット）
	    COMPRESSTYPE CompressType;		//!<	圧縮形式
	    unt32 CompressSize;	//!<	アーカイブ内のファイルサイズ
	    unt32 FileSize;     //!<	実際のファイルサイズ
    };

    struct ALIASINFO
    {
      wchar_t Target[260];  //!<  リンク先
    };

    struct ELEMENTHEADER
    {
		  unt32     StructSize;		//!<	この構造体の大きさ（バイト単位）

      enum TYPE
      {
        TYPE_FILE,  //!< 普通のファイル
        TYPE_ALIAS, //!< TYPE_FILE へのショートカット
      };

      TYPE  Type;

      union
      {
        FILEINFO  File;
        ALIASINFO Alias;
      }Info;
    };
    
    //  ELEMENTHEADER の後に wchar_t が並んでいる。すべて小文字になっています

    inline String MakeArchiveFileName( const String& base, int no )
    {
      //  一個目はそのまま
      if( no==0 ) { return base; }

      //  拡張子を a01 a02 a03 という風につけていく
      const String ext = String::GetExtension(base);

      return base.substr( 0,base.length()-ext.length() )+String::PrintFormat("a%02d",no);
    }

  }
}
#endif
