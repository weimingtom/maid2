#include"graphics2drender.h"
#include"customvertextemplate.h"
#include"../auxiliary/mathematics/vector.h"
#include"../auxiliary/mathematics/matrix.h"



namespace Maid
{
//! 指定したテクスチャをマスク領域にしたがって描画する
/*!
    @param	Pos     [i ]	描画開始座標
    @param	Texture [i ]	描画するもの
    @param	Size    [i ]	切り抜き範囲
    @param	Center  [i ]	中心座標
    @param	alpha   [i ]	透明度(0.0f:見えない  1.0f:完全に見える）
    @param	Mask    [i ]	影響を与えるマスク(透明度0が最初に消えて、1が最後に消えます）
 */
void Graphics2DRender::BltMix( const POINT2DI& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow, float alpha )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  MODELINFO m;
  CreateVirtualScreenModel( Pos, TexBegin.GetSize(), Center, m );
  BltMix( m.Model, m.Translate, TexBegin, BeginRect, Center, TexEnd, EndRect, pow, alpha );
}

#if 0
//! 指定したテクスチャをマスク領域にしたがって描画する
/*!
    @param	Pos     [i ]	描画開始座標
    @param	Texture [i ]	描画するもの
    @param	Size    [i ]	切り抜き範囲
    @param	Center  [i ]	中心座標
    @param	alpha   [i ]	透明度(0.0f:見えない  1.0f:完全に見える）
    @param	Scale   [i ]	拡大率(1.0f が等倍)
    @param	Mask    [i ]	影響を与えるマスク(透明度0が最初に消えて、1が最後に消えます）
 */
void Graphics2DRender::BltMaskS ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, const Texture2DBase& Mask )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  const COLOR_R32G32B32A32F col(1,1,1,alpha);

  MODELINFO m;
  CreateVirtualScreenModel( Pos, rc.GetSize(), Center, m );
  BltMask( m.Model, MATRIX4DF().SetScale(Scale.w,Scale.h,1)*m.Translate, Texture, rc, col, Mask );
}

//! 指定したテクスチャをマスク領域にしたがって描画する
/*!
    @param	Pos     [i ]	描画開始座標
    @param	Texture [i ]	描画するもの
    @param	Size    [i ]	切り抜き範囲
    @param	Center  [i ]	中心座標
    @param	alpha   [i ]	透明度(0.0f:見えない  1.0f:完全に見える）
    @param	Rotate  [i ]	回転角度（ラジアン度）
    @param	vec     [i ]	回転軸
    @param	Mask    [i ]	影響を与えるマスク(透明度0が最初に消えて、1が最後に消えます）
 */
void Graphics2DRender::BltMaskR ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, float Rotate, const VECTOR3DF& vec, const Texture2DBase& Mask )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  const COLOR_R32G32B32A32F col(1,1,1,alpha);

  MODELINFO m;
  CreateVirtualScreenModel( Pos, rc.GetSize(), Center, m );
  BltMask( m.Model, MATRIX4DF().SetRotationXYZ(Rotate,vec)*m.Translate, Texture, rc, col, Mask );
}

//! 指定したテクスチャをマスク領域にしたがって描画する
/*!
    @param	Pos     [i ]	描画開始座標
    @param	Texture [i ]	描画するもの
    @param	Size    [i ]	切り抜き範囲
    @param	Center  [i ]	中心座標
    @param	alpha   [i ]	透明度(0.0f:見えない  1.0f:完全に見える）
    @param	Scale   [i ]	拡大率(1.0f が等倍)
    @param	Rotate  [i ]	回転角度（ラジアン度）
    @param	vec     [i ]	回転軸
    @param	Mask    [i ]	影響を与えるマスク(透明度0が最初に消えて、1が最後に消えます）
 */
void Graphics2DRender::BltMaskSR( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec, const Texture2DBase& Mask )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  const COLOR_R32G32B32A32F col(1,1,1,alpha);

  MODELINFO info;
  CreateVirtualScreenModel( Pos, rc.GetSize(), Center, info );

  const MATRIX4DF m = MATRIX4DF().SetScale(Scale.w,Scale.h,1)
                    * MATRIX4DF().SetRotationXYZ(Rotate,vec)
                    * info.Translate;

  BltMask( info.Model, m, Texture, rc, col, Mask );
}
#endif

void Graphics2DRender::BltMix ( const VECTOR4DF* Model, const MATRIX4DF& mat, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow, float alpha )
{
  const RECT2DF BeginUV = TexBegin.CalcUV( BeginRect );
  const RECT2DF EndUV = TexEnd.CalcUV( EndRect );

  COLOR_R32G32B32A32F Color(pow,1,1,alpha);

  CUSTOMVERTEX_SPRITE2 v[] =
  {
    CUSTOMVERTEX_SPRITE2( POINT3DF(0,0,0), Color, BeginUV.GetLT(), EndUV.GetLT() ),
    CUSTOMVERTEX_SPRITE2( POINT3DF(0,0,0), Color, BeginUV.GetRT(), EndUV.GetRT() ),
    CUSTOMVERTEX_SPRITE2( POINT3DF(0,0,0), Color, BeginUV.GetLB(), EndUV.GetLB() ),
    CUSTOMVERTEX_SPRITE2( POINT3DF(0,0,0), Color, BeginUV.GetRB(), EndUV.GetRB() ),
  };

  SetupVertex( Model, mat, v, v[0].GetStructSize() );
  SetCommonVertex( v, sizeof(v), v[0].GetStructSize() );

  const IInputLayout&  layout = m_BltMixLayout;
  const IVertexShader& vs = m_BltMixVertexShader;
  const IMaterial&  material0 = TexBegin;
  const IMaterial&  material1 = TexEnd;

  Graphics::IDrawCommand& Command = GetCommand();


  Command.SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  Command.SetInputLayout( layout.Get() );
  Command.PSSetMaterial( 0, material0.Get() );
  Command.PSSetMaterial( 1, material1.Get() );

  Command.VSSetShader( vs.Get() );
  if( m_BltState==BLENDSTATE_MUL || m_BltState==BLENDSTATE_SCREEN )
  {
    const IPixelShader&  ps = m_BltMixPixelShader217;
    Command.PSSetShader( ps.Get() );
  }else
  {
    const IPixelShader&  ps = m_BltMixPixelShader216;
    Command.PSSetShader( ps.Get() );
  }


  SetupState();

  Command.Draw( 4, 0 );
}

}