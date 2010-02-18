#ifndef maid2_framework_gui_iguiparts_h
#define maid2_framework_gui_iguiparts_h


#include"../../config/define.h"
#include"../../config/typedef.h"


#include"../../graphics/rendertargetbase.h"
#include"../../graphics/depthstencil.h"
#include"guimessage.h"


#include<list>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class GUIMessageQue;
  class IGUIParts
  {
  public:
    IGUIParts();
    virtual ~IGUIParts();

    typedef unt32 ID;
    enum
    {
      PARTSID_NONE   = 0xFFFFFFFF, //  IDが不明
      PARTSID_LOCAL  = 0xFFFF0000, //  この値以降は継承クラス専用のパーツ
    };

    ID   GetID() const;

    void SendMessage( const IGUIParam& pParam );
    bool IsCollision( const POINT2DI& pos ) const;


    //  ↓のはＧＵＩパーツなら必ずあるだろうし、挙動が決まってる感じなので、ここに用意してる
    //  本当は継承先ごとに用意すべきだけどね。
    void UpdateFrame();
    void UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth );
    void SetPosition( const POINT2DI& pos );
    void MovePosition( const VECTOR2DI& offset );
    void SetEnable ( bool IsEnable ); 
    void SetVisible( bool IsVisible ); 

    const POINT2DI& GetPosition() const;
    bool IsEnable () const; 
    bool IsVisible() const;

    bool IsExistChildParts( IGUIParts::ID id )const;
    const IGUIParts& GetChildParts( IGUIParts::ID id )const;

    typedef std::list<IGUIParts*> CHILDLIST;
    const CHILDLIST& GetChildList() const;

    void AddChildParts( IGUIParts::ID id, IGUIParts& Parts );
    void DelChildParts( IGUIParts::ID id );
    void DelChildAll();

    enum ZORDER
    {
      ZORDER_FRONTLOCK=0, //  常に手前
      ZORDER_FRONT,       //  手前
      ZORDER_BACK,        //  奥
      ZORDER_BACKLOCK,    //  常に奥
    };
    void SetZOrder( ZORDER type );
    ZORDER GetZOrder()const;
    void ChangeChildZOrder( IGUIParts::ID id, ZORDER type );

    bool IsMouseIn() const;
    bool IsKeybordFocus() const;

    void SetMessageQue( GUIMessageQue* pQue );
  protected:
    void LocalAddChildParts( IGUIParts::ID id, IGUIParts& Parts );

  protected:
    enum MESSAGERETURN
    {
      MESSAGERETURN_NONE,       //  このパーツで起きたイベントを親に届けない
      MESSAGERETURN_SENDPARENT, //  親に届ける
    };

    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );

    virtual bool LocalIsCollision( const POINT2DI& pos ) const;
    virtual void ReciveMessage( ID src, const IGUIParam& Param );
    virtual void OnUpdateFrame();
    virtual void OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos );

    virtual void OnInitialize( ID id, const IGUIParts& Parent );
    virtual void OnFinalize();
    virtual void OnSetPosition( const POINT2DI& pos );
    virtual void OnMovePosition( const VECTOR2DI& vec );
    virtual void OnEnable( bool IsEnable );
    virtual void OnVisible( bool IsVisible );
    virtual void OnMouseMove( const POINT2DI& pos );
    virtual void OnMouseIn( const POINT2DI& pos );
    virtual void OnMouseOut( const POINT2DI& pos );
    virtual void OnKeybordFocus( bool in );

    const IGUIParts* GetParent() const { return m_pParent; }

    void PostMessage( const IGUIParam& Param );
    POINT2DI CalcLocalPosition( const POINT2DI& pos ) const;

    IGUIParts& _GetChildParts( IGUIParts::ID id );

  private:
    void  EraseChildList( IGUIParts& Parts );
    void  InsertChildList( IGUIParts& Parts );

  private:
    ID          m_ID;
    IGUIParts*  m_pParent;
    bool        m_IsEnable;
    bool        m_IsVisible;
    POINT2DI    m_Position;
    bool        m_IsMouseIn;
    bool        m_IsKeybordFocus;
    GUIMessageQue*  m_pPostTarget;

    CHILDLIST m_ChildList;  //  m_ChildList.begin()...最前面 .end() 最背面
    ZORDER    m_ZOrder;  //  最前面か？
  };

  typedef boost::shared_ptr<IGUIParts> SPGUIPARTS;

}


#endif
