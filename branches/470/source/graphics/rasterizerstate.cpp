#include"rasterizerstate.h"


namespace Maid
{

RasterizerState::RasterizerState()
{

}

RasterizerState::~RasterizerState()
{
  Delete();
}

void RasterizerState::Create( const Graphics::RASTERIZERSTATEPARAM& state )
{
  Delete();
  const Graphics::SPDEVICE& pDevice = GlobalPointer<GraphicsCore>::Get()->GetDevice();

  Graphics::SPRASTERIZERSTATE obj = pDevice->CreateRasterizerState( state );
  IRasterizerState::Set( obj );
}

void RasterizerState::Delete()
{
  IRasterizerState::Clear();
}

}