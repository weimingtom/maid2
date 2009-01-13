#ifndef maid2_framework_gui_guiradiogroup_h
#define maid2_framework_gui_guiradiogroup_h


#include"../../config/define.h"
#include"iguiparts.h"
#include"guiradiobutton.h"

#include<vector>

namespace Maid
{
  class GUIRadioGroup : public IGUIParts
  {
  public:

    virtual ~GUIRadioGroup();

    enum
    {
      SELECT_NONE=-1, //  選択されていない
    };

    void SetRadioButton( GUIRadioButton* Data, size_t Length );
    void SetRadioButton( const std::vector<GUIRadioButton*>& RadioButtonList );

    void Clear();

    int GetCheckNo() const;
    void SetCheck( int pos );

  protected:
    IGUIParts* LocalFindChildParts( const POINT2DI& pos );
    virtual void ReciveMessage( ID src, const IGUIParam& Param );
    virtual void OnUpdateFrame();
    virtual void OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& offset );

    virtual MESSAGERETURN OnGroupMouseMove( int no, const POINT2DI& pos ){ return MESSAGERETURN_SENDPARENT; }
    virtual MESSAGERETURN OnGroupMouseIn( int no, const POINT2DI& pos ){ return MESSAGERETURN_SENDPARENT; }
    virtual MESSAGERETURN OnGroupMouseOut( int no, const POINT2DI& pos ){ return MESSAGERETURN_SENDPARENT; }

    virtual MESSAGERETURN OnGroupMouseDown( int no, const POINT2DI& pos, int Button ){ return MESSAGERETURN_SENDPARENT; }
    virtual MESSAGERETURN OnGroupMouseUp( int no, const POINT2DI& pos, int Button ){ return MESSAGERETURN_SENDPARENT; }

    virtual MESSAGERETURN OnChangeSelect( int OldNo, int NewNo ){ return MESSAGERETURN_SENDPARENT; }

  private:
    std::vector<GUIRadioButton*> m_Button;
    int m_SelectNo;
  };


}
#endif