/*!	
	@file
	@brief	同期ファイル読み込みクラス
*/

#ifndef maid2_storage_filereadersync_h
#define maid2_storage_filereadersync_h


#include"../config/define.h"
#include"../config/typedef.h"

#include"filereader.h"

#include"../auxiliary/string.h"

namespace Maid
{
  class FileReaderSync
  {
  public:
    bool Open( const String& filename );
    size_t Read( void* pData, size_t size );

    enum POSITION
    {
      POSITION_BEGIN,   //!<	ファイルの先頭から
      POSITION_END,     //!<	ファイルの終端から
      POSITION_CURRENT, //!<	現在の位置から
    };

    bool Seek( POSITION ePos, int Size );

    bool Close();

    size_t  GetSize() const;
    size_t  GetPosition() const;

  private:
    bool Sync();

  private:
    FileReader m_Reader;
  };
}

#endif