#ifndef maid2_sound_core_idevice_h
#define maid2_sound_core_idevice_h

#include"../../config/define.h"

#include"../common.h"
#include"ibuffer.h"
#include<boost/smart_ptr.hpp>

namespace Maid { namespace Sound {

  /*!
      @class  IDevice idevice.h
      @brief  サウンドデバイス
  */

  class IDevice
	{
	public:
    virtual ~IDevice(){}

    //! デバイスを初期化する
    /*!
     */
		virtual void Initialize()=0;

    //! デバイスを解放する
    /*!
     */
    virtual void Finalize()=0;

    //! 出力フォーマットの設定
    /*!
        @param  format  [i ]  フォーマット
     */
    virtual void SetFormat( const PCMFORMAT& format )=0;


    //! PCMバッファを作成する
    /*!
        @param  param  [i ]  フォーマット
     */
    virtual SPBUFFER CreateBuffer( const CREATEBUFFERPARAM& param )=0;

    //! すでに作成されているPCMバッファを共有する
    /*!
        @param  pSrc  [i ]  共有させたいバッファ
     */
    virtual SPBUFFER DuplicateBuffer( const SPBUFFER& pSrc )=0;

  };

  typedef	boost::shared_ptr<IDevice>	SPDEVICE;

}}


#endif
