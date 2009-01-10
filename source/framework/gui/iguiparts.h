#ifndef maid2_framework_gui_iguiparts_h
#define maid2_framework_gui_iguiparts_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"../../input/mouse.h"
#include"../../input/keybord.h"
#include"../../graphics/rendertargetbase.h"
#include"../../graphics/depthstencil.h"

#include"guimessage.h"


#include<boost/smart_ptr.hpp>
#include<map>

namespace Maid
{
  class IGUIParts;
  class GUIManager;
  typedef boost::shared_ptr<IGUIParts> SPGUIPARTS;
  class IGUIParts
  {
    friend GUIManager;
  public:
    IGUIParts();
    virtual ~IGUIParts(){}

    typedef unt32 ID;
    enum
    {
      PARTSID_NONE   = 0xFFFFFFFF, //  IDが不明
    };

    void SetParent( IGUIParts* pParent );

    ID   GetID() const;

    void SendMessage( const IGUIParam& Param );
    void UpdateFrame();
    void UpdateDraw( RenderTargetBase& Target, IDepthStencil& Depth );

    //  ↓の３つはＧＵＩパーツなら必ずあるだろうし、挙動が決まってる感じなので、ここに用意してる
    //  本当は継承先ごとに用意すべきだけどね。
    void SetPosition( const POINT2DI& pos );
    void SetEnable ( bool IsEnable ); 
    void SetVisible( bool IsVisible ); 

    POINT2DI GetPosition() const;
    bool IsEnable () const; 
    bool IsVisible() const;

    bool IsMouseIn( const POINT2DI& pos ) const;

  protected:
    void SendMessageParent( const IGUIParam& Param );

    virtual void LocalSendMessage( const IGUIParam& Param ){}
    virtual void ReciveMessage( ID src, const IGUIParam& Param ){}
    virtual void LocalUpdateFrame(){}
    virtual void LocalUpdateDraw( RenderTargetBase& Target, IDepthStencil& Depth, const VECTOR2DI& pos ){}
    virtual bool LocalIsMouseIn( const POINT2DI& pos ) const{ return false; }

    virtual void OnInitialize( const Mouse* pMouse, const Keybord* pKey /*const FEP* pFep */ ){}
    virtual void OnFinalize(){}
    virtual void OnPosition( const POINT2DI& pos ){}
    virtual void OnEnable( bool IsEnable ){}
    virtual void OnVisible( bool IsVisible ){}


    const VECTOR2DI& GetOffset() const;

  protected:
    ID          m_ID;
    IGUIParts*  m_pParent;
    bool        m_IsEnable;
    bool        m_IsVisible;
    VECTOR2DI   m_Offset;
  };

  typedef boost::shared_ptr<IGUIParts> SPGUIPARTS;

}


#endif
