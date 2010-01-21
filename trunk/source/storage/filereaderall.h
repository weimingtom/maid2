/*!	
	@file
	@brief	非同期ファイル一括読み込みクラス
*/

#ifndef maid2_storage_filereaderall_h
#define maid2_storage_filereaderall_h


#include"../config/define.h"
#include"../config/typedef.h"
#include"../auxiliary/globalpointer.h"
#include"../auxiliary/string.h"
#include"../auxiliary/memorybuffer.h"

#include"storage.h"

namespace Maid
{
  class FileReaderAll
    : public GlobalPointer<Storage>
  {
  public:
    FileReaderAll();
    virtual ~FileReaderAll();

    void Open( const String& filename ); 
    void Close();
    bool    IsEmpty() const;
    bool    IsExecuting() const;
    bool    IsError() const;
    size_t  GetSize() const;
    bool Sync();

    const MemoryBuffer& GetBuffer() const;
    const String& GetFileName() const;

  private:
    SPMEMORYBUFFER  m_pBuffer;
    SPSTORAGEOBJECT m_pObject;
    String          m_FileName;
  };
}

#endif