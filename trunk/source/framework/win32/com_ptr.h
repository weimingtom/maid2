/*!
    @file
    @brief	COM 用スマートポインタ
 */

#ifndef maid2_framework_win32_com_ptr_h
#define maid2_framework_win32_com_ptr_h

#include"../../config/define.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/exception.h"
#include<objbase.h>

namespace Maid
{ 
	/*!
      @brief	COM 用スマートポインタ
  \n			参照がなくなったときに Release() されます
	 */
  template <class ComObject>
  class com_ptr
  {
  public:
    com_ptr() : m_pInterface(NULL) {}								//!<	コンストラクタその１
    explicit com_ptr( ComObject* pInterface ) : m_pInterface(pInterface){}	//!<	コンストラクタその２
    com_ptr(const com_ptr<ComObject>& pObject )						//!<	コンストラクタその３
    {
      m_pInterface = pObject.m_pInterface;
      if( !IsNull() )
      {
	      m_pInterface->AddRef();
      }
    }

    ~com_ptr()	//!<	デストラクタ
    {
      reset();
    }

    //! このクラスの初期化
    /*!
        @param	pInterface	[i ]	新しく管理するＣＯＭ（デフォルトでＮＵＬＬ）
     */
    void reset( ComObject* pInterface=NULL )	//!<	デストラクタ
    {
      if( m_pInterface==pInterface ) { return ; }

      if( !IsNull() )
      {
	      m_pInterface->Release();
      }
      m_pInterface = pInterface;
    }

    //! コピー
    /*!
        @param	NewCom	[i ]	共有するＣＯＭ
     */
    com_ptr<ComObject>& operator=( com_ptr<ComObject> const & NewCom )
    {
      if( m_pInterface==NewCom.get() ) { return *this; }
      reset();
      m_pInterface = NewCom.get();
      m_pInterface->AddRef();
      return *this;
    }

    bool IsNull() const { return m_pInterface==NULL; }	//!<	何も管理していないか？

    ComObject* get() const 	{ return m_pInterface;	}	//!<	ＣＯＭ取得
    ComObject* operator->() const //!<	保持しているＣＯＭを透過的に扱う
    {
      MAID_ASSERT( get()==NULL, "まだ設定されていません" );
      return get();  
    }		

    //! COMオブジェクトの作成
    /*!
        @param  ClassID       [i ]	CoCreateInstance() のヘルプを参照
        @param	pUnknown      [i ]	CoCreateInstance() のヘルプを参照
        @param	ClassContext  [i ]	CoCreateInstance() のヘルプを参照
        @param	riid          [i ]	CoCreateInstance() のヘルプを参照

        @exception Exception COMオブジェクトの作成に失敗した場合
     */
    void CoCreateInstance( REFCLSID ClassID, LPUNKNOWN pUnknown, DWORD ClassContext, REFIID riid )
    {
      reset();
      const HRESULT ret = ::CoCreateInstance( ClassID, pUnknown, ClassContext, riid, (void **)&m_pInterface );

      if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("CoCreateInstance() に失敗") ); }
    }

    //! COMオブジェクトの作成
    /*!
      @param	riid		[i ]	QueryInterface() のヘルプを参照
      @param	pObj		[ o]	作成されたCOMオブジェクト

      @exception Exception COMオブジェクトの作成に失敗した場合
     */
    template <class QueryObject>
    void QueryInterface( REFIID riid, com_ptr<QueryObject>& pObj )
    {
      MAID_ASSERT( get()==NULL, "まだ設定されていません" );

      QueryObject* p;
      const HRESULT ret = m_pInterface->QueryInterface( riid, (void **)&p );
      if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("QueryInterface() に失敗") ); }

      pObj.reset(p);
    }

  private:
	  ComObject*	m_pInterface;
  };

  template<class ComObject> 
  inline bool operator==(const com_ptr<ComObject>  & lhs, const com_ptr<ComObject>  & rhs)
  {
    return lhs.get() == rhs.get();
  }

  template<class ComObject> 
  inline bool operator!=(const com_ptr<ComObject>  & lhs, const com_ptr<ComObject>  & rhs)
  {
    return !(lhs == rhs);
  }
}
#endif
