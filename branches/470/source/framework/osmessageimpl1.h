#ifndef maid2_framework_osmessageimpl1_h
#define maid2_framework_osmessageimpl1_h

#include"../config/define.h"
#include"../config/typedef.h"
#include"../auxiliary/string.h"
#include"osmessage.h"


namespace Maid
{
  namespace OSMessage
  {
    struct CharCode : public Base
    {
      CharCode() : Base(CHARACODE){}

      unt32	Code;
    };

    struct ImeResult : public Base
    {
      ImeResult() : Base(IMERESULT){}

      String	Code;
    };

    struct Activate : public Base
    {
      Activate() : Base(ACTIVATE){}

      bool	IsActive;
    };
  }

}


#endif
