/*!
 	@file
 	@brief IME ラッパクラス
 */

#ifndef maid2_input_core_win32_inputmethoddevicewindows_h
#define maid2_input_core_win32_inputmethoddevicewindows_h

#include"../../../config/define.h"
#include"../iinputmethoddevice.h"

#include"../../../frameWork/win32/messagehook.h"
#include"../../../framework/win32/window.h"


namespace Maid{ namespace Input {

	class InputMethodDeviceWindows 
    : public IInputMethodDevice
    , MessageHook
	{
	public:
		InputMethodDeviceWindows( const Window& hWnd );
		virtual ~InputMethodDeviceWindows();
		virtual void Initialize();
		virtual void Finalize();

		void GetStatus( DWORD* pConversion, DWORD* pSentence) const;

		virtual void Open() ;
		virtual void Close();

		virtual bool IsOpen() const;
		virtual int GetConvertPos() const;
		virtual int GetCursorPos() const;


		virtual int GetCandidateCount() const;
		virtual int GetCandidateSelect() const;
		virtual String GetCandidateString(int iIndex) const;
		virtual String GetCompString() const;

    virtual void Flush( String& buffer );

		virtual RETURNCODE OnMessage( WindowsMessage& Param );

	private:
		void UpdateCandidate();
		
		const Window& m_Window;
    String  m_EnterString;  //  確定した結果

		LPCANDIDATELIST m_pCandidateList;
		boost::shared_array<unt08>	m_pCandidateBuff;
	};
}}

#endif
