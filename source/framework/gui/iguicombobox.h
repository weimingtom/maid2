#ifndef maid2_framework_gui_iguicombobox_h
#define maid2_framework_gui_iguicombobox_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"iguiparts.h"

#include<map>

namespace Maid
{
  class IGUIComboBox 
    : public IGUIParts
  {
  public:

    class IElement
    {
    public:
      IElement(){}
      virtual ~IElement(){}

      virtual void UpdateFrame(){}

      virtual void DrawNormal( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos ){}
      virtual void DrawMouseIn( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos ){}
      virtual void DrawSelect( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos ){}

      virtual bool IsCollision( const POINT2DI& pos ) const{return false;}
      virtual SIZE2DI GetBoxSize() const { return SIZE2DI(0,0); }
    };

    IGUIComboBox();
    virtual ~IGUIComboBox(){}

    void Insert( int id, IElement& parts );
    void Delete( int id );
    void DeleteAll();

    enum STATE
    {
      STATE_NORMAL,            //  普通の状態
      STATE_SELECTING,         //  リストを表示して選択中
      STATE_SLIDERBUTTONDOWN,  //  スライダー押してる
    };

    STATE GetState() const;

    void SetSelectID( int id );
    int GetSelectID() const;

    void SetSelectListMax( int count );


  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );
    virtual bool LocalIsCollision( const POINT2DI& pos ) const;

    virtual bool IsBoxCollision( const POINT2DI& pos ) const=0;
    virtual bool IsSliderCollision( const POINT2DI& pos ) const=0;
    virtual bool IsSliderButtonCollision( const POINT2DI& pos ) const=0;


    void DrawElement( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos );

  private:
    void ChangeState( STATE s );

    IElement& GetSelectElement();

  private:
    STATE m_State;

    typedef std::map<int,IElement*> ELEMENTLIST;
    ELEMENTLIST m_ElementList;

    int m_SelectListMax;

    int m_SelectID;

    IElement  m_Tmp;
  };
}

#endif
