#ifndef framework_osmessageimpl1_h
#define framework_osmessageimpl1_h

#include"../config/define.h"
#include"../config/typedef.h"
#include"../auxiliary/string.h"
#include"osmessage.h"


namespace Maid
{
  namespace OSMessage
  {
    class CharCode : public Base
    {
    public:
      CharCode() : Base(CHARACODE){}

      unt32	Code;
    };

    class ImeResult : public Base
    {
    public:
      ImeResult() : Base(IMERESULT){}

      String	Code;
    };

    class Activate : public Base
    {
    public:
      Activate() : Base(ACTIVATE){}

      bool	IsActive;
    };
  }

}


#endif
