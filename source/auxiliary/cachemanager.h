/*!
 	@file
 	@brief	キャッシュコンテナ
 */

#ifndef auxiliary_cachemanager_h
#define auxiliary_cachemanager_h

#include"../config/define.h"
#include"macro.h"

#include<map>
#include<set>
#include<algorithm>

namespace Maid
{ 
  /*!
    @class CacheManager cachemanager.h
    @brief  キャッシュコンテナ
  \n        データをどんどん追加していって、特定個数になると使われていない順に自動的に削除してくれます。
   */
  template<class KEY, class DATA>
  class CacheManager
  {
  public:
    CacheManager()
      :m_CacheMax(200)
      ,m_UpdateCount(0)
      ,m_GarbageState(GARBAGESTATE_AUTO)
    {
    }


    enum GARBAGESTATE
    {
      GARBAGESTATE_MANUAL,	//!<	使う人が Garbage() を呼び出す
      GARBAGESTATE_AUTO,		//!<	自動的に Garbage() が呼ばれる
    };

    void SetGarbageState( GARBAGESTATE state )
    {
      m_GarbageState = state;
    }


    //! 登録されているデータの取得
    /*!
      登録してないデータを取得しようとしたら動作は不定です。
      必ず IsExist() でデータが存在しているか調べること
  	 
      @param	Key	[i ]	取得したいデータのキー
  	 
      @return	データ
     */
    const DATA& GetData( const KEY& Key )
    {
      m_UpdateCount++;

      CACHELIST::iterator ite = m_CacheList.find( Key );

      CACHEDATA dat = *(ite->second);

      dat.UpdateCount = m_UpdateCount;

      m_Pool.erase( ite->second );
      std::pair<POOL::iterator, bool> ret = m_Pool.insert(dat);
      m_CacheList[Key] = ret.first;

      return ret.first->Data;
    }

    //! データが存在しているか調べる
    /*!
      @param	Key	[i ]	調べたいデータのキー
  	 
      @return	データが存在しているなら true
     */
    bool IsExist( const KEY& Key )
    {
      return m_CacheList.find( Key )!=m_CacheList.end();
    }


    //! データの登録
    /*!
      @param	Key		[i ]	登録するデータのキー
      @param	Data	[i ]	登録するデータ
     */
    void Register( const KEY& Key, const DATA& Data )
    {
      m_UpdateCount++;

      CACHEDATA dat( Key, Data, m_UpdateCount );

      std::pair<POOL::iterator, bool> ret = m_Pool.insert(dat);

      if( !(ret.second) ) { return ; }

      m_CacheList[Key] = ret.first;

      if( m_GarbageState == GARBAGESTATE_AUTO )
      {
	      Garbage();
      }
    }

    //! 登録しているデータをすべて消す
    /*!
     */
    void ClearAll()
    {
      m_CacheList.clear();
      m_Pool.clear();
    }

    //! 登録しているデータを消す
    /*!
     */
    void ClearData( const KEY& Key )
    {
      CACHELIST::iterator ite = m_CacheList.find( Key );

      if( ite==m_CacheList.end() ) { return ; }

      m_Pool.erase( ite->second );
      m_CacheList.erase( Key );
    }

    //! キャッシュとして保持する数を変更する
    /*!
      @param	Size			[i ]	最低限キャッシュしておく数
     */
    void SetCacheSize( int Size )
    {
      m_CacheMax = Size;
      Garbage();
    }

    //! キャッシュの掃除
    /*!
      m_CacheMax 個まで減少させます
     */
    void Garbage()
    {
      const int Max = m_CacheMax;

      while( true )
      {
	      if( (int)m_CacheList.size()<=Max ) { break; }
	      POOL::iterator ite = m_Pool.begin();

	      m_CacheList.erase( ite->Key );
	      m_Pool.erase( ite );
      }
    }

  private:

    struct CACHEDATA
    {
      CACHEDATA(){}
      CACHEDATA( const CACHEDATA& rha )
        :Key(rha.Key),Data(rha.Data),UpdateCount(rha.UpdateCount)
      {}

      CACHEDATA( const KEY& k, const DATA& d, int c)
        :Key(k),Data(d),UpdateCount(c)
      {}


      KEY		Key;
      DATA	Data;
      int		UpdateCount;

      bool operator < ( const CACHEDATA& rhs ) const
      {
	      return UpdateCount < rhs.UpdateCount;
      }
    };

    //!	作成された物を溜めておくところ
    typedef std::set<CACHEDATA> POOL;
    typedef std::map<typename KEY,typename POOL::iterator> CACHELIST;


  private:
    #pragma COMPILERMSG("TODO:最近使われた順に並べるだけなのでヒープソートでもできそう")	
    CACHELIST m_CacheList;  //!<	キャッシュの作成時間でソートされているリスト
    POOL      m_Pool;       //!<	キャッシュデータ
    int       m_CacheMax;   //!<	キャッシュを保持できる最大数

    int           m_UpdateCount;	//!<	m_Poolが更新された時間（コレを元にキャッシュされる）
    GARBAGESTATE  m_GarbageState;
  };
}


#endif