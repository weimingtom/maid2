/*!	
	@file
	@brief	統合ファイルを読み出すクラス
*/

#ifndef maid2_storage_packfilereader_h
#define maid2_storage_packfilereader_h


#include"../config/define.h"
#include"../config/typedef.h"
#include"../auxiliary/buffer.h"
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

    SPFILEREAD FindFile( const String& FileName );

  private:

    struct ARCHIVEDATA
    {
      String  FileName;
      PACKFILE::HEADER  Header;
      Buffer  ElementBuffer;  //  ELEMENTINFO のバッファが入っている実態

      std::vector<unt64>  ArchiveSize;  //  分割されているファイルのそれぞれの大きさ
      typedef std::map<String,const PACKFILE::ELEMENTINFO*> ELEMENTINFOLIST;
      ELEMENTINFOLIST  Data;
    };

    typedef std::list<ARCHIVEDATA> ARCHIVELIST;
    ARCHIVELIST  ArchiveList;
  };
}

#endif