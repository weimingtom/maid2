#ifndef maid2_framework_gui_iguiparts_h
#define maid2_framework_gui_iguiparts_h


#include"../../config/define.h"
#include"../../config/typedef.h"


#include"guimessage.h"


#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IGUIParts;
  typedef boost::shared_ptr<IGUIParts> SPGUIPARTS;

  class IGUIParts
  {
  public:
    IGUIParts();
    virtual ~IGUIParts();

    typedef unt32 ID;
    enum
    {
      PARTSID_NONE   = 0xFFFFFFFF, //  IDが不明
    };

    ID   GetID() const;

    void SendMessage( const IGUIParam& Param );
    bool IsCollision( const POINT2DI& pos ) const;


    //  ↓のはＧＵＩパーツなら必ずあるだろうし、挙動が決まってる感じなので、ここに用意してる
    //  本当は継承先ごとに用意すべきだけどね。
    void UpdateFrame();
    void UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos );
    void SetOffset( const VECTOR2DI& offset );
    void SetEnable ( bool IsEnable ); 
    void SetVisible( bool IsVisible ); 

    const VECTOR2DI& GetOffset() const;
    bool IsEnable () const; 
    bool IsVisible() const;

    IGUIParts* FindChildParts( const POINT2DI& pos );
    bool IsMouseIn() const { return m_IsMouseIn; }

  protected:
    void SendMessageParent( const IGUIParam& Param );

    virtual IGUIParts* LocalFindChildParts( const POINT2DI& pos ) { return NULL; }
    virtual bool LocalIsCollision( const POINT2DI& pos ) const{ return false; }

    enum MESSAGERETURN
    {
      MESSAGERETURN_NONE,       //  このパーツで起きたイベントを親に届けない
      MESSAGERETURN_SENDPARENT, //  親に届ける
    };

    virtual MESSAGERETURN OnSendMessage( const IGUIParam& Param ){ return MESSAGERETURN_SENDPARENT; }
    virtual void ReciveMessage( ID src, const IGUIParam& Param ){}

    virtual void OnUpdateFrame(){}
    virtual void OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos ){}

    virtual MESSAGERETURN OnInitialize( ID id, const IGUIParts* pParent ){ return MESSAGERETURN_SENDPARENT; }
    virtual MESSAGERETURN OnFinalize(){ return MESSAGERETURN_SENDPARENT; }
    virtual MESSAGERETURN OnMouseMove( const POINT2DI& pos ){ return MESSAGERETURN_SENDPARENT; }
    virtual MESSAGERETURN OnMouseIn( const POINT2DI& pos ){ return MESSAGERETURN_SENDPARENT; }
    virtual MESSAGERETURN OnMouseOut( const POINT2DI& pos ){ return MESSAGERETURN_SENDPARENT; }
    virtual MESSAGERETURN OnMouseDown( const POINT2DI& pos, IGUIParam::BUTTONTYPE Button ){ return MESSAGERETURN_SENDPARENT; }
    virtual MESSAGERETURN OnMouseUp( const POINT2DI& pos, IGUIParam::BUTTONTYPE Button ){ return MESSAGERETURN_SENDPARENT; }

    const IGUIParts* GetParent() const { return m_pParent; }
    VECTOR2DI CalcTotalOffset() const;

  private:
    ID          m_ID;
    IGUIParts*  m_pParent;
    bool        m_IsEnable;
    bool        m_IsVisible;
    VECTOR2DI   m_Offset;
    bool        m_IsMouseIn;
  };

  typedef boost::shared_ptr<IGUIParts> SPGUIPARTS;

}


#endif
