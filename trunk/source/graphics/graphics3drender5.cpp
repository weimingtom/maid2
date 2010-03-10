#include"graphics3drender.h"

#include"customvertextemplate.h"
#include"graphics3drenderlocal.h"

namespace Maid
{
void Graphics3DRender::SpriteMixBlt  ( const POINT3DF& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow )
{
  const float texel = m_CommandCtl.GetTexelMapValue();

  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );

  SpriteMixBlt( t, TexBegin, BeginRect, POINT3DF((float)Center.x, (float)Center.y, 0), alpha, TexEnd, EndRect, pow );
}

void Graphics3DRender::SpriteMixBltS ( const POINT3DF& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow )
{
  const float texel = m_CommandCtl.GetTexelMapValue();
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );
  const MATRIX4DF s = MATRIX4DF().SetScale( Scale.w, Scale.h, 1 );

  const MATRIX4DF world = s*t;

  SpriteMixBlt( world, TexBegin, BeginRect, POINT3DF((float)Center.x, (float)Center.y, 0), alpha, TexEnd, EndRect, pow );
}

void Graphics3DRender::SpriteMixBltR ( const POINT3DF& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, float Rotate, const VECTOR3DF& vec, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow )
{
  const float texel = m_CommandCtl.GetTexelMapValue();
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );
  const MATRIX4DF r = MATRIX4DF().SetRotationXYZ( Rotate, vec );

  const MATRIX4DF world = r*t;

  SpriteMixBlt( world, TexBegin, BeginRect, POINT3DF((float)Center.x, (float)Center.y, 0), alpha, TexEnd, EndRect, pow );
}

void Graphics3DRender::SpriteMixBltSR( const POINT3DF& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow )
{
  const float texel = m_CommandCtl.GetTexelMapValue();
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );
  const MATRIX4DF s = MATRIX4DF().SetScale( Scale.w, Scale.h, 1 );
  const MATRIX4DF r = MATRIX4DF().SetRotationXYZ( Rotate, vec );

  const MATRIX4DF world = s*r*t;

  SpriteMixBlt( world, TexBegin, BeginRect, POINT3DF((float)Center.x, (float)Center.y, 0), alpha, TexEnd, EndRect, pow  );
}




void Graphics3DRender::SpriteMixBlt( const MATRIX4DF& world, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT3DF& Center, float alpha, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow )
{
  Graphics::IDrawCommand& Com = GetCommand();
  const IConstant& constant = m_ShaderConstant;
  const IVertex& vertex = m_SpriteVertex;
  int VertexStructSize = 0;

  const MATRIX4DF mat = world * m_Camera.GetViewMatrix() * m_Camera.GetProjectionMatrix();

  {
    const int sub = 0;
    Graphics::MAPPEDRESOURCE map;
    Com.ResourceMap( constant.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map );
    memcpy( map.pData, &(mat.GetTranspose()), sizeof(mat) );
    Com.ResourceUnmap( constant.Get(), sub );
    Com.VSSetConstant( 0, constant.Get() );
  }

  {
    const float vl = -Center.x;
    const float vt = Center.y;//  ３D座標系は上がプラスなんだけど、２D座標系は下がプラスなので反転させる
    const float vr = vl + BeginRect.GetSize().w;
    const float vb = vt - BeginRect.GetSize().h; //  ３D座標系は上がプラスなんだけど、２D座標系は下がプラスなので反転させる

    const float z = Center.z;
    const COLOR_R32G32B32A32F Color(pow,1,1,alpha);

    const RECT2DF BeginUV = TexBegin.CalcUV( BeginRect );
    const RECT2DF EndUV = TexEnd.CalcUV( EndRect );



    const CUSTOMVERTEX_SPRITE2 v[] =
    {
      CUSTOMVERTEX_SPRITE2( POINT3DF(vl,vt,z), Color, EndUV.GetLT(), BeginUV.GetLT() ),  //  左上
      CUSTOMVERTEX_SPRITE2( POINT3DF(vr,vt,z), Color, EndUV.GetRT(), BeginUV.GetRT() ),  //  右上
      CUSTOMVERTEX_SPRITE2( POINT3DF(vl,vb,z), Color, EndUV.GetLB(), BeginUV.GetLB() ),  //  左下
      CUSTOMVERTEX_SPRITE2( POINT3DF(vr,vb,z), Color, EndUV.GetRB(), BeginUV.GetRB() ),  //  右下
    };

    VertexStructSize = v[0].GetStructSize();

    const int sub = 0;
    Graphics::MAPPEDRESOURCE map;
    Com.ResourceMap( vertex.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map );
    memcpy( map.pData, v, sizeof(v) );
    Com.ResourceUnmap( vertex.Get(), sub );
  }

  const IInputLayout&  layout = m_SpriteMixBltLayout;
  const IVertexShader& vs = m_SpriteMixBltVertexShader;
  const IMaterial&  material0 = TexEnd;
  const IMaterial&  material1 = TexBegin;

  Com.SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  Com.SetVertex( 0, vertex.Get(), 0, VertexStructSize );
  Com.SetInputLayout( layout.Get() );
  Com.PSSetMaterial( 0, material0.Get() );
  Com.PSSetMaterial( 1, material1.Get() );

  Com.VSSetShader( vs.Get() );
  if( m_SpriteBltState==BLENDSTATE_MUL || m_SpriteBltState==BLENDSTATE_SCREEN )
  {
    const IPixelShader&  ps = m_SpriteMixBltPixelShader231;
    Com.PSSetShader( ps.Get() );
  }
  else if( m_SpriteBltState==BLENDSTATE_REVERSE )
  {
    const IPixelShader&  ps = m_SpriteMixBltPixelShader232;
    Com.PSSetShader( ps.Get() );
  }
  else
  {
    const IPixelShader&  ps = m_SpriteMixBltPixelShader230;
    Com.PSSetShader( ps.Get() );
  }
  SetupSpriteState();

  Com.Draw( 4, 0 );
}







}

