/*!
 	@file
 	@brief	キーボード入力を管理するクラス
 */

#ifndef maid2_input_keybord_h
#define maid2_input_keybord_h

#include"../config/define.h"
#include"../config/typedef.h"
#include"core/ikeyborddevice.h"

#include<vector>


namespace Maid
{
	class Keybord
	{
	public:
		Keybord();
		virtual ~Keybord();


    void Initialize( const Input::SPKEYBORDDEVICE& pKey );
		void Finalize();

		void Update();
		void Flash();

		bool	IsDown  ( unt08 KeyCode ) const;
		bool	IsIn    ( unt08 KeyCode ) const;
		bool	IsOut   ( unt08 KeyCode ) const;
		bool	IsRepeat( unt08 KeyCode, unt Frame  ) const;
		bool	IsRepeat( unt08 KeyCode, unt Frame, unt BeginFrame  ) const;

		/*!
		 	@enum	KEYCODE CKeybord.h
		 	@brief	仮想キーコード
		\n			IsDown(), IsClick() の引数に使えます
		\n			各アルファベットは大文字で指定すること 例 'A'
		 */
		enum KEYCODE
		{
			BUTTON_TAB   = 0x09,	//	tab
			BUTTON_ENTER = 0x0D,	//	改行
			BUTTON_SHIFT = 0x10,	//	シフト
			BUTTON_CTRL  = 0x11,	//	Ctrl
			BUTTON_ESC   = 0x1B,	//	エスケープ

			BUTTON_SPACE    = 0x20,	//	スペース
			BUTTON_PAGEUP   = 0x21,	//	ページアップ
			BUTTON_PAGEDOWN = 0x22,	//	ページダウン
			BUTTON_END		= 0x23,	//	最後
			BUTTON_HOME		= 0x24,	//	最初

			BUTTON_LEFT		= 0x25,	//	←
			BUTTON_UP		= 0x26,	//	↑
			BUTTON_RIGHT	= 0x27,	//	→
			BUTTON_DOWN		= 0x28,	//	↓

			BUTTON_INSERT	= 0x2D,	//	挿入
			BUTTON_DELETE	= 0x2E,	//	削除

			BUTTON_NUM0   = 0x30,		//	キーボードの '0'
			BUTTON_NUM1,				//	キーボードの '1'
			BUTTON_NUM2,				//	キーボードの '2'
			BUTTON_NUM3,				//	キーボードの '3'
			BUTTON_NUM4,				//	キーボードの '4'
			BUTTON_NUM5,				//	キーボードの '5'
			BUTTON_NUM6,				//	キーボードの '6'
			BUTTON_NUM7,				//	キーボードの '7'
			BUTTON_NUM8,				//	キーボードの '8'
			BUTTON_NUM9,				//	キーボードの '9'

			BUTTON_TEN0   = 0x60,		//	テンキーの '0'
			BUTTON_TEN1,				//	テンキーの '1'
			BUTTON_TEN2,				//	テンキーの '2'
			BUTTON_TEN3,				//	テンキーの '3'
			BUTTON_TEN4,				//	テンキーの '4'
			BUTTON_TEN5,				//	テンキーの '5'
			BUTTON_TEN6,				//	テンキーの '6'
			BUTTON_TEN7,				//	テンキーの '7'
			BUTTON_TEN8,				//	テンキーの '8'
			BUTTON_TEN9,				//	テンキーの '9'

			BUTTON_F01   = 0x70,	//	F01
			BUTTON_F02,				//	F02
			BUTTON_F03,				//	F03
			BUTTON_F04,				//	F04
			BUTTON_F05,				//	F05
			BUTTON_F06,				//	F06
			BUTTON_F07,				//	F07
			BUTTON_F08,				//	F08
			BUTTON_F09,				//	F09
//			BUTTON_F10 以降はなぜか取得できない （・Д・;）

			BUTTON_MAX=255
		};

	private:
		struct BUTTONSTATE
		{
			BUTTONSTATE() :DownCount(0),IsOut(false){}
			unt		DownCount;
			bool	IsOut;
		};

		std::vector<BUTTONSTATE>  m_ButtonList;
    Input::SPKEYBORDDEVICE    m_pKeybordDevice;
	};

}

#endif