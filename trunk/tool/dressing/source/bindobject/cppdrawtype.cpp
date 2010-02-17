#include"stdafx.h"
#include"cppdrawtype.h"

using namespace Sqrat;

void CppDrawType::Register()
{
  RootTable().Bind(
    _SC("CppDrawType"), 
    Class<CppDrawType,NoConstructor>()
  );
}

CppDrawType::~CppDrawType()
{

}

MyApp& CppDrawType::GetApp()
{
  return *GlobalPointer<MyApp>::Get();
}


