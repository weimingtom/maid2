/*!	
	@file
	@brief	読み込みデータをストレージクラスに中継する
*/

#ifndef maid2_storage_filereader_h
#define maid2_storage_filereader_h


#include"../config/define.h"
#include"../config/typedef.h"
#include"../auxiliary/globalpointer.h"
#include"string.h"

#include"storage.h"

namespace Maid
{
  class FileReader : public GlobalPointer<Storage>
  {
  public:
    FileReader();
    virtual ~FileReader();

    void Open( const String& filename );
    void Read( void* pData, size_t size );

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
        STATUS_READING,   //!<  読み込み中
      };

      STATUS  Status;
      size_t  Size;     //  ファイルサイズ
      size_t  Position; //  ファイルポジション
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