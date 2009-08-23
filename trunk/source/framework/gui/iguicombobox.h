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
      STATE_SELECTING,         //  選択リストを表示中
      STATE_SELECTINGDOWN,     //  選択リストを表示して、マウス押してる
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
    virtual int GetBoxHeight() const=0;

    virtual void DrawSlider( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos ){}

    void DrawElement( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos );

    int CalcSliderButtonOffset() const;

  private:
    typedef std::map<int,IElement*> ELEMENTLIST;
    void ChangeState( STATE s );
    int  CalcSliderHeight() const;

    IElement& GetSelectElement();

    ELEMENTLIST::const_iterator GetSliderTopIte()const;

  private:
    STATE m_State;

    ELEMENTLIST m_ElementList;

    int m_SelectListMax;  //  選択項目を何個表示するか？
    int m_SliderTop;  //  スライダの値(最上位に表示しているelementが何個目のやつか？)

    int m_SelectID;   //  現在選択されているＩＤ
    int m_MouseInID;  //  項目選択時にマウスが乗っかっているelementのＩＤ

    IElement  m_Tmp;  //  各種取得関数でエラった時に返すテンポラリ要素
  };
}

#endif
