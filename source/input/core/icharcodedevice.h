#ifndef maid2_input_core_icharcodedevice_h
#define maid2_input_core_icharcodedevice_h

#include"../../config/define.h"
#include"../../auxiliary/string.h"

#include<boost/smart_ptr.hpp>
#include<string>

namespace Maid{ namespace Input {

  /*!
    @class	ICharCodeDevice icharcodedevice.h
    @brief	文字コード取得ドライバ
  \n			たとえば キーボードの A を押した場合
  \n			a, A, ﾁ の可能性があるわけですが、それを識別してくれるクラス
   */

  class ICharCodeDevice
  {
  public:
    virtual ~ICharCodeDevice(){};

    //! 初期化処理
    /*!
     */
    virtual void  Initialize()=0;

    //! 解放処理
    /*!
     */
    virtual void  Finalize()=0;


    //! バッファにたまっている文字コードを全部拾う
    /*!
        溜め込んだ文字が全部飛びます。

 	    @param	buffer	[ o]	前回Flush() してから今回までにたまった文字コード
     */
    virtual void Flush( String& buffer )=0;
  };


	typedef boost::shared_ptr<ICharCodeDevice> SPCHARCODEDEVICE;

}}

#endif
