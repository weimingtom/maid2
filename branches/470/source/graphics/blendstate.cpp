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

//! 標準的なブレンド設定を作成する
/*!
    @param type [i ]  作成したい設定
 */
void BlendState::Create( TEMPLATE type )
{
  //  各種アルゴリズムは
  //  http://ofo.jp/osakana/cgtips/blendmode.phtml
  //  ここを参照。

  Graphics::BLENDSTATEPARAM param;

  switch( type )
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

      param.Target[0].AlphaSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ZERO;
      param.Target[0].AlphaDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ONE;
      param.Target[0].AlphaOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_ADD;
    }break;

  case TEMPLATE_ADD:
    {
      param.Target[0].Enable = true;
      param.Target[0].ColorSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_SRC_ALPHA;
      param.Target[0].ColorDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ONE;
      param.Target[0].ColorOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_ADD;

      param.Target[0].AlphaSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ZERO;
      param.Target[0].AlphaDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ONE;
      param.Target[0].AlphaOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_ADD;
    }break;

  case TEMPLATE_SUB:
    {
      param.Target[0].Enable = true;
      param.Target[0].ColorSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_SRC_ALPHA;
      param.Target[0].ColorDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ONE;
      param.Target[0].ColorOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_REV_SUBTRACT;

      param.Target[0].AlphaSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ZERO;
      param.Target[0].AlphaDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ONE;
      param.Target[0].AlphaOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_ADD;
    }break;

  case TEMPLATE_MUL:
    {
      param.Target[0].Enable = true;
      param.Target[0].ColorSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_DST_COLOR;
      param.Target[0].ColorDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ZERO;
      param.Target[0].ColorOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_ADD;

      param.Target[0].AlphaSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ZERO;
      param.Target[0].AlphaDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ONE;
      param.Target[0].AlphaOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_ADD;
    }break;


  case TEMPLATE_SCREEN:
    {
      param.Target[0].Enable = true;
      param.Target[0].ColorSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ONE;
      param.Target[0].ColorDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_INV_SRC_COLOR;
      param.Target[0].ColorOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_ADD;

      param.Target[0].AlphaSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ZERO;
      param.Target[0].AlphaDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ONE;
      param.Target[0].AlphaOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_ADD;
    }break;


  case TEMPLATE_DARKEN:
    {
      param.Target[0].Enable = true;
      param.Target[0].ColorSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_SRC_ALPHA;
      param.Target[0].ColorDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ONE;
      param.Target[0].ColorOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_MIN;
    }break;

  case TEMPLATE_LIGHTTEN:
    {
      param.Target[0].Enable = true;
      param.Target[0].ColorSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_SRC_ALPHA;
      param.Target[0].ColorDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ONE;
      param.Target[0].ColorOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_MAX;

      param.Target[0].AlphaSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ZERO;
      param.Target[0].AlphaDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_ONE;
      param.Target[0].AlphaOp = Graphics::BLENDSTATEPARAM::TARGET::OPERATION_ADD;
    }break;

  default:
    {
      MAID_ASSERT( true, "未実装" );
    }break;
  }

  Create( param );
}

//! 自分でカスタマイズした設定で作成する
/*!
    @param state [i ]  作成したい設定
 */
void BlendState::Create( const Graphics::BLENDSTATEPARAM& state )
{
  Delete();
  const Graphics::SPDEVICE& pDevice = GlobalPointer<GraphicsCore>::Get()->GetDevice();

  Graphics::SPBLENDSTATE obj = pDevice->CreateBlendState( state );
  IBlendState::Set( obj );
}

//! 設定を開放する
/*!
 */
void BlendState::Delete()
{
  IBlendState::Clear();
}

}