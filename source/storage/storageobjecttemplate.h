#ifndef maid2_storage_storageobjecttemplate_h
#define maid2_storage_storageobjecttemplate_h



#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/string.h"
#include"../auxiliary/globalpointer.h"

#include"storage.h"
#include"storagemessagebase.h"
#include<vector>

namespace Maid
{
  //  オブジェクトを共有するための補助クラス
  /*
    StorageObjectTemplate<共有アルゴリズム,共有データ>
  */

  template<class ID,class OPTION, class OBJECT>
  class StorageObjectTemplate : public GlobalPointer<Storage>, public OBJECT
  {
  private:
    struct STORAGEINFO
    {
      OPTION  Option;
      int     ReferenceCount;
      SPSTORAGEOBJECT pObject;
      SPSTORAGEFUNCTION pFunc;
    };

    typedef std::map<ID,STORAGEINFO> STORAGEMAP;
    static STORAGEMAP s_Storage;

    enum STATE
    {
      STATE_EMPTY,	//	まだ何もしてない
      STATE_LOADING,	//	読み込み中
      STATE_WORKING,	//	稼動中
    };

    STATE   m_State;
    ID      m_ID;

    STORAGEINFO& GetStrageInfo()
    {
      MAID_ASSERT( m_State==STATE_EMPTY, "まだ作成していない" );

      STORAGEMAP::iterator ite = s_Storage.find(m_ID);
      MAID_ASSERT( ite==s_Storage.end(), "データが見つかりません" );
      MAID_ASSERT( ite->second.ReferenceCount==0, "作成と開放の回数が一致しません" );

      return ite->second;
    }

  public:

    void Load( const ID& id )
    {
      Delete();
      _Load(ConvertID(id));
    }

    void Delete()
    {
      if( m_State==STATE_EMPTY ) { return ; }

      LocalClear();

      STORAGEINFO& info = GetStrageInfo();
      info.ReferenceCount -= 1;

      if( info.ReferenceCount==0 )
      {
        DeleteOption( info.Option );
        s_Storage.erase( s_Storage.find(m_ID) );
      }

      m_ID = ID();
      m_State = STATE_EMPTY;
    }

    bool IsLoading()
    {
      if( m_State==STATE_EMPTY ) { return false; }  //  読み込みしてないならfalseでしょう
      LoadingCheck();
      return m_State!=STATE_WORKING;
    }

    StorageObjectTemplate():m_State(STATE_EMPTY){}

    StorageObjectTemplate( const StorageObjectTemplate<ID,OPTION,OBJECT>& rha )
      :m_State(STATE_EMPTY)
    {
      if( rha.m_State == STATE_EMPTY ) { return ; }
      _Load(rha.m_ID); 
    }

    StorageObjectTemplate<ID,OPTION,OBJECT>& operator = ( const StorageObjectTemplate<ID,OPTION,OBJECT>& rha )
    {
      Delete();
      if( rha.m_State!=STATE_EMPTY ) 
      {
        _Load(rha.m_ID); 
      }
      return *this;
    }

    virtual ~StorageObjectTemplate()
    {
      Delete();
    }

  private:
    void _Load( const ID& id )
    {
      m_ID = id;
      STORAGEMAP::iterator ite = s_Storage.find(id);

      if( ite==s_Storage.end() )
      { //  もし初めての読み込みだったら、ファイトです！
        SPSTORAGEFUNCTION pFunc( CreateFunction(id) );
        SPSTORAGEOBJECT pObj = GlobalPointer<Storage>::Get()->ExecuteFunction( pFunc );

        STORAGEINFO& info = s_Storage[id];

        info.ReferenceCount = 1;
        info.pObject = pObj;
        info.pFunc = pFunc;
      }
      else
      {
        //  ２回目以降はキャッシュのみ
        ite->second.ReferenceCount += 1;
      }

      m_State = STATE_LOADING;

      //  とりあえず調べておく
      LoadingCheck();
    }

    void LoadingCheck()
    {
      if( m_State!=STATE_LOADING ) { return; }

      STORAGEINFO& info = GetStrageInfo();

      //  info.pObject が存在している == まだ稼動準備中ということになる
      if( info.pObject.get()!=NULL )
      {
        //  残っているメッセージがある限りは準備中
        if( info.pObject->GetMessageCount()!=0 ) { return ; }

        //  ここにきたら読み込みが完了しているので、処理しちゃう
        CreateOption( info.pFunc.get(), info.Option );
        info.pFunc.reset();
        info.pObject.reset();
      }

      LocalSetup( info.Option );
      m_State = STATE_WORKING;
    }
  };
}


#endif