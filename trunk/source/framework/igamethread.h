
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
#include"../graphic/core/ivideodevicelist.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IGameThread 
  {
  public:
    virtual ~IGameThread(){}

    struct DEFAULTCONFIG
    {
      struct GRAPHIC
      {
        int   DeviceID;            //!<  使うデバイスＩＤ
        bool  IsSoftwareVertexProcessing; //!<  頂点変換をソフトウェアで行うか？
        SIZE2DI ScreenSize;

        //  GraphicCore::SCREENMODE
      };

      String WindowCaption;
      GRAPHIC Graphic;
    };

    void OnSetup( const std::vector<IVideoDeviceList::INFO>& VideoDevice );

    struct BEGINPARAM
    {
//      CGraphic::FRAMEINFO* pInfo;
//      SPFONTDRIVER		pFontDriver;
//      SPKEYBORDDRIVER		pKeybordDriver;
//      SPMOUSEDRIVER		pMouseDriver;
//      SPCHARCODEDRIVER	pCharCodeDriver;
//      SPINPUTMETHODDRIVER	pIMEDriver;
      int dummy;
    };

    void OnBegin( const BEGINPARAM& param );
    void OnLoop();
    void OnEnd();

    void OnOSMessage( const SPOSMESSAGE& pMess );

//    void OnNextFrame( CGraphic& gra );

    bool IsExitGame();
    void OnExitGame();

    const DEFAULTCONFIG& GetDefaultConfig() const;
  protected:
    virtual void LocalSetup( const std::vector<IVideoDeviceList::INFO>& VideoDevice, DEFAULTCONFIG& conf );
    virtual void LocalBegin( const BEGINPARAM& param );
    virtual void LocalLoop();
    virtual void LocalEnd();



    enum OSMESSAGECALLBACK
    {
      OSMESSAGECALLBACK_RETURN,   //!<	メッセージは処理されたのでこれ以降は呼ばない
      OSMESSAGECALLBACK_DEFAULT,  //!<	デフォルトの動作を呼び出す

    };
    virtual OSMESSAGECALLBACK LocalOnOSMessage( const SPOSMESSAGE& pMess );

  private:
    DEFAULTCONFIG m_DefaultConfig;
    bool		m_IsExitGame;
  };

  typedef	boost::shared_ptr<IGameThread>	SPGAMETHREAD;
}
#endif
