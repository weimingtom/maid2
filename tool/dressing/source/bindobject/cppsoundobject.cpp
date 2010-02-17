#include"stdafx.h"

#include"cppsoundobject.h"


using namespace Maid;
using namespace Sqrat;

void CppSoundObject::Register()
{
  RootTable().Bind(
    _SC("CppSoundObject"), 
    ClassEx<CppSoundObject,CppSoundObjectAllocator>()
	  .SqObject(_SC("Type"), &CppSoundObject::Type)
    .Prop(_SC("Play"), &CppSoundObject::GetPlay, &CppSoundObject::SetPlay)
    .Prop(_SC("Volume"), &CppSoundObject::GetVolume, &CppSoundObject::SetVolume)
    .Prop(_SC("Position"), &CppSoundObject::GetPosition, &CppSoundObject::SetPosition)
  );
}



CppSoundObject::CppSoundObject()
{
  HSQUIRRELVM v = DefaultVM::Get();
  MAID_ASSERT( v==NULL, "VMが設定されていません" );
  sq_resetobject( &Type );
}

CppSoundObject::~CppSoundObject()
{
  HSQUIRRELVM v = DefaultVM::Get();
  MAID_ASSERT( v==NULL, "VMが設定されていません" );
  const SQBool type = sq_release( v, &Type );
}



bool CppSoundObject::GetPlay() const
{
  return GetType()->IsPlay();
}

void CppSoundObject::SetPlay(const bool& type)
{
  GetType()->Play(type);
}

float CppSoundObject::GetVolume() const
{
  return GetType()->GetVolume();
}

void CppSoundObject::SetVolume(const float& vol)
{
  GetType()->SetVolume(vol);
}

float CppSoundObject::GetPosition() const
{
  return GetType()->GetPosition();
}

void CppSoundObject::SetPosition(const float& time)
{
  GetType()->SetPosition(time);
}


CppSoundType*  CppSoundObject::GetType()
{
  HSQOBJECT& obj = const_cast<CppSoundObject*>(this)->Type;

  CppSoundType* p  = (CppSoundType*)sq_objtoinstance(&obj);

  return p;
}

const CppSoundType*  CppSoundObject::GetType()const
{
  HSQOBJECT& obj = const_cast<CppSoundObject*>(this)->Type;

  const CppSoundType* p  = (const CppSoundType*)sq_objtoinstance(&obj);

  return p;
}