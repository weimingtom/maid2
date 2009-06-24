#include"Window.h"

#include<shellapi.h>


#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/trace.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/exception.h"


namespace Maid
{
/*!
    @class	Window window.h
    @brief	ウィンドウクラス
\n          １ウィンドウにつき１クラスです。
 */ 

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
Window::Window()
  :m_hWnd(NULL)
{
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
Window::~Window()
{
	Destory();
}


HWND Window::GetHWND()const
{
  MAID_ASSERT( !IsCreated(), "まだ作成されていません" );
	return m_hWnd;
}

SIZE2DI Window::GetSize()const
{
  MAID_ASSERT( !IsCreated(), "まだ作成されていません" );

  RECT rc;
  GetWindowRect( m_hWnd, &rc );
	return SIZE2DI(rc.right-rc.left, rc.bottom-rc.top );
}

SIZE2DI Window::GetClientSize()const
{
  MAID_ASSERT( !IsCreated(), "まだ作成されていません" );
  RECT rc;
  GetClientRect( m_hWnd, &rc );
	return SIZE2DI(rc.right-rc.left, rc.bottom-rc.top );

}

DWORD Window::GetStyle()const
{
  MAID_ASSERT( !IsCreated(), "まだ作成されていません" );
  const DWORD ret = ::GetWindowLong( m_hWnd, GWL_STYLE );

	return ret;
}

bool Window::IsCreated()const
{
  return m_hWnd!=NULL;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! メッセージが飛んできたときにコールバックされる関数の設定
/*!
    @param  Msg       [i ]	ここで指定したメッセージが飛んでくるとコールバックされます
    @param  Function  [i ]	メッセージがきたときに呼ぶ予定の関数
 */
void Window::RegistCallBack( UINT Msg, const MESSAGEFUNCTION& Function )
{
	m_MessageMap[Msg] = Function;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! メッセージが飛んできたときにコールバックされる関数の削除
/*!
    @param  Msg   [i ]	削除したい関数
 */
void Window::DeleteCallBack( UINT Msg )
{
	m_MessageMap.erase( Msg );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 作成されたウィンドウを表示します
/*!
 */
void Window::Show()
{
  MAID_ASSERT( !IsCreated(), "まだ作成されていません" );

  ::ShowWindow(m_hWnd, SW_SHOW );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 作成されたウィンドウを非表示にします
/*!
 */
void Window::Hide()
{
  MAID_ASSERT( !IsCreated(), "まだ作成されていません" );

	ShowWindow(m_hWnd, SW_HIDE );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ウィンドウの表示位置の変更
/*!
    @param	pos [i ]	あたらしく設定する表示座標
 */
void Window::SetPosition( const POINT2DI& pos )
{
  MAID_ASSERT( !IsCreated(), "まだ作成されていません" );
	::SetWindowPos( m_hWnd, NULL, pos.x, pos.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE );
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ウィンドウのサイズの変更
/*!
    @param	size [i ]	あたらしく設定するサイズ
 */
void Window::SetSize( const SIZE2DI& size )
{
  MAID_ASSERT( !IsCreated(), "まだ作成されていません" );

  ::SetWindowPos( m_hWnd, NULL, 0, 0, size.w, size.h, SWP_NOZORDER|SWP_NOMOVE );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! クライアントサイズの変更
/*!
    @param	size [i ]	あたらしく設定するクライアントサイズ
 */
void Window::SetClientSize( const SIZE2DI& size )
{
  MAID_ASSERT( !IsCreated(), "まだ作成されていません" );

	const DWORD StyleFlag = GetStyle();
	RECT	rc = { 0, 0, size.w, size.h };
	::AdjustWindowRect( &rc, StyleFlag, FALSE );
	const int w = rc.right  - rc.left;
	const int h = rc.bottom - rc.top;

	::SetWindowPos( m_hWnd, NULL, 0, 0, w, h, SWP_NOZORDER|SWP_NOMOVE );
 }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ウィンドウスタイルの変更
/*!
 */
void Window::SetStyle( DWORD Style )
{
  MAID_ASSERT( !IsCreated(), "まだ作成されていません" );


	::SetWindowLong( m_hWnd, GWL_STYLE, Style );

  //  枠が替わることがあるので、再描画してもらう
	::SetWindowPos( m_hWnd, NULL,
			-1, -1,
			-1, -1,
			SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED );
}

void Window::SetCaption( const String& Name )
{
  MAID_ASSERT( !IsCreated(), "まだ作成されていません" );
  ::SetWindowText( m_hWnd, String::ConvertMAIDtoUNICODE(Name).c_str() );
}

void Window::SetZOrder( HWND order )
{
  MAID_ASSERT( !IsCreated(), "まだ作成されていません" );
	::SetWindowPos( m_hWnd, order,
			-1, -1,
			-1, -1,
			SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED );
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! このウィンドウへのＤ＆Ｄを許可するか？
/*!
    @param	IsAccept	[i ]	true で許可
 */
void Window::SetDragDropMode( bool IsAccept )
{
  MAID_ASSERT( !IsCreated(), "まだ作成されていません" );

	::DragAcceptFiles( m_hWnd, IsAccept? TRUE:FALSE );

}










/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ウィンドウを作成します
/*!
    作成されるだけでまだ表示されません
    表示させるときは Show() を呼び出す必要があります
*/

void Window::Create( const String& Caption, const POINT2DI& Pos, const SIZE2DI& Size, DWORD Style )
{
	Destory();

	//	CreateWindowEx() の内部でWM_CREATEが送られてくるけど
	//	RegistCallBack()でフックしようとしてもフックできない。
	//	そこで強引に行ってしまう
	HWND hWnd = NULL;
 
  {
    const std::wstring ClassName    = String::ConvertMAIDtoUNICODE( GlobalPointer<OSDevice>::Get()->GetClassName() );
    const HINSTANCE hInstance = GlobalPointer<OSDevice>::Get()->GetHINSTANCE();

	  EnterInterruptMode();
	  hWnd = ::CreateWindowEx(	NULL,
								  ClassName.c_str(),
								  String::ConvertMAIDtoUNICODE(Caption).c_str(),
								  Style,
								  Pos.x , Pos.y,
								  Size.w, Size.h,
								  NULL, NULL,
								  hInstance,
								  NULL );
	  LeaveInterruptMode();
  }

  if( hWnd==NULL ) { MAID_WARNING( MAIDTEXT("ウィンドウの作成に失敗") ); return ; }

	AddProc( hWnd );
	m_hWnd = hWnd;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ウィンドウを破棄します
/*!
 */
void Window::Destory()
{
	if( m_hWnd!=NULL )
	{
		::DestroyWindow( m_hWnd );
		DelProc();
	}

	m_hWnd = NULL;
}




/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Application からフックされて呼び出される関数
/*!
    引数などは MessageHook::WindowProc を参照すること
 */
MessageHook::RETURNCODE Window::OnMessage( WindowsMessage& Param )
{
  std::map<UINT,MESSAGEFUNCTION>::iterator ite = m_MessageMap.find( Param.GetMSG() );

  if( ite!=m_MessageMap.end() )
  {
    switch( ite->second( Param ) )
    {
    case CALLBACKCODE_RETURN:   { return RETURNCODE_RETURN; }break;
    case CALLBACKCODE_DEFAULT: { return RETURNCODE_DEFAULT; }break;
    }
  }

  return RETURNCODE_DEFAULT;
}

}