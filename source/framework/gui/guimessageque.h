#ifndef maid2_framework_gui_guimessageque_h
#define maid2_framework_gui_guimessageque_h

#include"../../config/define.h"

#include"guimessage.h"
#include"iguiparts.h"
#include<vector>

namespace Maid
{
  class GUIMessageQue
  {
  public:

    virtual ~GUIMessageQue();

    void PostMessage( IGUIParts* pTarget, const IGUIParam& Param );

    void Execute();

  private:
    struct MESSAGE
    {
      IGUIParts*  pTarget;
      SPGUIPARAM  pParam;
    };
    typedef std::vector<MESSAGE> MESSAGEQUE;

    MESSAGEQUE m_Que;
  };

}


#endif