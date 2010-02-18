/*!
    @file
    @brief ウィンドウクラス
 */


#ifndef maid2_framework_win32_window_h
#define maid2_framework_win32_window_h

#include"../../config/define.h"
#include"../../config/win32.h"

#include<map>
#include<boost/function.hpp>
#include<boost/bind.hpp>

#include"../../auxiliary/string.h"
#include"../../auxiliary/mathematics/point.h"
#include"../../auxiliary/mathematics/size.h"
#include"../../auxiliary/globalpointer.h"

#include"messagehook.h"
#include"osdevice.h"

namespace Maid
{ 
  class Application;
  class Window : public MessageHook , public GlobalPointer<OSDevice>
	{
	public:
		enum CALLBACKCODE
		{
			CALLBACKCODE_RETURN=0,   //!<	メッセージは処理されたのでこれ以降は呼ばない
			CALLBACKCODE_DEFAULT,    //!<	メッセージをOSに戻す
		};
		typedef boost::function1<CALLBACKCODE, WindowsMessage& >	MESSAGEFUNCTION;

	public:
		Window();
		virtual ~Window();

		void RegistCallBack( UINT Msg, const MESSAGEFUNCTION& Function );
		void DeleteCallBack( UINT Msg );

		void Show();
		void Hide();

		void SetPosition( const POINT2DI& pos );
		void SetSize( const SIZE2DI& pos );
		void SetClientSize( const SIZE2DI& pos );
		void SetStyle( DWORD Style );
		void SetDragDropMode( bool IsAccept );
		void SetCaption( const String& Name );
		void SetZOrder( HWND order );


		HWND    GetHWND()const;
		POINT2DI GetPosition()const;
		SIZE2DI GetSize()const;
		POINT2DI GetClientPosition()const;
		SIZE2DI GetClientSize()const;
		DWORD   GetStyle()const;


		void Create( const String& Caption, const POINT2DI& Pos, const SIZE2DI& Size, DWORD Style );
		void Destory();
    bool IsCreated()const;

		virtual MessageHook::RETURNCODE OnMessage( WindowsMessage& Param );

	private:
		HWND	m_hWnd;     //!< m_hWnd==NULL まだウィンドウを作ってない
		std::map<UINT,MESSAGEFUNCTION>	m_MessageMap;	//!<	メッセージマップ
	};

	//!	省略的な書き方
	#define MakeCallBackObject(pFunc,pObject) boost::bind( pFunc, pObject, _1 )
}

#endif
