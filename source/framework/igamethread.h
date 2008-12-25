
/*!
    @file
    @brief ゲーム実装用フレームワーク
 */

#ifndef framework_igamethread_h
#define framework_igamethread_h


#include"../config/define.h"
#include"../config/typedef.h"
#include"../auxiliary/string.h"
#include"../auxiliary/mathematics.h"
#include"osmessage.h"
#include"../graphics/core/idevice.h"
#include"../graphics/core/idevicelist.h"
#include"../graphics/core/idrawcommand.h"
#include"../graphics/graphicsobjecttemplate.h"
#include"ifontdevice.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IGameThread 
  {
  public:
    virtual ~IGameThread(){}

    struct DEFAULTCONFIG
    {
      struct GRAPHICS
      {
        int   DeviceID;            //!<  使うデバイスＩＤ
        bool  IsSoftwareVertexProcessing; //!<  頂点変換をソフトウェアで行うか？
        SIZE2DI ScreenSize;

        int DrawCommandCount; //  DeferredRendering に使うコンテキストの数

        //  GraphicCore::SCREENMODE
      };

      String WindowCaption;
      GRAPHICS Graphics;
    };

    bool SelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice );

    struct BEGINPARAM
    {
      std::vector<Graphics::SPDRAWCOMMAND> DrawCommand;
      SPFONTDEVICE  pFontDevice;
//      SPFONTDRIVER		pFontDriver;
//      SPKEYBORDDRIVER		pKeybordDriver;
//      SPMOUSEDRIVER		pMouseDriver;
//      SPCHARCODEDRIVER	pCharCodeDriver;
//      SPINPUTMETHODDRIVER	pIMEDriver;
    };

    void Initialize( const BEGINPARAM& param );
    void UpdateFrame( float now, float delta );
    void UpdateDraw( const IRenderTarget& target, float now, float delta );
    void Finalize();

    enum OSMESSAGESTATE
    {
      OSMESSAGESTATE_RETURN,   //!<	メッセージは処理されたのでこれ以降は呼ばない
      OSMESSAGESTATE_DEFAULT,  //!<	デフォルトの動作を呼び出す
    };
    OSMESSAGESTATE OnOSMessage( const OSMessage::Base& Message );

    bool IsExitGame();
    void OnExitGame();

    const DEFAULTCONFIG& GetDefaultConfig() const;
  protected:
    virtual bool LocalSelectDevice( const std::vector<Graphics::IDeviceList::INFO>& VideoDevice, DEFAULTCONFIG& conf );
    virtual void LocalInitialize( const BEGINPARAM& param );
    virtual void LocalUpdateFrame( float now, float delta );
    virtual void LocalUpdateDraw( const IRenderTarget& target,float now, float delta );
    virtual void LocalFinalize();




    virtual OSMESSAGESTATE LocalOnOSMessage( const OSMessage::Base& Message );

  private:
    DEFAULTCONFIG m_DefaultConfig;
    bool		m_IsExitGame;
  };

  typedef	boost::shared_ptr<IGameThread>	SPGAMETHREAD;
}
#endif
