/*!
 	@file
 	@brief	入力ドライバの親 オブジェクト
 */

#ifndef maid2_input_core_idevicelist_h
#define maid2_input_core_idevicelist_h

#include"../../config/define.h"
#include"../../auxiliary/string.h"
#include"imousedevice.h"
#include"ikeyborddevice.h"
#include"icharcodedevice.h"
#include"iinputmethoddevice.h"


namespace Maid{ namespace Input {

	/*!
	 	@class	IDeviceList idevicelist.h
	 	@brief	入力ドライバクラス
	 */
	class IDeviceList
	{
	public:
		virtual ~IDeviceList(){}

    //! 初期化
		/*!
		 */
		virtual void Initialize()=0;

    struct KEYBORDINFO
    {
      int     DeviceID; //!<  デバイスＩＤ
      String  Name;     //!<  デバイスの名前
    };

    //! 使えるキーボードデバイスの情報を取得する
    /*!
        @param  info  [ o]  設定先
     */
    virtual void GetKeybordList( std::vector<KEYBORDINFO>& info )=0;

    //! キーボードドライバの作成
		/*!
		 	@return キーボードドライバ
		 */
		virtual SPKEYBORDDEVICE CreateKeybordDevice( int DeviceID )=0;

    struct MOUSEINFO
    {
      int     DeviceID; //!<  デバイスＩＤ
      String  Name;     //!<  デバイスの名前
    };

    //! 使えるマウスデバイスの情報を取得する
    /*!
        @param  info  [ o]  設定先
     */
    virtual void GetMouseList( std::vector<MOUSEINFO>& info )=0;

    //! マウスデバイスの作成
		/*!
		 	@return マウスデバイス
		 */
		virtual SPMOUSEDEVICE   CreateMouseDevice( int DeviceID )=0;

		//! 文字コードデバイスの作成
		/*!
		 	@return 文字コードデバイス
		 */
		virtual SPCHARCODEDEVICE   CreateCharCodeDevice()=0;

		//! IMデバイスの作成
		/*!
		 	@return IMデバイス
		 */
		virtual SPINPUTMETHODDEVICE   CreateInputMethodDevice()=0;

	};


}}

#endif