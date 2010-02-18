#ifndef maid2_sound_core_idevicelist_h
#define maid2_sound_core_idevicelist_h

#include"../../config/define.h"
#include"../../auxiliary/string.h"

#include<boost/smart_ptr.hpp>
#include<vector>
#include"idevice.h"

namespace Maid { namespace Sound {

  class IDeviceList
	{
	public:
    virtual ~IDeviceList(){}

    //! 初期化
		/*!
		 */
		virtual void Initialize()=0;

    struct INFO
    {
      INFO( int id, const String& name ): DeviceID(id),Name(name){}
      int     DeviceID; //!<  デバイスＩＤ
      String  Name;     //!<  デバイスの名前
    };

    //! 使えるサウンドデバイスの情報を取得する
    /*!
        @param  info  [ o]  設定先
     */
    virtual void GetList( std::vector<INFO>& info )=0;

    //! サウンドデバイスの作成
		/*!
		 	@return サウンドデバイス
		 */
		virtual SPDEVICE CreateDevice( int DeviceID )=0;

  };


}}


#endif
