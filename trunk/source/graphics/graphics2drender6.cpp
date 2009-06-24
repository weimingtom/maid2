#include"graphics2drender.h"
#include"customvertextemplate.h"
#include"../auxiliary/mathematics/vector.h"
#include"../auxiliary/mathematics/matrix.h"



namespace Maid
{
//! 指定したテクスチャをマスク領域にしたがって描画する
/*!
    @param	Pos       [i ]	描画開始座標
    @param	TexBegin  [i ]	描画するもの
    @param	BeginRect [i ]	切り抜き範囲
    @param	Center    [i ]	中心座標
    @param	alpha     [i ]	透明度(0.0f:見えない  1.0f:完全に見える）
    @param	TexEnd    [i ]	合成先テクスチャ
    @param	EndRect   [i ]	TexEndの切り抜き範囲
    @param	pow       [i ]	影響量(0でTexBeginが完全に表示されて 1だとTexEndが表示される)
 */
void Graphics2DRender::BltMix( const POINT2DI& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  MODELINFO m;
  CreateVirtualScreenModel( Pos, TexBegin.GetSize(), Center, m );
  BltMix( m.Model, m.Translate, TexBegin, BeginRect, alpha, TexEnd, EndRect, pow );
}

//! 指定したテクスチャをマスク領域にしたがって描画する
/*!
    @param	Pos       [i ]	描画開始座標
    @param	TexBegin  [i ]	描画するもの
    @param	BeginRect [i ]	切り抜き範囲
    @param	Center    [i ]	中心座標
    @param	Scale     [i ]	拡大率(1.0f が等倍)
    @param	alpha     [i ]	透明度(0.0f:見えない  1.0f:完全に見える）
    @param	TexEnd    [i ]	合成先テクスチャ
    @param	EndRect   [i ]	TexEndの切り抜き範囲
    @param	pow       [i ]	影響量(0でTexBeginが完全に表示されて 1だとTexEndが表示される)
 */
void Graphics2DRender::BltMixS ( const POINT2DI& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  MODELINFO m;
  CreateVirtualScreenModel( Pos, TexBegin.GetSize(), Center, m );
  BltMix( m.Model, MATRIX4DF().SetScale(Scale.w,Scale.h,1)*m.Translate, TexBegin, BeginRect, alpha, TexEnd, EndRect, pow );
}

//! 指定したテクスチャをマスク領域にしたがって描画する
/*!
    @param	Pos       [i ]	描画開始座標
    @param	TexBegin  [i ]	描画するもの
    @param	BeginRect [i ]	切り抜き範囲
    @param	Center    [i ]	中心座標
    @param	alpha     [i ]	透明度(0.0f:見えない  1.0f:完全に見える）
    @param	Rotate    [i ]	回転角度（ラジアン度）
    @param	vec       [i ]	回転軸
    @param	TexEnd    [i ]	合成先テクスチャ
    @param	EndRect   [i ]	TexEndの切り抜き範囲
    @param	pow       [i ]	影響量(0でTexBeginが完全に表示されて 1だとTexEndが表示される)
 */
void Graphics2DRender::BltMixR ( const POINT2DI& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, float Rotate, const VECTOR3DF& vec, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  MODELINFO m;
  CreateVirtualScreenModel( Pos, TexBegin.GetSize(), Center, m );
  BltMix( m.Model, MATRIX4DF().SetRotationXYZ(Rotate,vec)*m.Translate, TexBegin, BeginRect, alpha, TexEnd, EndRect, pow );
}

//! 指定したテクスチャをマスク領域にしたがって描画する
/*!
    @param	Pos       [i ]	描画開始座標
    @param	TexBegin  [i ]	描画するもの
    @param	BeginRect [i ]	切り抜き範囲
    @param	Center    [i ]	中心座標
    @param	alpha     [i ]	透明度(0.0f:見えない  1.0f:完全に見える）
    @param	Scale     [i ]	拡大率(1.0f が等倍)
    @param	Rotate    [i ]	回転角度（ラジアン度）
    @param	vec       [i ]	回転軸
    @param	TexEnd    [i ]	合成先テクスチャ
    @param	EndRect   [i ]	TexEndの切り抜き範囲
    @param	pow       [i ]	影響量(0でTexBeginが完全に表示されて 1だとTexEndが表示される)
 */
void Graphics2DRender::BltMixSR( const POINT2DI& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  MODELINFO info;
  CreateVirtualScreenModel( Pos, TexBegin.GetSize(), Center, info );

  const MATRIX4DF m = MATRIX4DF().SetScale(Scale.w,Scale.h,1)
                    * MATRIX4DF().SetRotationXYZ(Rotate,vec)
                    * info.Translate;

  BltMix( info.Model, m, TexBegin, BeginRect, alpha, TexEnd, EndRect, pow );
}

void Graphics2DRender::BltMix ( const VECTOR4DF* Model, const MATRIX4DF& mat, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, float alpha, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow )
{
  const RECT2DF BeginUV = TexBegin.CalcUV( BeginRect );
  const RECT2DF EndUV = TexEnd.CalcUV( EndRect );

  const COLOR_R32G32B32A32F Color(pow,1,1,alpha);

  CUSTOMVERTEX_SPRITE2 v[] =
  {
    CUSTOMVERTEX_SPRITE2( POINT3DF(0,0,0), Color, EndUV.GetLT(), BeginUV.GetLT() ),
    CUSTOMVERTEX_SPRITE2( POINT3DF(0,0,0), Color, EndUV.GetRT(), BeginUV.GetRT() ),
    CUSTOMVERTEX_SPRITE2( POINT3DF(0,0,0), Color, EndUV.GetLB(), BeginUV.GetLB() ),
    CUSTOMVERTEX_SPRITE2( POINT3DF(0,0,0), Color, EndUV.GetRB(), BeginUV.GetRB() ),
  };

  SetupVertex( Model, mat, v, v[0].GetStructSize() );
  SetCommonVertex( v, sizeof(v), v[0].GetStructSize() );

  const IInputLayout&  layout = m_BltMixLayout;
  const IVertexShader& vs = m_BltMixVertexShader;
  const IMaterial&  material0 = TexEnd;
  const IMaterial&  material1 = TexBegin;

  Graphics::IDrawCommand& Command = GetCommand();


  Command.SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  Command.SetInputLayout( layout.Get() );
  Command.PSSetMaterial( 0, material0.Get() );
  Command.PSSetMaterial( 1, material1.Get() );

  Command.VSSetShader( vs.Get() );
  if( m_BltState==BLENDSTATE_MUL || m_BltState==BLENDSTATE_SCREEN )
  {
    const IPixelShader&  ps = m_BltMixPixelShader231;
    Command.PSSetShader( ps.Get() );
  }
  else if( m_BltState==BLENDSTATE_REVERSE )
  {
    const IPixelShader&  ps = m_BltMixPixelShader232;
    Command.PSSetShader( ps.Get() );
  }
  else
  {
    const IPixelShader&  ps = m_BltMixPixelShader230;
    Command.PSSetShader( ps.Get() );
  }


  SetupState();

  Command.Draw( 4, 0 );
}

}