#include"graphics3drender.h"

#include"customvertextemplate.h"
#include"graphics3drenderlocal.h"

namespace Maid
{
void Graphics3DRender::SpriteMaskBlt  ( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const Texture2DBase& Mask )
{
  const float texel = m_CommandCtl.GetTexelMapValue();

  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );
  const COLOR_R32G32B32A32F col = COLOR_R32G32B32A32F(1,1,1,alpha);

  SpriteMaskBlt( t, Texture, col, rc, POINT3DF((float)Center.x, (float)Center.y, 0), Mask );
}


void Graphics3DRender::SpriteMaskBltS ( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, const Texture2DBase& Mask )
{
  const float texel = m_CommandCtl.GetTexelMapValue();
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );
  const MATRIX4DF s = MATRIX4DF().SetScale( Scale.w, Scale.h, 1 );
  const COLOR_R32G32B32A32F col = COLOR_R32G32B32A32F(1,1,1,alpha);

  const MATRIX4DF world = s*t;

  SpriteMaskBlt( world, Texture, col, rc, POINT3DF((float)Center.x, (float)Center.y, 0), Mask );
}


void Graphics3DRender::SpriteMaskBltR ( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, float Rotate, const VECTOR3DF& vec, const Texture2DBase& Mask )
{
  const float texel = m_CommandCtl.GetTexelMapValue();
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );
  const MATRIX4DF r = MATRIX4DF().SetRotationXYZ( Rotate, vec );
  const COLOR_R32G32B32A32F col = COLOR_R32G32B32A32F(1,1,1,alpha);

  const MATRIX4DF world = r*t;

  SpriteMaskBlt( world, Texture, col, rc, POINT3DF((float)Center.x, (float)Center.y, 0), Mask );
}


void Graphics3DRender::SpriteMaskBltSR( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec, const Texture2DBase& Mask )
{
  const float texel = m_CommandCtl.GetTexelMapValue();
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x-texel, Pos.y+texel, Pos.z );
  const MATRIX4DF s = MATRIX4DF().SetScale( Scale.w, Scale.h, 1 );
  const MATRIX4DF r = MATRIX4DF().SetRotationXYZ( Rotate, vec );
  const COLOR_R32G32B32A32F col = COLOR_R32G32B32A32F(1,1,1,alpha);

  const MATRIX4DF world = s*r*t;

  SpriteMaskBlt( world, Texture, col, rc, POINT3DF((float)Center.x, (float)Center.y, 0), Mask );
}




void Graphics3DRender::SpriteMaskBlt  ( const MATRIX4DF& world, const Texture2DBase& Texture, const COLOR_R32G32B32A32F& Color, const RECT2DI& rc, const POINT3DF& Center, const Texture2DBase& Mask )
{
  Graphics::IDrawCommand& Com = GetCommand();
  const IConstant& constant = m_ShaderConstant;
  const IVertex& vertex = m_SpriteVertex;
  int VertexStructSize = 0;

  const MATRIX4DF mat = world * m_ViewMatrix * m_ProjectionMatrix;

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

    const CUSTOMVERTEX_SPRITE2 v[] =
    {
      CUSTOMVERTEX_SPRITE2( POINT3DF(vl,vt,z), vcol, POINT2DF(0,0), POINT2DF(tl,tt) ),  //  左上
      CUSTOMVERTEX_SPRITE2( POINT3DF(vr,vt,z), vcol, POINT2DF(1,0), POINT2DF(tr,tt) ),  //  右上
      CUSTOMVERTEX_SPRITE2( POINT3DF(vl,vb,z), vcol, POINT2DF(0,1), POINT2DF(tl,tb) ),  //  左下
      CUSTOMVERTEX_SPRITE2( POINT3DF(vr,vb,z), vcol, POINT2DF(1,1), POINT2DF(tr,tb) ),  //  右下
    };

    VertexStructSize = v[0].GetStructSize();

    const int sub = 0;
    Graphics::MAPPEDRESOURCE map;
    Com.ResourceMap( vertex.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map );
    memcpy( map.pData, v, sizeof(v) );
    Com.ResourceUnmap( vertex.Get(), sub );
  }

  const IInputLayout&  layout = m_SpriteMaskBltLayout;
  const IVertexShader& vs = m_SpriteMaskBltVertexShader;
  const IMaterial&  material0 = Mask;
  const IMaterial&  material1 = Texture;

  Com.SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  Com.SetVertex( 0, vertex.Get(), 0, VertexStructSize );
  Com.SetInputLayout( layout.Get() );
  Com.PSSetMaterial( 0, material0.Get() );
  Com.PSSetMaterial( 1, material1.Get() );

  Com.VSSetShader( vs.Get() );
  if( m_SpriteBltState==BLENDSTATE_MUL || m_SpriteBltState==BLENDSTATE_SCREEN )
  {
    if( AlphaAdd )
    {
      const IPixelShader&  ps = m_SpriteMaskBltPixelShader222;
      Com.PSSetShader( ps.Get() );
    }else
    {
      const IPixelShader&  ps = m_SpriteMaskBltPixelShader223;
      Com.PSSetShader( ps.Get() );
    }
  }
  else if( m_SpriteBltState==BLENDSTATE_REVERSE )
  {
    if( AlphaAdd )
    {
      const IPixelShader&  ps = m_SpriteMaskBltPixelShader224;
      Com.PSSetShader( ps.Get() );
    }else
    {
      const IPixelShader&  ps = m_SpriteMaskBltPixelShader225;
      Com.PSSetShader( ps.Get() );
    }
  }  
  else
  {
    if( AlphaAdd )
    {
      const IPixelShader&  ps = m_SpriteMaskBltPixelShader220;
      Com.PSSetShader( ps.Get() );
    }else
    {
      const IPixelShader&  ps = m_SpriteMaskBltPixelShader221;
      Com.PSSetShader( ps.Get() );
    }
  }
  SetupSpriteState();

  Com.Draw( 4, 0 );
}







}

