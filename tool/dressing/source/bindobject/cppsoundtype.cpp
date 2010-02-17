#include"stdafx.h"
#include"cppsoundtype.h"

using namespace Sqrat;

void CppSoundType::Register()
{
  RootTable().Bind(
    _SC("CppSoundType"), 
    Class<CppSoundType,NoConstructor>()
  );
}

CppSoundType::~CppSoundType()
{

}

MyApp& CppSoundType::GetApp()
{
  return *GlobalPointer<MyApp>::Get();
}

