#ifndef maid2_framework_gui_guicallback_h
#define maid2_framework_gui_guicallback_h


#include"../../config/define.h"
#include"iguiparts.h"
#include<boost/function.hpp>

namespace Maid
{


  class GUICallback : public IGUIParts
  {
  public:
    typedef boost::function2<void, IGUIParts::ID, const IGUIParam&>	RECIVEFUNCTION;
    void SetReviceFunction( const RECIVEFUNCTION& Func );

  protected:
    void ReciveMessage( ID src, const IGUIParam& Param );

  private:
    RECIVEFUNCTION  m_Func;
  };

}
#endif