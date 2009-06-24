#include"samplerstate.h"


namespace Maid
{

SamplerState::SamplerState()
{

}

SamplerState::~SamplerState()
{
  Delete();
}

void SamplerState::Create( const Graphics::SAMPLERSTATEPARAM& state )
{
  Delete();
  const Graphics::SPDEVICE& pDevice = GlobalPointer<GraphicsCore>::Get()->GetDevice();

  Graphics::SPSAMPLERSTATE obj = pDevice->CreateSamplerState( state );
  ISamplerState::Set( obj );
}

void SamplerState::Delete()
{
  ISamplerState::Clear();
}

}