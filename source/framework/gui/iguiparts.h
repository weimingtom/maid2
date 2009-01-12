#ifndef maid2_framework_gui_iguiparts_h
#define maid2_framework_gui_iguiparts_h


#include"../../config/define.h"
#include"../../config/typedef.h"


#include"guimessage.h"


#include<boost/smart_ptr.hpp>
#include<map>

namespace Maid
{
  class IGUIParts
  {
  public:
    IGUIParts();
    virtual ~IGUIParts(){}

    typedef unt32 ID;
    enum
    {
      PARTSID_NONE   = 0xFFFFFFFF, //  IDが不明
    };

    ID   GetID() const;

    void SendMessage( const IGUIParam& Param );
    bool IsMouseIn( const POINT2DI& pos ) const;

    //  ↓のはＧＵＩパーツなら必ずあるだろうし、挙動が決まってる感じなので、ここに用意してる
    //  本当は継承先ごとに用意すべきだけどね。
    void UpdateFrame();
    void UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth );
    void SetPosition( const POINT2DI& pos );
    void SetEnable ( bool IsEnable ); 
    void SetVisible( bool IsVisible ); 

    POINT2DI GetPosition() const;
    bool IsEnable () const; 
    bool IsVisible() const;


  protected:
    void SendMessageParent( const IGUIParam& Param );

    virtual bool LocalIsMouseIn( const POINT2DI& pos ) const{ return false; }

    virtual void OnSendMessage( const IGUIParam& Param ){}
    virtual void ReciveMessage( ID src, const IGUIParam& Param ){}

    virtual void OnInitialize( const Mouse* pMouse, const Keybord* pKey /*const FEP* pFep */ ){}
    virtual void OnFinalize(){}
    virtual void OnPosition( const POINT2DI& pos ){}
    virtual void OnEnable( bool IsEnable ){}
    virtual void OnVisible( bool IsVisible ){}
    virtual void OnUpdateFrame(){}
    virtual void OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const VECTOR2DI& offset ){}

    const VECTOR2DI& GetOffset() const { return m_Offset; }
    const IGUIParts* GetParent() const { return m_pParent; }
  private:
    ID          m_ID;
    IGUIParts*  m_pParent;
    bool        m_IsEnable;
    bool        m_IsVisible;
    VECTOR2DI   m_Offset;
  };

  typedef boost::shared_ptr<IGUIParts> SPGUIPARTS;

}


#endif
