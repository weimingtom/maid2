#include"graphicsrender.h"
#include"customvertextemplate.h"
#include"../auxiliary/mathematics/vector.h"
#include"../auxiliary/mathematics/matrix.h"



namespace Maid
{
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定したテクスチャを描画する
/*!
    @param	Pos     [i ]	描画開始座標
    @param	Texture [i ]	描画するもの
    @param	Size    [i ]	切り抜き範囲
    @param	Center  [i ]	中心座標
    @param	alpha   [ o]	透明度
 */
void GraphicsRender::Blt  ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  const COLOR_R32G32B32A32F col(1,1,1,alpha);

  MODELINFO m;
  CreateVirtualScreenModel( Pos, rc.GetSize(), Center, m );
  Blt( m.Model, m.Translate, Texture, rc, col );
}


void GraphicsRender::BltS ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  const COLOR_R32G32B32A32F col(1,1,1,alpha);

  MODELINFO m;
  CreateVirtualScreenModel( Pos, rc.GetSize(), Center, m );
  Blt( m.Model, MATRIX4DF().SetScale(Scale.w,Scale.h,1)*m.Translate, Texture, rc, col );
}

void GraphicsRender::BltR ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, float Rotate, const VECTOR3DF& vec )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  const COLOR_R32G32B32A32F col(1,1,1,alpha);

  MODELINFO m;
  CreateVirtualScreenModel( Pos, rc.GetSize(), Center, m );
  Blt( m.Model, MATRIX4DF().SetRotationXYZ(Rotate,vec)*m.Translate, Texture, rc, col );
}

void GraphicsRender::BltSR( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec )
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

void GraphicsRender::Blt( const VECTOR4DF* Model, const MATRIX4DF& mat, const Texture2DBase& Texture,  const RECT2DI& rc, const COLOR_R32G32B32A32F& Color )
{
  //  uvの計算
  const SIZE2DI& image = Texture.GetSize();
  const SIZE2DI& create= Texture.GetCreateSize();
  const SIZE2DI& texsize=Texture.GetTextureSize();

  const int x = rc.x * image.w / create.w;
  const int y = rc.y * image.h / create.h;
  const int w = rc.w * image.w / create.w;
  const int h = rc.h * image.h / create.h;

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
  const IPixelShader&  ps = m_BltPixelShader;
  const IMaterial&  material = Texture;

  const Graphics::SPDRAWCOMMAND& pCommand = m_pDrawCommand;


  pCommand->SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  pCommand->SetInputLayout( layout.Get() );
  pCommand->PSSetMaterial( 0, material.Get() );

  pCommand->VSSetShader( vs.Get() );
  pCommand->PSSetShader( ps.Get() );
  pCommand->Draw( 4, 0 );
}

}