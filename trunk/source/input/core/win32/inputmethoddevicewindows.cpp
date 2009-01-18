
#include"inputmethoddevicewindows.h"

#include<imm.h>
#pragma comment(lib, "imm32.lib")


namespace Maid{ namespace Input {

/*!
 	@class	InputMethodDeviceWindows inputmethoddevicewindows.h
 	@brief	FEP管理クラス
\n			IME の状態から入力されているテキストを取得できるクラス
 *			http://www.honet.ne.jp/~tri/program/noime.html
 */


	//	自動releaseクラス
	class IMCHandle
	{
	public:
		IMCHandle( const Window& hWnd )
		{
			m_hWnd = hWnd.GetHWND();
			m_hImc = ::ImmGetContext(m_hWnd);
		}
		~IMCHandle()
		{
			::ImmReleaseContext(m_hWnd,m_hImc);
		}

		HIMC Get() { return m_hImc; }
	private:
		HWND	m_hWnd;
		HIMC	m_hImc;
	};


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
InputMethodDeviceWindows::InputMethodDeviceWindows( const Window& hWnd )
  :m_pCandidateList(NULL)
  ,m_Window(hWnd)
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
InputMethodDeviceWindows::~InputMethodDeviceWindows()
{
    Finalize();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! FEPの開放
/*!
*/
void InputMethodDeviceWindows::Finalize()
{
	m_pCandidateList = NULL;
	DelProc();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! FEP の初期化
/*!
 	@param	hWnd	[i ]	関連つけるウィンドウハンドル
 */
void InputMethodDeviceWindows::Initialize()
{
  Finalize();

	AddProc( m_Window.GetHWND() );
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! FEP の起動
/*!
 */
void InputMethodDeviceWindows::Open()
{
	if( IsOpen() ) { return ; }

	IMCHandle hImc(m_Window);
	ImmSetOpenStatus(hImc.Get(), TRUE);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! FEP の終了
/*!
 */
void InputMethodDeviceWindows::Close()
{
	if( !IsOpen() ) { return ; }

	IMCHandle hImc(m_Window);
	ImmSetOpenStatus(hImc.Get(), FALSE);
}




/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! FEP がうごいているか？
/*!
 *	\return ＦＥＰが動いているなら true
\n			うごいていないなら false
 */
bool InputMethodDeviceWindows::IsOpen() const
{
	IMCHandle hImc(m_Window);
	const bool IsOpen = ImmGetOpenStatus(hImc.Get())!=0;
	return IsOpen;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ＦＥＰの変換状態を取得する
/*!
 	@param	pConversion	[i ]	入力モード(半角カナ、全角　等）
 	@param	pSentence	[i ]	変換モード(一般・地名優先　等）
 */
void InputMethodDeviceWindows::GetStatus( DWORD* pConversion, DWORD* pSentence) const
{
	IMCHandle hImc(m_Window);

	ImmGetConversionStatus(hImc.Get(),pConversion,pSentence);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 変換位置の取得
/*!
 	@return	変換位置
 */
int InputMethodDeviceWindows::GetConvertPos() const
{
	IMCHandle hImc(m_Window);
    DWORD dwBuff[2];
    if(::ImmGetCompositionString(hImc.Get(),GCS_COMPCLAUSE,dwBuff,sizeof(DWORD)*2) == 0) { return 0; }
    return dwBuff[1];
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! カーソル位置の取得
/*!
 	@return	カーソル位置
 */
int InputMethodDeviceWindows::GetCursorPos() const
{
  IMCHandle hImc(m_Window);

  DWORD count = ::ImmGetCompositionString(hImc.Get(),GCS_CURSORPOS,NULL,0);

  return count;
/*
  // ↑の戻り値はバイト数で戻ってくるので
  // 文字数に変換する

  const String str = GetCompString();
  if( str.empty() ) { return 0; }

  int index = 0;

  while( true )
  {
    if( count==0 ) { break; }

    if( str[index]<=0xFF )	{ count -= 1; }
    else					{ count -= 2; }

    ++index;
  }
  return index;
*/
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 変換候補リストの作成
/*!
   GetCountCandidate()
\n GetCandidateIndex()
\n GetCandidateString()
\n GetCandidateStart()
\n GetCandidateEnd()
\n	を使う前に呼んでおくこと
 */
void InputMethodDeviceWindows::UpdateCandidate()
{
  IMCHandle hImc(m_Window);
  //変換候補の取得
  const INT iCandidateSize = ::ImmGetCandidateList(hImc.Get(),0,NULL,0);
  if(iCandidateSize==0) { return; }

  m_pCandidateBuff.reset( new unt08[iCandidateSize] );
  m_pCandidateList = (LPCANDIDATELIST)m_pCandidateBuff.get();

  ::ImmGetCandidateList(hImc.Get(),0,m_pCandidateList,iCandidateSize);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 変換候補数の取得
/*!
 	@return 変換候補数
 */
int InputMethodDeviceWindows::GetCandidateCount() const
{
	if( m_pCandidateList==NULL ) { return 0; }
	return m_pCandidateList->dwCount;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在選択中の変換候補の取得
/*!
 	@return 変換候補位置
 */
int InputMethodDeviceWindows::GetCandidateSelect() const
{
	if(m_pCandidateList==NULL) { return 0; }
    return m_pCandidateList->dwSelection;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! FEP入力文字列の取得
/*!
 	@return	入力中の文字列
 */
String InputMethodDeviceWindows::GetCompString() const
{
  IMCHandle hImc(m_Window);
  const LONG lByte = ::ImmGetCompositionString(hImc.Get(),GCS_COMPSTR,NULL,0);

  if( lByte==0 ) { return String(); }

  std::wstring	Buff;
  Buff.resize(lByte);

  ::ImmGetCompositionString( hImc.Get(), GCS_COMPSTR, &Buff[0], lByte);

  return String::ConvertUNICODEtoMAID(Buff);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定されたインデックスの変換候補を取得
/*!
 	@return 変換候補文字
 */
String InputMethodDeviceWindows::GetCandidateString(int Index) const
{
	if(m_pCandidateList==NULL) { return String(); }

  unt08* p = (unt08*)m_pCandidateList+m_pCandidateList->dwOffset[Index];

	std::wstring ret = (wchar_t*)p;

  return String::ConvertUNICODEtoMAID( ret );
}


void InputMethodDeviceWindows::Flush( String& buffer )
{
  buffer = m_EnterString;
  m_EnterString.clear();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ウィンドウプロシージャ
/*!
 */
InputMethodDeviceWindows::RETURNCODE InputMethodDeviceWindows::OnMessage( WindowsMessage& Param )
{
	switch( Param.GetMSG() )
	{
	case WM_IME_SETCONTEXT:
		{
			Param.SetLPARAM( Param.GetLPARAM()&~ISC_SHOWUIALL );
		}break;

	case WM_IME_COMPOSITION:
		{
      if( Param.GetLPARAM()&GCS_RESULTSTR )
			{
        return RETURNCODE_DEFAULT;
/*
				IMCHandle hImc(m_Window);
				const LONG lByte = ::ImmGetCompositionString(hImc.Get(),GCS_RESULTSTR,NULL,0);
				if( lByte==0 ) { break; }

				std::wstring	str;
				str.resize(lByte);

				::ImmGetCompositionString( hImc.Get(), GCS_RESULTSTR, &str[0], lByte);

        m_EnterString += String::ConvertUNICODEtoMAID(str);
*/
			}
			return RETURNCODE_RETURN;
		}
		break;

	case WM_IME_STARTCOMPOSITION: { return RETURNCODE_RETURN;}break;
	case WM_IME_ENDCOMPOSITION:   { return RETURNCODE_RETURN;}break;

	case WM_IME_NOTIFY:
		{
			switch(Param.GetWPARAM())
			{
			case IMN_OPENSTATUSWINDOW:
			case IMN_CLOSESTATUSWINDOW:
				{
					return RETURNCODE_RETURN;
				}break;

			case IMN_OPENCANDIDATE:
			case IMN_CHANGECANDIDATE:
			case IMN_CLOSECANDIDATE:
				{
					UpdateCandidate();
					return RETURNCODE_RETURN;
				}break;
			}
		}break;

	}

	return RETURNCODE_DEFAULT;
}

}}


/*
  http://nienie.com/~masapico/api_ImmGetCompositionString.html
  このへんを参考

*/


/*

  http://www.geocities.co.jp/SiliconValley-Cupertino/5872/Message/WM/
  からの抜き出し

WM_IME_CHAR = &H286 
	機能 
		IMEが変換結果の文字を受け取ったとき、アプリケーションに送られる
 
	wParam 
		1バイトの文字のときASCⅡ値
		2バイトの文字のときシフトJISコード
 
	lParam 
		キーストロークメッセージ
 


WM_IME_COMPOSITION = &H10F 
機能 
キーの入力の結果、変換文字列が変化したときにアプリケーションに送られる
 
wParam 
変換文字列に対して最後の変化を表す2バイト文字
 
lParam 
変換文字列が変化した原因を表す定数の組み合わせ
定数                  値         内容
---------------------------------------------------------------------------
GCS_COMPATTR          &H10       変換文字列の属性を取得･更新
GCS_COMPLAUSE         &H20       変換文字列の句情報を取得･更新
GCS_COMPREADATTR      &H2        現在の変換文字列の読みの属性取得・更新
GCS_COMPREADCLAUSE    &H4        現在の文字列の読みの句情報を取得･更新
GCS_COMPREADSTR       &H1        現在の変換文字列の読みを取得・更新
GCS_COMPSTR           &H8        現在の変換文字列を取得・更新
GCS_CURSORPOS         &H80       変換文字列のカーソルの位置を取得･更新
GCS_RESULTCLAUSE      &H1000     変換後文字列の句情報を取得・更新
GCS_RESULTREADCLAUSE  &H400      変換後文字列の読みの句情報を取得･更新
GCS_RESULTREADSTR     &H200      変換後文字列の読みの属性を取得･更新
GCS_RESULTSTR         &H800      変換後文字列を取得･更新
---------------------------------------------------------------------------
 
備考 
このメッセージのSDKの解説は不備な点が多い
 



機能 
変換ウィンドウの領域を確保できないとき、アプリケーションに送られる
 
wParam 
常に0
 
lParam 
常に0
 
備考 
このメッセージを受け取った場合
IME_SETCOMPOSITIONWINDOWを使って表示方法を指定する
 


  WM_IME_CONTROL = &H283 
機能 
アプリケーションが作成したIMEウィンドウに対して
実行すべき内容をIMEウィンドウに送るメッセージ
 
wParam 
実効内容を指定するフラッグ(IMC_xxx)
 
lParam 
wParamの設定に応じた値
 
戻り値 
lParamが戻る
 

WM_IME_ENDCOMPOSITION = &H10E
  機能 
IMEが変換を終了するときアプリケーションに送られる
 
wParam 
常に0
 
lParam 
常に0
 





WM_IME_KEYDOWN = &H290 
キーを押したときアプリケーションに送られる
 
wParam 
仮想キーコード
 
lParam 
キーストロークメッセージ
 


WM_IME_KEYUP = &H291 
機能 
キーを離したときアプリケーションに送られる
 
wParam 
仮想キーコード
 
lParam 
キーストロークメッセージ
 





WM_IME_NOTIFY = &H282 
機能 
IMEウィンドウに変化があったときアプリケーションに送られる
 
wParam 
IMEウィンドウの実行内容を指定するフラッグ(IMN_xxx)
 
lParam 
wParamに応じた値
 
備考 
アプリケーションがIMEウィンドウの全ての動作に対して
責任を持つ場合に使うメッセージ
 




WM_IME_REQUEST = &H288 
機能 
IMEの状態に関する情報を要求する

 
wParam 
要求する内容を指定するフラッグ(IMR_xxx)
 
lParam 
wParamに応じた値
 
備考 
アプリケーションが直接IMEウィンドウを管理する場合に使う
 


WM_IME_SELECT = &H285 
機能 
システムがIMEを変更しようとしているとき、アプリケーションに送られる
 
wParam 
現在のIMEが選択されているとき   1
選択されてないとき              0
 
lParam 
IMEに関連付けられているキーボードレイアウトのハンドル
 


WM_IME_SETCONTEXT = &H281 
機能 
アプリケーションのウィンドウがアクティブになるとき
アプリケーションに送られる
 
wParam 
入力コンテキストがアクティブなとき     1
アクティブでないとき                   0
 
lParam 
表示するウィンドウを指定するフラッグ(IMC_xxx)
 

WM_IME_STARTCOMPOSITION = &H10D 
機能 
キー入力によって変換文字列が生成される直前にアプリケーションに送られる
 
wParam 
常に0
 
lParam 
常に0
 


























http://www.honet.ne.jp/~tri/program/noime.html
のこぴぺ

  IMEで文字を表示させない方法　～正攻法～
一つ目は、純粋に描画を止める方法でいくつかのIME関連のメッセージを処理することで実現できます。

switch(uMsg){
case WM_IME_SETCONTEXT:
    lParam &= ~ISC_SHOWUIALL;
    break;
case WM_IME_STARTCOMPOSITION:
case WM_IME_COMPOSITION:
case WM_IME_ENDCOMPOSITION:
    return 0;
case WM_IME_NOTIFY:
    switch(wParam){
    case IMN_OPENSTATUSWINDOW:
    case IMN_CLOSESTATUSWINDOW:
    case IMN_OPENCANDIDATE:
    case IMN_CHANGECANDIDATE:
    case IMN_CLOSECANDIDATE:
        return 0;
    default:
        return DefWindowProc(..[略]..);
    }
default
    return DefWindowProc(..[略]..);
}

とまぁこんな感じ。順番に説明していくと…

WM_IME_SETCONTEXT
このメッセージは基本的にウィンドウがアクティブ/非アクティブになったときに送られてきます。 
その中のLPARAMはIME関連のウィンドウを再描画するかどうかがフラグで指定されています。
で、ここでは全部表示する必要は無いのでISC_SHOWUIALLで表示フラグを全て消して再描画させてません。
最初はMSDNのドキュメントを見て、「このメッセージを処理しておけば、ずっと描画されない」と勘違いしてました。（；´д⊂）
実際はこのメッセージを処理した時に描画がされないだけで、他の再描画のタイミングがあれば描画されてしまいます。


WM_IME_STARTCOMPOSITION/WM_IME_COMPOSITION/WM_IME_ENDCOMPOSITION
このメッセージは変換中文字列が操作された時に送られてきます。
日本語IMEだとアンダーラインがついてる文字列の事です。 
これらの文字列が「出てきた時/変更された時/無くなった時」にそれぞれ発生します。
で、このメッセージをDefWindowProc()に渡さなければ描画されなくなります。


IMN_OPENSTATUSWINDOW/IMN_CLOSESTATUSWINDOW
ここからはWM_IME_NOTIFYの一部になりますが、この二つのメッセージはツールバーが開く時と閉じる時に飛んできます
なんで、これもDefWindowProc()に渡さないとツールバーが描画されなくなります。
…と思ったら大間違いでした。（；´д⊂）
ツールバーに関連してるのは確かなんですが、
別にこれをDefWindowProc()に渡さなくても普通に描画されるんですよね…。(Win2000 SP4では)
なので、このメッセージを処理する必要があるのかどうか分かりませんが、一応やっておきましょう。


IMN_OPENCANDIDATE/IMN_CHANGECANDIDATE/IMN_CLOSECANDIDATE
これもWM_IME_COMPOSITION系に似たような物で、送られてくるタイミングは
→ IMN_OPENCANDIDATEは変換候補のウィンドウが表示された時  
→ IMN_CHANGECANDIDATEは変換された時（MSIMEだとスペース押した時）  
→ IMN_CLOSECANDIDATEは変換候補のウィンドウが閉じた時  
と、こんな感じになってます。このメッセージもDefWindowProc()に渡さないと
変換候補ウィンドウが描画されなくなったり、 変換文字列が描画されなくなります。 
本来はもう少し場合分けをする必要があるんですが、日本語と韓国のIMEを使ってる限りはこれで問題なかったので無視してます。（￣ー￣）
もし、正確なのが知りたい人はPlatformSDKのサンプルを見ましょう。
ちなみに、これはVisualStudioとは別の物でPlatform SDK Update からダウンロードできます。


*/