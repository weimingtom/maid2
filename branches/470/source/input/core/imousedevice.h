/*!
 	@file
 	@brief	マウスデバイス
\n			このクラスを継承して各ＯＳごとの機能をラップします
 */

#ifndef maid2_input_core_imousedevice_h
#define maid2_input_core_imousedevice_h

#include"../../config/define.h"
#include<vector>
#include<boost/smart_ptr.hpp>

namespace Maid{ namespace Input {
  /*!
    @class	IMouseDevice imousedevice.h
    @brief	マウスドライバ
  \n			メンバ関数の指定引数 Index は
  \n			0..Ｘ軸   1..Ｙ軸   2..ホイール  3以降..拡張マウス
  \n			になっています
   */

  class IMouseDevice
  {
  public:
    virtual ~IMouseDevice(){};	//!<	デストラクタ

    //! マウスドライバの初期化
    /*!
      @return 初期化に成功したら MRESULT_OK
    \n			初期化に失敗したら MRESULT_OK 以外
     */
    virtual void  Initialize()=0;
    //! マウスドライバの解放
    /*!
     */
    virtual void  Finalize()=0;

    //! 各軸の座標および各ボタンが押されているかを取得する
    /*!
      @param	PosList	[ o]	各軸の状態
      @param	KeyList	[ o]	各ボタンの状態
     */
    virtual void  GetState( std::vector<int>& PosList, std::vector<bool>& KeyList ) const=0;


    //! このドライバで識別できる軸の数を取得する
    /*!
      @return 軸の数
     */
    virtual int	GetPositionMax() const=0;

    //! このドライバで識別できるボタンの数を取得する
    /*!
      @return ボタンの数
     */
    virtual int	GetButtonMax() const=0;
  };

  typedef boost::shared_ptr<IMouseDevice> SPMOUSEDEVICE;

}}

#endif