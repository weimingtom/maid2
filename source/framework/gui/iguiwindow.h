#ifndef maid2_framework_gui_iguiwindow_h
#define maid2_framework_gui_iguiwindow_h


#include"../../config/define.h"
#include"iguiparts.h"
#include"iguibutton.h"


namespace Maid
{
  class IGUIWindow : public IGUIParts
  {
  public:
    IGUIWindow();

    void SetMinimum( bool IsMin );
    bool IsMinimum() const;

    enum STATE
    {
      STATE_NORMAL,             //  普通の状態
      STATE_MOVING,             //  バーをもって移動中
      STATE_MINIMUMBUTTONDOWN,  //  最小化中
      STATE_HIDEBUTTONDOWN,     //  消えています
    };

    STATE GetState() const;

  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );
    virtual bool LocalIsCollision( const POINT2DI& pos ) const;

    virtual bool IsBarCollision( const POINT2DI& pos ) const = 0;
    virtual bool IsClientCollision( const POINT2DI& pos ) const = 0;
    virtual bool IsMinimumButtonCollision( const POINT2DI& pos ) const;
    virtual bool IsHideButtonCollision( const POINT2DI& pos ) const;
 
    virtual void OnChangeState( const POINT2DI& pos, STATE state );
    virtual void OnMinimum( bool IsMin );

  private:
    void ChangeState( const POINT2DI& pos, STATE state );
  private:
    bool m_IsMinimum;
    STATE m_State;

    POINT2DI  m_PrevMovePosition; //  前フレーム時にあったカーソル位置
  };


}
#endif