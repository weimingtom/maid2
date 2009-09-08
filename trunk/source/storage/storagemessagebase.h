#ifndef maid2_storage_storagemessagebase_h
#define maid2_storage_storagemessagebase_h

#include"../config/define.h"
#include"../config/typedef.h"

#include<vector>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class Storage;
  class IStorageObject
  {
    friend Storage;
  public:
    IStorageObject() : m_MessageCount(0),m_Position(0),m_Size(0),m_IsError(false){}
    virtual ~IStorageObject(){}

    int     GetMessageCount() const{ return m_MessageCount; }
    size_t  GetPosition() const { return m_Position; }
    size_t  GetSize() const { return m_Size; }
    bool    IsError() const { return m_IsError; }

  private:
    int    m_MessageCount;
    size_t m_Position;
    size_t m_Size;
    bool   m_IsError;
  };

  typedef boost::shared_ptr<IStorageObject> SPSTORAGEOBJECT;


  //  描画スレッドに投げる　メッセージクラス
  namespace StorageMessage
  {
    struct Base
    {
      enum MESSAGE
      {
        NONE,           //!<  何もしない

        FILE_W_CREATE,  //!<  ファイルの作成＆オープン
        FILE_W_OPEN,    //!<  ファイルのオープン
        FILE_W_ADD,     //!<  ファイルの追記オープン
        FILE_W_WRITE,   //!<  ファイルの書き込み
        FILE_W_SEEK,    //!<  ファイルポジションの移動
        FILE_W_CLOSE,   //!<  ファイルを閉じる

        FILE_R_OPEN,    //!<  ファイルのオープン
        FILE_R_READ,    //!<  ファイルの読み込み
        FILE_R_SEEK,    //!<  ファイルポジションの移動
        FILE_R_CLOSE,   //!<  ファイルを閉じる

        FILE_R_OPENREAD,//!<  ファイルのオープン&全読み込み

        FILE_MOUNT,     //!<  アーカイブを乗っける
        FILE_UNMOUNT,   //!<  アーカイブをはずす
      };

      Base( MESSAGE mess ) : Message(mess){}
      const MESSAGE	Message;
      SPSTORAGEOBJECT pObject;
    };
  }

  typedef boost::shared_ptr<StorageMessage::Base> SPSTORAGEMESSAGE;
  typedef std::vector<SPSTORAGEMESSAGE>           STORAGEMESSAGEQUE;
}



#endif
