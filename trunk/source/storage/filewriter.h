/*!	
	@file
	@brief	書き込むデータをストレージクラスに中継する
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

    struct INFO
    {
      enum STATUS
      {
        STATUS_SLEEPING,  //!<  なにもしてない
        STATUS_WRITING,   //!<  書き込み中
      };

      STATUS  Status;

      //  そのうち書き込み状況を取得できるメンバを加えるかも

    };

    const INFO& GetStatus()const;

  private:
    void CreateObject();
    void SendMessage( const SPSTORAGEMESSAGE& pMess );

  private:
    INFO            m_Info;
    SPSTORAGEOBJECT m_pObject;
  };
}

#endif