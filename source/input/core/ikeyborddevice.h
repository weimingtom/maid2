/*!
 	@file
 	@brief	キーボードデバイス
\n			このクラスを継承して各ＯＳごとの機能をラップします
 */

#ifndef maid2_input_core_ikeyborddevice_h
#define maid2_input_core_ikeyborddevice_h

#include"../../config/define.h"
#include<boost/smart_ptr.hpp>
#include<vector>

namespace Maid{ namespace Input {

	/*!
	 	@class	IKeybordDevice ikeyborddevice.h
	 	@brief	キーボードデバイス
	\n			各キーが押されているか調べれるだけなので
	\n			キーボードに対応した文字コードが欲しい場合は ICharCodeDriver から取得すること
	 */

	class IKeybordDevice
	{
	public:
		virtual ~IKeybordDevice()=0{};	//!<	デストラクタ

		//! キーボードドライバの初期化
		/*!
		 */
		virtual void	Initialize()=0;
		//! キーボードドライバの解放
		/*!
		 */
		virtual void		Finalize()=0;

		//! 各ボタンが押されているか調べる
		/*!
		 	@param	KeyList	[ o]	各ボタンの状態
		 
		*/
		virtual void	IsDown( std::vector<bool>& KeyList )	 const=0;

		//! このドライバで識別できるボタンの数を取得する
		/*!
		 	@return ボタンの数
		 */
		virtual int	GetButtonnMax() const=0;
	};

	typedef boost::shared_ptr<IKeybordDevice> SPKEYBORDDEVICE;

}}

#endif