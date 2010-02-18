/*!	
	@file
	@brief	非同期ファイル書き込みクラス
*/

#ifndef maid2_storage_filewriter_h
#define maid2_storage_filewriter_h


#include"../config/define.h"
#include"../config/typedef.h"
#include"../auxiliary/globalpointer.h"
#include"string.h"

#include"storage.h"

namespace Maid
{
  class FileWriter : public GlobalPointer<Storage>
  {
  public:
    FileWriter();
    virtual ~FileWriter();

    void Create( const String& filename );
    void Open( const String& filename );
    void OpenAdd( const String& filename );
    void Write( const void* pData, size_t size );

    enum POSITION
    {
      POSITION_BEGIN,   //!<	ファイルの先頭から
      POSITION_END,     //!<	ファイルの終端から
      POSITION_CURRENT, //!<	現在の位置から
    };

    void Seek( POSITION ePos, int Size );

    void Close();
    void Close( bool IsSync );


    bool    IsEmpty() const;
    bool    IsExecuting() const;
    bool    IsError() const;
    size_t  GetSize() const;
    size_t  GetPosition() const;

  private:
    void CreateObject();
    void SendMessage( const SPSTORAGEMESSAGE& pMess );

  private:
    SPSTORAGEOBJECT m_pObject;
  };
}

#endif