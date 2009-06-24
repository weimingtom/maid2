/*!	
	@file
	@brief	統合ファイルを読み出すクラス
*/

#ifndef maid2_storage_packfilereader_h
#define maid2_storage_packfilereader_h


#include"../config/define.h"
#include"../config/typedef.h"
#include"fileio/ifileread.h"
#include"packfileinfo.h"

#include<vector>
#include<list>
#include<map>

namespace Maid
{
  class PackFileReader
  {
  public:

    void Mount( const String& FileName );
    void UnMount( const String& FileName );

    SPFILEREAD FindFile( const String& FileName )const;

    bool IsExist( const String& FileName )const;

  private:

    struct ARCHIVEDATA
    {
      String  FileName;
      PACKFILE::HEADER  Header;

      std::vector<unt64>  ArchiveSize;  //  分割されているファイルのそれぞれの大きさ
      typedef std::map<String,PACKFILE::FILEINFO> FILEINFOLIST;
      FILEINFOLIST  Data;
    };

    typedef std::list<ARCHIVEDATA> ARCHIVELIST;
    ARCHIVELIST  ArchiveList;
  };
}

#endif