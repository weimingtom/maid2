#include"inputlayout.h"


namespace Maid
{

InputLayout::InputLayout()
{

}

InputLayout::~InputLayout()
{
  Delete();
}

void InputLayout::Create( const Graphics::IDevice::INPUT_ELEMENT* pElement, int Count,  const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )
{
  const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();

  Graphics::hInputLayout obj = pDevice->CreateInputLayout( pElement, Count, pShaderBytecodeWithInputSignature, BytecodeLength );
  IInputLayout::Set( obj );
}

void InputLayout::Delete()
{
  const Graphics::hInputLayout& obj = IInputLayout::Get();
  if( obj.empty() ) { return ; }

  const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();

  pDevice->DeleteObject( obj );
  IInputLayout::Clear();
}

}