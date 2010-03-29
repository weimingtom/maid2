#include"stdafx.h"
#include"cppmatrix.h"

using namespace Maid;
using namespace Sqrat;


void CppMatrix::Register()
{
  RootTable().Bind(
    _SC("CppMatrix"), 
    Class<CppMatrix>()
    .Var(_SC("m11"), &CppMatrix::m11)
    .Var(_SC("m12"), &CppMatrix::m12)
    .Var(_SC("m13"), &CppMatrix::m13)
    .Var(_SC("m14"), &CppMatrix::m14)

    .Var(_SC("m21"), &CppMatrix::m21)
    .Var(_SC("m22"), &CppMatrix::m22)
    .Var(_SC("m23"), &CppMatrix::m23)
    .Var(_SC("m24"), &CppMatrix::m24)

    .Var(_SC("m31"), &CppMatrix::m31)
    .Var(_SC("m32"), &CppMatrix::m32)
    .Var(_SC("m33"), &CppMatrix::m33)
    .Var(_SC("m34"), &CppMatrix::m34)

    .Var(_SC("m41"), &CppMatrix::m41)
    .Var(_SC("m42"), &CppMatrix::m42)
    .Var(_SC("m43"), &CppMatrix::m43)
    .Var(_SC("m44"), &CppMatrix::m44)
  );
}

const MATRIX4DF& CppMatrix::GetMatrix() const
{
  const MATRIX4DF* p = (MATRIX4DF*)&m11;

  return *p;
}


