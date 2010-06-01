#ifndef maid2_auxiliary_globalkeyvaluetable_h
#define maid2_auxiliary_globalkeyvaluetable_h


#include"../config/define.h"
#include"debug/assert.h"
#include"debug/warning.h"

#include<map>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  /*!
    @class  GlobalKeyValueTable globalkeyvaluetable.h
    @brief  このインスタンスがグローバルな共有されるテンプレート
  */
	template<class KEY, class VALUE>
	class GlobalKeyValueTable
	{
  private:
    typedef boost::shared_ptr<VALUE> SPVALUE;
    typedef std::map<KEY, SPVALUE> KEYVALUETABLE;

    SPVALUE m_pCurrent;
    KEY     m_Key; 


    static KEYVALUETABLE& GetTable()
    {
      static KEYVALUETABLE s_obj;
      return s_obj;
    }



  public:
    GlobalKeyValueTable()
    {
    }
    ~GlobalKeyValueTable()
    {
      Release();
    }

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! すでにキーが存在しているか
		/*!
	 	  @param  key	[i ]	調べるキー
		 
		 	戻り値	存在しているなら true
		\n        ないなら false
		 */
    bool IsExist( const KEY& key ) const
    {
      return GetTable().find(key)!=GetTable().end();
    }

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! データを作成する
		/*!
	 	  @param  key	  [i ]	キー
	 	  @param  value	[i ]	データ	 
		 */
    void Create( const KEY& key, const VALUE& value )
    {
      Release();

      SPVALUE p( new VALUE(value) );

      GetTable()[key] = p;
      m_pCurrent = p;
      m_Key = key;
    }

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! データを取得する
		/*!
	 	  @param  key	  [i ]	キー
		 */
    void Load( const KEY& key )
    {
      MAID_ASSERT( !IsExist(key), "データがありません" ); 

      m_pCurrent = GetTable()[key];
      m_Key = key;
    }

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! このインスタンスはデータを取得していないか？
		/*!
	 	  @param  key	  [i ]	キー
		 	戻り値	取得しているなら true
		\n        ないなら false
		 */
    bool IsEmpty() const
    {
      return m_pCurrent.get()==NULL;
    }

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 取得しているデータを開放する
		/*!
		 */
    void Release()
    {
      if( m_pCurrent.get()==NULL ) { return ; }

      const KEY key = m_Key;
      m_Key = KEY();
      m_pCurrent.reset();

      KEYVALUETABLE::iterator ite = GetTable().find(key);
      if( ite->second.unique() )
      {
        GetTable().erase(ite);
      }
    }

    const KEY& GetKey() const{ return m_Key; }
    const VALUE& GetValue() const{ return *m_pCurrent; }
          VALUE& GetValue(){ return *m_pCurrent; }

  };
}

#endif