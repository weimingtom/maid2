#include"blendstate.h"


namespace Maid
{

BlendState::BlendState()
{

}

BlendState::~BlendState()
{
  Delete();
}

void BlendState::Create( TEMPLATE t )
{
  Graphics::BLENDSTATEPARAM param;

  switch( t )
  {
  case TEMPLATE_NORMAL:
    {
      param.Target[0].Enable = false;
    }break;

  case TEMPLATE_ALPHA:
    {
      param.Target[0].Enable = true;
      param.Target[0].ColorSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_SRC_ALPHA;
      param.Target[0].ColorDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_INV_SRC_ALPHA;
      param.Target[0].ColorOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_ADD;
    }break;

  case TEMPLATE_ADD:
    {
      param.Target[0].Enable = true;
      param.Target[0].ColorSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_SRC_ALPHA;
      param.Target[0].ColorDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ONE;
      param.Target[0].ColorOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_ADD;
    }break;

  case TEMPLATE_SUB:
    {
      param.Target[0].Enable = true;
      param.Target[0].ColorSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_SRC_ALPHA;
      param.Target[0].ColorDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ONE;
      param.Target[0].ColorOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_REV_SUBTRACT;
    }break;

  case TEMPLATE_MUL:
    {
      param.Target[0].Enable = true;
      param.Target[0].ColorSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_DST_COLOR;
      param.Target[0].ColorDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ZERO;
      param.Target[0].ColorOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_ADD;
    }break;
  }

  Create( param );
}


void BlendState::Create( const Graphics::BLENDSTATEPARAM& state )
{
  Delete();
  const Graphics::SPDEVICE& pDevice = GlobalPointer<GraphicsCore>::Get()->GetDevice();

  Graphics::SPBLENDSTATE obj = pDevice->CreateBlendState( state );
  IBlendState::Set( obj );
}

void BlendState::Delete()
{
  IBlendState::Clear();
}

}