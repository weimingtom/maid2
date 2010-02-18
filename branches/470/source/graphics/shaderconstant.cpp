#include"shaderconstant.h"

namespace Maid
{

ShaderConstant::ShaderConstant()
{

}

ShaderConstant::~ShaderConstant()
{

}

void ShaderConstant::Create( size_t Length )
{
  Delete();
  const Graphics::SPDEVICE& pDevice = GlobalPointer<GraphicsCore>::Get()->GetDevice();

  Graphics::CREATEBUFFERPARAM param;

  param.Length = Length;
  param.Usage  = Graphics::RESOURCEUSAGE_DYNAMIC;
  param.BindFlags  = Graphics::RESOURCEBINDFLAG_CONSTANT;
  param.CPUAccessFlags = Graphics::CPUACCESSFLAG_WRITE;
  param.MiscFlags = 0;

  Graphics::SPBUFFER obj = pDevice->CreateBuffer( param, NULL );

  IConstant::Set( obj );

}

void ShaderConstant::Delete()
{
  IConstant::Clear();
}

}

