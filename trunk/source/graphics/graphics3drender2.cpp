#include"graphics3drender.h"

#include"customvertextemplate.h"
#include"graphics3drenderlocal.h"

namespace Maid
{


void Graphics3DRender::SpriteFill( const POINT3DF& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center )
{
  const float texel = m_CommandCtl.GetTexelMapValue();
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );

  const MATRIX4DF world = t;

  SpriteFill( world, Color, Size, POINT3DF((float)Center.x, (float)Center.y, 0) );
}

void Graphics3DRender::SpriteFillS ( const POINT3DF& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale )
{
  const float texel = m_CommandCtl.GetTexelMapValue();
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );
  const MATRIX4DF s = MATRIX4DF().SetScale( Scale.w, Scale.h, 1 );

  const MATRIX4DF world = s*t;

  SpriteFill( world, Color, Size, POINT3DF((float)Center.x, (float)Center.y, 0) );
}

void Graphics3DRender::SpriteFillR ( const POINT3DF& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, float Rotate, const VECTOR3DF& vec )
{
  const float texel = m_CommandCtl.GetTexelMapValue();
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );
  const MATRIX4DF r = MATRIX4DF().SetRotationXYZ( Rotate, vec );

  const MATRIX4DF world = r*t;

  SpriteFill( world, Color, Size, POINT3DF((float)Center.x, (float)Center.y, 0) );
}

void Graphics3DRender::SpriteFillSR( const POINT3DF& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec )
{
  const float texel = m_CommandCtl.GetTexelMapValue();
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );
  const MATRIX4DF s = MATRIX4DF().SetScale( Scale.w, Scale.h, 1 );
  const MATRIX4DF r = MATRIX4DF().SetRotationXYZ( Rotate, vec );

  const MATRIX4DF world = r*s*t;

  SpriteFill( world, Color, Size, POINT3DF((float)Center.x, (float)Center.y, 0) );
}


void Graphics3DRender::SpriteFill( const MATRIX4DF& world, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT3DF& Center )
{
  Graphics::IDrawCommand& Com = GetCommand();
  const IConstant& constant = m_ShaderConstant;
  const IVertex& vertex = m_SpriteVertex;
  int VertexSize = 0;

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
    const float l = -Center.x;
    const float t =  Center.y;//  ３D座標系は上がプラスなんだけど、２D座標系は下がプラスなので反転させる
    const float r = l + Size.w;
    const float b = t - Size.h; //  ３D座標系は上がプラスなんだけど、２D座標系は下がプラスなので反転させる

    const float z = Center.z;

    CUSTOMVERTEX_COLOR v[] =
    {
      CUSTOMVERTEX_COLOR( POINT3DF(l,t,z), Color ),  //  左上
      CUSTOMVERTEX_COLOR( POINT3DF(r,t,z), Color ),  //  右上
      CUSTOMVERTEX_COLOR( POINT3DF(l,b,z), Color ),  //  左下
      CUSTOMVERTEX_COLOR( POINT3DF(r,b,z), Color ),  //  右下
    };

    VertexSize = v[0].GetStructSize();

    const int sub = 0;
    Graphics::MAPPEDRESOURCE map;
    Com.ResourceMap( vertex.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map );
    memcpy( map.pData, v, sizeof(v) );
    Com.ResourceUnmap( vertex.Get(), sub );
  }

  const IInputLayout&  layout = m_SpriteFillLayout;
  const IVertexShader& vs = m_SpriteFillVertexShader;
  const IPixelShader&  ps = m_SpriteFillPixelShader;


  Com.SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  Com.SetVertex( 0, vertex.Get(), 0, VertexSize );
  Com.SetInputLayout( layout.Get() );

  Com.VSSetShader( vs.Get() );
  Com.PSSetShader( ps.Get() );

  SetupSpriteState();

  Com.Draw( 4, 0 );

}


}

