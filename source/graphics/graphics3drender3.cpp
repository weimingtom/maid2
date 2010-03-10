#include"graphics3drender.h"

#include"customvertextemplate.h"
#include"graphics3drenderlocal.h"

namespace Maid
{
void Graphics3DRender::SpriteBlt  ( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha )
{
  const float texel = m_CommandCtl.GetTexelMapValue();

  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );
  const COLOR_R32G32B32A32F col = COLOR_R32G32B32A32F(1,1,1,alpha);

  SpriteBlt( t, Texture, col, rc, POINT3DF((float)Center.x, (float)Center.y, 0) );
}


void Graphics3DRender::SpriteBltS ( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale )
{
  const float texel = m_CommandCtl.GetTexelMapValue();
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );
  const MATRIX4DF s = MATRIX4DF().SetScale( Scale.w, Scale.h, 1 );
  const COLOR_R32G32B32A32F col = COLOR_R32G32B32A32F(1,1,1,alpha);

  const MATRIX4DF world = s*t;

  SpriteBlt( world, Texture, col, rc, POINT3DF((float)Center.x, (float)Center.y, 0) );
}

void Graphics3DRender::SpriteBltR ( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, float Rotate, const VECTOR3DF& vec )
{
  const float texel = m_CommandCtl.GetTexelMapValue();
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );
  const MATRIX4DF r = MATRIX4DF().SetRotationXYZ( Rotate, vec );
  const COLOR_R32G32B32A32F col = COLOR_R32G32B32A32F(1,1,1,alpha);

  const MATRIX4DF world = r*t;

  SpriteBlt( world, Texture, col, rc, POINT3DF((float)Center.x, (float)Center.y, 0) );
}

void Graphics3DRender::SpriteBltSR( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec )
{
  const float texel = m_CommandCtl.GetTexelMapValue();
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );
  const MATRIX4DF s = MATRIX4DF().SetScale( Scale.w, Scale.h, 1 );
  const MATRIX4DF r = MATRIX4DF().SetRotationXYZ( Rotate, vec );
  const COLOR_R32G32B32A32F col = COLOR_R32G32B32A32F(1,1,1,alpha);

  const MATRIX4DF world = s*r*t;

  SpriteBlt( world, Texture, col, rc, POINT3DF((float)Center.x, (float)Center.y, 0) );
}


void Graphics3DRender::SpriteBlt  ( const MATRIX4DF& world, const Texture2DBase& Texture, const COLOR_R32G32B32A32F& Color, const RECT2DI& rc, const POINT3DF& Center )
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
    const SIZE2DI& image = Texture.GetSize();
    const SIZE2DI& real  = Texture.GetRealSize();
    const SIZE2DI& texsize=Texture.GetTextureSize();

    const float vl = -Center.x;
    const float vt = Center.y;//  ３D座標系は上がプラスなんだけど、２D座標系は下がプラスなので反転させる
    const float vr = vl + rc.GetSize().w;
    const float vb = vt - rc.GetSize().h; //  ３D座標系は上がプラスなんだけど、２D座標系は下がプラスなので反転させる

    const float z = Center.z;

    const int ux = rc.x * real.w / image.w;
    const int vy = rc.y * real.h / image.h;
    const int uw = rc.w * real.w / image.w;
    const int vh = rc.h * real.h / image.h;

    const float tl = float(ux) / float(texsize.w);
    const float tt = float(vy) / float(texsize.h);
    const float tr = float(ux+uw) / float(texsize.w);
    const float tb = float(vy+vh) / float(texsize.h);

    const CUSTOMVERTEX_SPRITE v[] =
    {
      CUSTOMVERTEX_SPRITE( POINT3DF(vl,vt,z), Color, POINT2DF(tl,tt) ),  //  左上
      CUSTOMVERTEX_SPRITE( POINT3DF(vr,vt,z), Color, POINT2DF(tr,tt) ),  //  右上
      CUSTOMVERTEX_SPRITE( POINT3DF(vl,vb,z), Color, POINT2DF(tl,tb) ),  //  左下
      CUSTOMVERTEX_SPRITE( POINT3DF(vr,vb,z), Color, POINT2DF(tr,tb) ),  //  右下
    };

    VertexStructSize = v[0].GetStructSize();

    const int sub = 0;
    Graphics::MAPPEDRESOURCE map;
    Com.ResourceMap( vertex.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map );
    memcpy( map.pData, v, sizeof(v) );
    Com.ResourceUnmap( vertex.Get(), sub );
  }

  const IInputLayout&  layout = m_SpriteBltLayout;
  const IVertexShader& vs = m_SpriteBltVertexShader;
  const IMaterial&  material = Texture;


  Com.SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  Com.SetVertex( 0, vertex.Get(), 0, VertexStructSize );
  Com.SetInputLayout( layout.Get() );
  Com.PSSetMaterial( 0, material.Get() );

  Com.VSSetShader( vs.Get() );
  if( m_SpriteBltState==BLENDSTATE_MUL || m_SpriteBltState==BLENDSTATE_SCREEN )
  {
    const IPixelShader&  ps = m_SpriteBltPixelShader211;
    Com.PSSetShader( ps.Get() );
  }
  else if( m_SpriteBltState==BLENDSTATE_REVERSE )
  {
    const IPixelShader&  ps = m_SpriteBltPixelShader212;
    Com.PSSetShader( ps.Get() );
  }
  else
  {
    const IPixelShader&  ps = m_SpriteBltPixelShader210;
    Com.PSSetShader( ps.Get() );
  }
  SetupSpriteState();

  Com.Draw( 4, 0 );
}






}

