#include"graphics2drender.h"
#include"customvertextemplate.h"
#include"../auxiliary/mathematics/vector.h"
#include"../auxiliary/mathematics/matrix.h"



namespace Maid
{
//! 指定したテクスチャを描画する
/*!
    @param	Pos     [i ]	描画開始座標
    @param	Texture [i ]	描画するもの
    @param	Size    [i ]	切り抜き範囲
    @param	Center  [i ]	中心座標
    @param	alpha   [ o]	透明度(0.0f:見えない  1.0f:完全に見える）
 */
void Graphics2DRender::Blt  ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  const COLOR_R32G32B32A32F col(1,1,1,alpha);

  MODELINFO m;
  CreateVirtualScreenModel( Pos, rc.GetSize(), Center, m );
  Blt( m.Model, m.Translate, Texture, rc, col );
}

//! 指定したテクスチャを描画する
/*!
    @param	Pos     [i ]	描画開始座標
    @param	Texture [i ]	描画するもの
    @param	Size    [i ]	切り抜き範囲
    @param	Center  [i ]	中心座標
    @param	alpha   [i ]	透明度(0.0f:見えない  1.0f:完全に見える）
    @param	Scale   [i ]	拡大率(1.0f が等倍)
 */
void Graphics2DRender::BltS ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  const COLOR_R32G32B32A32F col(1,1,1,alpha);

  MODELINFO m;
  CreateVirtualScreenModel( Pos, rc.GetSize(), Center, m );
  Blt( m.Model, MATRIX4DF().SetScale(Scale.w,Scale.h,1)*m.Translate, Texture, rc, col );
}

//! 指定したテクスチャを描画する
/*!
    @param	Pos     [i ]	描画開始座標
    @param	Texture [i ]	描画するもの
    @param	Size    [i ]	切り抜き範囲
    @param	Center  [i ]	中心座標
    @param	alpha   [i ]	透明度(0.0f:見えない  1.0f:完全に見える）
    @param	Rotate  [i ]	回転角度（ラジアン度）
    @param	vec     [i ]	回転軸
 */
void Graphics2DRender::BltR ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, float Rotate, const VECTOR3DF& vec )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  const COLOR_R32G32B32A32F col(1,1,1,alpha);

  MODELINFO m;
  CreateVirtualScreenModel( Pos, rc.GetSize(), Center, m );
  Blt( m.Model, MATRIX4DF().SetRotationXYZ(Rotate,vec)*m.Translate, Texture, rc, col );
}

//! 指定したテクスチャを描画する
/*!
    @param	Pos     [i ]	描画開始座標
    @param	Texture [i ]	描画するもの
    @param	Size    [i ]	切り抜き範囲
    @param	Center  [i ]	中心座標
    @param	alpha   [i ]	透明度(0.0f:見えない  1.0f:完全に見える）
    @param	Scale   [i ]	拡大率(1.0f が等倍)
    @param	Rotate  [i ]	回転角度（ラジアン度）
    @param	vec     [i ]	回転軸
 */
void Graphics2DRender::BltSR( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  const COLOR_R32G32B32A32F col(1,1,1,alpha);

  MODELINFO info;
  CreateVirtualScreenModel( Pos, rc.GetSize(), Center, info );

  const MATRIX4DF m = MATRIX4DF().SetScale(Scale.w,Scale.h,1)
                    * MATRIX4DF().SetRotationXYZ(Rotate,vec)
                    * info.Translate;

  Blt( info.Model, m, Texture, rc, col );
}

void Graphics2DRender::Blt( const VECTOR4DF* Model, const MATRIX4DF& mat, const Texture2DBase& Texture,  const RECT2DI& rc, const COLOR_R32G32B32A32F& Color )
{
  //  uvの計算
  const SIZE2DI& image = Texture.GetSize();
  const SIZE2DI& real  = Texture.GetRealSize();
  const SIZE2DI& texsize=Texture.GetTextureSize();

  const int x = rc.x * real.w / image.w;
  const int y = rc.y * real.h / image.h;
  const int w = rc.w * real.w / image.w;
  const int h = rc.h * real.h / image.h;

  const float ux = float(x) / float(texsize.w);
  const float vy = float(y) / float(texsize.h);
  const float uw = float(w) / float(texsize.w);
  const float vh = float(h) / float(texsize.h);


  CUSTOMVERTEX_SPRITE v[] =
  {
    CUSTOMVERTEX_SPRITE( POINT3DF(0,0,0), Color, POINT2DF(ux   ,vy   ) ),
    CUSTOMVERTEX_SPRITE( POINT3DF(0,0,0), Color, POINT2DF(ux+uw,vy   ) ),
    CUSTOMVERTEX_SPRITE( POINT3DF(0,0,0), Color, POINT2DF(ux   ,vy+vh) ),
    CUSTOMVERTEX_SPRITE( POINT3DF(0,0,0), Color, POINT2DF(ux+uw,vy+vh) ),
  };

  SetupVertex( Model, mat, v, v[0].GetStructSize() );
  SetCommonVertex( v, sizeof(v), v[0].GetStructSize() );

  const IInputLayout&  layout = m_BltLayout;
  const IVertexShader& vs = m_BltVertexShader;
  const IMaterial&  material = Texture;

  Graphics::IDrawCommand& Command = GetCommand();


  Command.SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  Command.SetInputLayout( layout.Get() );
  Command.PSSetMaterial( 0, material.Get() );

  Command.VSSetShader( vs.Get() );

  if( m_BltState==BLENDSTATE_MUL || m_BltState==BLENDSTATE_SCREEN )
  {
    const IPixelShader&  ps = m_BltPixelShader211;
    Command.PSSetShader( ps.Get() );
  }
  else if( m_BltState==BLENDSTATE_REVERSE )
  {
    const IPixelShader&  ps = m_BltPixelShader212;
    Command.PSSetShader( ps.Get() );
  }
  else
  {
    const IPixelShader&  ps = m_BltPixelShader210;
    Command.PSSetShader( ps.Get() );
  }
  SetupState();

  Command.Draw( 4, 0 );
}

}