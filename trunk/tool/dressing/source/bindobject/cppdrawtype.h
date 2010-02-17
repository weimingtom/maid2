#ifndef tool_dressing_bindobject_cppdrawtype_h
#define tool_dressing_bindobject_cppdrawtype_h


#include"../../../source/auxiliary/mathematics.h"

class MyApp;
class CppDrawType
  : public Maid::GlobalPointer<MyApp>

{
public:
  static void Register();

  virtual ~CppDrawType();
  virtual void Draw( float time, const Maid::POINT3DF& pos )=0;

protected:
  MyApp& GetApp();

};




#endif
