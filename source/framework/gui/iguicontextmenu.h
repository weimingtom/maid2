#ifndef maid2_framework_gui_iguicontextmenu_h
#define maid2_framework_gui_iguicontextmenu_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"iguiparts.h"


namespace Maid
{
  class IGUIContextMenu
    : public IGUIParts
  {
  public:

    class ILabel
    {
    public:

      void Insert( int id, ILabel& parts );
      void Delete( int id );
      void DeleteAll();

      virtual void UpdateFrame(){}
      virtual void UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth ){}
      virtual SIZE2DI GetSize() const=0;

      //  m_Child の合計サイズを返す
      SIZE2DI GetChildSize() const;

    private:
      struct INFO
      {
        ILabel* pLabel;
        int     ID;
      };
      typedef std::list<INFO> CHILDLIST;
      CHILDLIST  m_Child;
    };

    IGUIContextMenu();

    void Insert( int id, ILabel& parts );
    void Delete( int id );
    void DeleteAll();
 
  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );


  private:

    class LabelTop : public ILabel
    {
    public:
      virtual SIZE2DI GetSize() const { return SIZE2DI(0,0); }
    };

    LabelTop  m_Top;
  };


}










#endif