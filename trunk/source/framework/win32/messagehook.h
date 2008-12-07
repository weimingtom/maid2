/*!
    @file
    @brief Win32 用メッセージプロシージャ
 */


#ifndef framework_win32_messagehook_h
#define framework_win32_messagehook_h

#include"../../config/define.h"
#include"../../config/win32.h"
#include"../../auxiliary/macro.h"
#include"../../auxiliary/globalpointer.h"



namespace Maid
{
  /*!
 	  @class	WindowsMessage WindowsMessage.h
 	  @brief	プロシージャ用編集クラス
   */
  class WindowsMessage
  {
  public:
    //!	コンストラクタ
    WindowsMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
      : m_hWnd(hWnd),m_Msg(msg),m_wParam(wParam),m_lParam(lParam)
    {

    }


    HWND	GetHWND() const { return m_hWnd; }		//!<	メッセージの送信先ハンドル
    UINT	GetMSG() const { return m_Msg; }		//!<	投げられたメッセージの取得

    enum PARAM
    {
      PARAM_LO,
      PARAM_HI,
      PARAM_ALL
    };


    WPARAM	GetWPARAM(PARAM pos=PARAM_ALL) const 
    {
      if( pos==PARAM_LO ) { return LOWORD(m_wParam); }
      if( pos==PARAM_HI ) { return HIWORD(m_wParam); }
      return m_wParam; 
    }

    LPARAM	GetLPARAM(PARAM pos=PARAM_ALL) const
    {
      if( pos==PARAM_LO ) { return LOWORD(m_lParam); }
      if( pos==PARAM_HI ) { return HIWORD(m_lParam); }
      return m_lParam; 
    }

    void SetHWND( HWND hWnd ) { m_hWnd = hWnd; }
    void SetMSG( UINT msg ) { m_Msg = msg; }
    void SetWPARAM( WPARAM wparam ) { m_wParam = wparam; }
    void SetLPARAM( LPARAM lparam ) { m_lParam = lparam; }

  private:
    HWND	  m_hWnd;
    UINT	  m_Msg;
    WPARAM	m_wParam;
    LPARAM	m_lParam;
  };


  class MessageHookManager;
  class MessageHook: public GlobalPointer<MessageHookManager>
  {
  public:
    MessageHook();
    virtual ~MessageHook();

    /*!
        WindowProc() で戻ってくるコード
     */
    enum RETURNCODE
    {
      RETURNCODE_RETURN=0,   //!<	メッセージは処理されたのでこれ以降は呼ばない
      RETURNCODE_DEFAULT,    //!<	メッセージをOSに戻す
    };

    void AddProc( HWND hWnd );
    void DelProc();

    void EnterInterruptMode();
    void LeaveInterruptMode();

    //! プロシージャにコールバックしてもらう関数
    /*!
        この関数を継承先で実装します

        @param	mess	[i ]	送られてきたメッセージ情報

        @retval	 RETURNCODE_EXIT		これ以降メッセージ処理を行いません
        @retval	 RETURNCODE_DEFAULT	まだ続けます
     */
	  virtual RETURNCODE OnMessage( WindowsMessage& mess )=0;

  private:
    bool	m_IsHook;	//!<	現在フックされているか
    HWND	m_hWnd;		//!<	現在のハンドル
  };
}

#endif
