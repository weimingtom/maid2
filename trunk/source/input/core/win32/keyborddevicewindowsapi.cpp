#include"keyborddevicewindowsapi.h"
#include"../../../auxiliary/macro.h"

// 仮想キーコードは↓を参照
// http://yokohama.cool.ne.jp/chokuto/urawaza/prm/virtual_key_code.html


namespace Maid{ namespace Input {

	/*!
	 	@class	KeybordDeviceWindowsAPI keyborddevicewindowsapi.h
	 	@brief	キーボードドライバ
	 */ 

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
KeybordDeviceWindowsAPI::KeybordDeviceWindowsAPI()
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
KeybordDeviceWindowsAPI::~KeybordDeviceWindowsAPI()
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! キーボードドライバの初期化
/*!
 	引数などは IKeybordDevice::Initialize を参照すること
 */
void	KeybordDeviceWindowsAPI::Initialize()
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! キーボードドライバの解放
/*!
 	引数などは IKeybordDevice::Finalize を参照すること
 */
void  KeybordDeviceWindowsAPI::Finalize()
{

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 各ボタンが押されているか調べる
/*!
 	引数などは IKeybordDevice::IsDown を参照すること
 */
void KeybordDeviceWindowsAPI::IsDown( std::vector<bool>& KeyList )	    const
{
  for( int i=0; i<256; ++i )
  {
    KeyList[i] = IsFlag(::GetAsyncKeyState(i),0x8000);
  }
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! このドライバで識別できるボタンの数を取得する
/*!
 *	引数などは IKeybordDevice::GetButtonnMax を参照すること
 */
int	KeybordDeviceWindowsAPI::GetButtonnMax()	const
{
	return 256;
}



}}