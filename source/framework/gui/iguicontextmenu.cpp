#include"iguicontextmenu.h"

namespace Maid
{

  IGUIContextMenu::IGUIContextMenu()
  {

  }

  void IGUIContextMenu::Insert( int id, ILabel& parts )
  {

  }

  void IGUIContextMenu::Delete( int id )
  {

  }

  void IGUIContextMenu::DeleteAll()
  {

  }
 
/*
 class IGUIContextMenu
    : public IGUIParts
  {
  public:

    class ILabel
      : public IGUIParts
    {
    public:

      void Insert( int id, ILabel& parts );
      void Delete( int id );
      void DeleteAll();

      virtual SIZE2DI GetSize() const=0;
    private:
      typedef std::list<ILabel*> CHILDLIST;
      CHILDLIST  m_Child;
    };

    IGUIContextMenu();

    void Insert( int id, ILabel& parts );
    void Delete( int id );
    void DeleteAll();
 
  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );


  private:
      typedef std::list<ILabel*> CHILDLIST;
      CHILDLIST  m_Child;
  };
*/
}