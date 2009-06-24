/*!
    @file
    @brief ビデオカードの列挙を行うベースクラス
 */

#ifndef maid2_graphics_core_idevicelist_h
#define maid2_graphics_core_idevicelist_h


#include"../../config/define.h"
#include"../../auxiliary/macro.h"
#include"../../auxiliary/string.h"

#include"idevice.h"
#include"ifontdevice.h"


namespace Maid { namespace Graphics {

  class IDeviceList
  {
  public:
    virtual ~IDeviceList(){}

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
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

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 使えるビデオデバイスの情報を取得する
    /*!
        @param  info  [ o]  設定先
     */
    virtual void GetList( std::vector<INFO>& info )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 指定したビデオデバイスを作成する
    /*!
        @param  No  [i ]  作成したいID

        @return	作成されたビデオデバイス
     */
    virtual SPDEVICE Create( int DeviceID ) =0;


    virtual SPFONTDEVICE CreateFontDevice()=0;
  };

  typedef	boost::shared_ptr<IDeviceList>	SPDEVICELIST;

}}


#endif
