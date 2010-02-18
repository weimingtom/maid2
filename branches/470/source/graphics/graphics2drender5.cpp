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
void Graphics2DRender::BltMask  ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const Texture2DBase& Mask )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  const COLOR_R32G32B32A32F col(1,1,1,alpha);

  MODELINFO m;
  CreateVirtualScreenModel( Pos, rc.GetSize(), Center, m );
  BltMask( m.Model, m.Translate, Texture, rc, col, Mask );
}

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


void Graphics2DRender::BltMask( const VECTOR4DF* Model, const MATRIX4DF& mat, const Texture2DBase& Texture,  const RECT2DI& rc, const COLOR_R32G32B32A32F& Color, const Texture2DBase& Mask )
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

  COLOR_R32G32B32A32F vcol = Color;
  bool AlphaAdd = false;

  if( 0.5f < Color.GetA() )
  {
    //  かわり始め
    AlphaAdd = true;
    vcol.SetA( (Color.GetA()-0.5f)*2.0f );
  }else
  {
    //  かわり終わりめ
    AlphaAdd = false;
    vcol.SetA( 1.0f - (Color.GetA() *2.0f) );
  }


  CUSTOMVERTEX_SPRITE2 v[] =
  {
    CUSTOMVERTEX_SPRITE2( POINT3DF(0,0,0), vcol, POINT2DF(0,0), POINT2DF(ux   ,vy   ) ),
    CUSTOMVERTEX_SPRITE2( POINT3DF(0,0,0), vcol, POINT2DF(1,0), POINT2DF(ux+uw,vy   ) ),
    CUSTOMVERTEX_SPRITE2( POINT3DF(0,0,0), vcol, POINT2DF(0,1), POINT2DF(ux   ,vy+vh) ),
    CUSTOMVERTEX_SPRITE2( POINT3DF(0,0,0), vcol, POINT2DF(1,1), POINT2DF(ux+uw,vy+vh) ),
  };

  SetupVertex( Model, mat, v, v[0].GetStructSize() );
  SetCommonVertex( v, sizeof(v), v[0].GetStructSize() );

  const IInputLayout&  layout = m_BltMaskLayout;
  const IVertexShader& vs = m_BltMaskVertexShader;
  const IMaterial&  material0 = Mask;
  const IMaterial&  material1 = Texture;

  Graphics::IDrawCommand& Command = GetCommand();


  Command.SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  Command.SetInputLayout( layout.Get() );
  Command.PSSetMaterial( 0, material0.Get() );
  Command.PSSetMaterial( 1, material1.Get() );

  Command.VSSetShader( vs.Get() );
  if( m_BltState==BLENDSTATE_MUL || m_BltState==BLENDSTATE_SCREEN )
  {
    if( AlphaAdd )
    {
      const IPixelShader&  ps = m_BltMaskPixelShader222;
      Command.PSSetShader( ps.Get() );
    }else
    {
      const IPixelShader&  ps = m_BltMaskPixelShader223;
      Command.PSSetShader( ps.Get() );
    }
  }
  else if( m_BltState==BLENDSTATE_REVERSE )
  {
    if( AlphaAdd )
    {
      const IPixelShader&  ps = m_BltMaskPixelShader224;
      Command.PSSetShader( ps.Get() );
    }else
    {
      const IPixelShader&  ps = m_BltMaskPixelShader225;
      Command.PSSetShader( ps.Get() );
    }
  }
  else
  {
    if( AlphaAdd )
    {
      const IPixelShader&  ps = m_BltMaskPixelShader220;
      Command.PSSetShader( ps.Get() );
    }else
    {
      const IPixelShader&  ps = m_BltMaskPixelShader221;
      Command.PSSetShader( ps.Get() );
    }
  }


  SetupState();

  Command.Draw( 4, 0 );
}

}