#ifndef maid2_framework_gui_iguicontextmenu_h
#define maid2_framework_gui_iguicontextmenu_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"iguiparts.h"

#include<set>
#include<deque>

namespace Maid
{
  class IGUIContextMenu
    : public IGUIParts
  {
  public:

    class IElement
    {
    public:
      IElement();
      virtual ~IElement(){}

      void Insert( int id, IElement& parts );
      void Delete( int id );
      void DeleteAll();

      void UpdateFrame();
      void UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos );

      //  m_Child の合計サイズを返す
      SIZE2DI CalcChildSize() const;
      RECT2DI CalcChildRect() const;

      struct INFO
      {
        int       ID;
        IElement* pChild;

        INFO(): pChild(NULL), ID(0){}
        INFO( int id, IElement* p ): ID(id), pChild(p){}

        bool operator < ( const INFO& rhs ) const 
        {
          return ID < rhs.ID;
        }
      };

      IElement* FindChild( const POINT2DI& pos );
      const INFO* FindChildInfo( const POINT2DI& pos ) const;

      void SetSelectState( bool on );

      bool IsSelect() const;
      void SetOffset( const VECTOR2DI& off );
      const VECTOR2DI& GetOffset() const;

      bool IsCollision( const POINT2DI& pos ) const;

      const IElement* GetParent() const;
      int GetChildSize() const;

    protected:
      virtual void LocalUpdateFrame(){}
      virtual void LocalUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos ){}
      virtual SIZE2DI GetSize() const=0;


    private:
      void UpdateChildPosition();

    private:

      typedef std::set<INFO> CHILDLIST;
      CHILDLIST   m_Child;
      bool        m_IsSelect;
      VECTOR2DI   m_Offset;
      const IElement* m_pParent;
    };



    IGUIContextMenu();






    void Insert( int id, IElement& parts );
    void Delete( int id );
    void DeleteAll();
 
  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );
    virtual bool LocalIsCollision( const POINT2DI& pos ) const;
    virtual void OnUpdateFrame();
    virtual void OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos );


  private:

    class LabelTop : public IElement
    {
    public:
      virtual SIZE2DI GetSize() const { return SIZE2DI(0,0); }
    };

    LabelTop  m_Top;


    //  表示しているメニュー順 m_SelectList.front() は m_Top です。
    typedef std::deque<IElement*> SELECTLIST;
    SELECTLIST m_SelectList;
  };


}










#endif