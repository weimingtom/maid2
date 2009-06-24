/*!
 	@file 
 	@brief	ファイル操作を行う関数集
 */

#ifndef maid2_storage_fileio_fileoperation_h
#define maid2_storage_fileio_fileoperation_h



#include"../../config/define.h"
#include"../../auxiliary/string.h"
#include"ifileread.h"

#include<vector>

#undef CreateDirectory

namespace Maid
{
  namespace FileOperation
  {
    bool IsFileExist( const String& FileName );
    void CreateDirectory( const String& DirectoryPath );
    void CreateDirectory( const String& DirectoryPath, std::vector<String>& CreateList );

    std::vector<unt08>  CalcMD5( IFileRead& hFile );

  }
}

#endif