
#include"Window.h"

#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/trace.h"
#include"../../auxiliary/exception.h"

#include"application.h"
#include<shellapi.h>

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
{
	m_Caption = MAIDTEXT("window");
	m_Pos  = POINT2DI(CW_USEDEFAULT,CW_USEDEFAULT);
	m_Size = SIZE2DI(320, 240);
  m_Style = WS_OVERLAPPEDWINDOW;

	m_IsShow  = false;
	m_IsDandD = false;
	m_hWnd = NULL;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
Window::~Window()
{
	Destory();
}


HWND Window::GetHWND()
{
	return m_hWnd;
}

SIZE2DI Window::GetSize()
{
	return m_Size;
}

DWORD Window::GetStyle()
{
	return m_Style;
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
	MAID_ASSERT( m_hWnd==NULL, "まだウィンドウが作成されていません" );

//	if( !m_IsShow ) 
	{
		::ShowWindow(m_hWnd, SW_SHOW );
		m_IsShow = true;
	}
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 作成されたウィンドウを非表示にします
/*!
 */
void Window::Hide()
{
	MAID_ASSERT( m_hWnd==NULL, "まだウィンドウが作成されていません" );

	if( m_IsShow )
	{
		ShowWindow(m_hWnd, SW_HIDE );
		m_IsShow = false;
	}
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ウィンドウの表示位置の変更
/*!
    @param	pos [i ]	あたらしく設定する表示座標
 */
void Window::SetPosition( const POINT2DI& pos )
{
	if( m_hWnd!=NULL )
	{
		::SetWindowPos( m_hWnd, NULL, pos.x, pos.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE );
	}

	m_Pos = pos;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ウィンドウの表示位置の変更
/*!
    @param	size [i ]	あたらしく設定するクライアントサイズ
 */
void Window::SetSize( const SIZE2DI& size )
{
	if( m_hWnd!=NULL )
	{
		const DWORD StyleFlag = GetStyle();
		::RECT	rc = { 0, 0, size.w, size.h };
		::AdjustWindowRect( &rc, StyleFlag, FALSE );
		const int w = rc.right  - rc.left;
		const int h = rc.bottom - rc.top;

		::SetWindowPos( m_hWnd, NULL, 0, 0, w, h, SWP_NOZORDER|SWP_NOMOVE );
	}

	m_Size = size;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ウィンドウスタイルの変更
/*!
 */
void Window::SetStyle( DWORD Style )
{
	if( m_hWnd!=NULL )
	{
		::SetWindowLong( m_hWnd, GWL_STYLE, Style );

		::SetWindowPos( m_hWnd, NULL,
				-1, -1,
				-1, -1,
				SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED|SWP_SHOWWINDOW );
	  //	ウィンドウの大きさがかわることがあるので、再設定
	  SetSize(m_Size);
	}

  m_Style = Style;
}

void Window::SetCaption( const String& Name )
{
	m_Caption = Name;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! このウィンドウへのＤ＆Ｄを許可するか？
/*!
    @param	IsAccept	[i ]	true で許可
 */
void Window::SetDragDropMode( bool IsAccept )
{
	if( m_hWnd!=NULL )
	{
		::DragAcceptFiles( m_hWnd, IsAccept? TRUE:FALSE );
	}
	m_IsDandD = IsAccept;
}










/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ウィンドウを作成します
/*!
    作成されるだけでまだ表示されません
    表示させるときは Show() を呼び出す必要があります

    @exception CException オープンに失敗した場合
*/
void Window::Create()
{
	Destory();


	HWND hWnd = NULL;
	DWORD StyleFlag = GetStyle();
	std::wstring ClassName;
	std::wstring Caption = String::ConvertMAIDtoUNICODE(m_Caption);

  const String name = GlobalPointer<Application>::Get()->GetClassName();
  ClassName = String::ConvertMAIDtoUNICODE(name);


	RECT	rc = { 0, 0, m_Size.w, m_Size.h };
	AdjustWindowRect( &rc, StyleFlag, FALSE );
	const int w = rc.right  - rc.left;
	const int h = rc.bottom - rc.top;

	//	CreateWindowEx() の内部でメッセージが送られてくる可能性があるけど
	//	RegistCallBack()でフックしようとしてもフックできない。
	//	そこで強引に行ってしまう

  {
	  EnterInterruptMode();
	  hWnd = ::CreateWindowEx(	NULL,
								  ClassName.c_str(),
								  Caption.c_str(),
								  StyleFlag,
								  m_Pos.x , m_Pos.y,
								  w, h,
								  NULL, NULL,
								  GlobalPointer<Application>::Get()->GetHINSTANCE(),
								  NULL );
	  LeaveInterruptMode();
  }

	if( hWnd==NULL )
	{
    MAID_THROWEXCEPTION( MAIDTEXT("ウィンドウの作成に失敗") );
	}

	AddProc( hWnd );
	m_hWnd = hWnd;

	if( m_IsDandD ) { SetDragDropMode(true); }

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
	m_IsShow = false;
	m_IsDandD= false;
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
    case CALLBACKCODE_CONTINUE: { return RETURNCODE_CONTINUE; }break;
    }
  }

  return RETURNCODE_CONTINUE;
}

}