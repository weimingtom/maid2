#ifndef maid2_framework_gui_iguicontextmenu_h
#define maid2_framework_gui_iguicontextmenu_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"iguiparts.h"


namespace Maid
{
  class IGUIContextMenuChild
    : public IGUIParts
  {
  public:

    virtual SIZE2DI GetSize() const=0;
  private:

  };

  class IGUIContextMenu
    : public IGUIParts
  {
  public:
    IGUIContextMenu();

    void Insert( unt id, int pos, bool IsDirectory, IGUIParts& parts );
    void Delete( unt id );
    void DeleteAll();
 
  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );


  private:
  };


}










#endif