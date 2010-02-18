#include"graphics3drender.h"

#include"customvertextemplate.h"
#include"graphics3drenderlocal.h"

namespace Maid
{









void Graphics3DRender::Fill()
{
  if( m_TestInputLayout.IsCompiling() ) { return ; }

  CUSTOMVERTEX_COLOR v[] =
  {
    CUSTOMVERTEX_COLOR( POINT3DF(0,0,0), COLOR_R32G32B32A32F(1,0,0,1) ),
    CUSTOMVERTEX_COLOR( POINT3DF(0,1,0), COLOR_R32G32B32A32F(0,1,0,1) ),
    CUSTOMVERTEX_COLOR( POINT3DF(1,1,0), COLOR_R32G32B32A32F(0,0,1,1) ),
    CUSTOMVERTEX_COLOR( POINT3DF(1,0,0), COLOR_R32G32B32A32F(1,0,1,1) ),
  };

  m_TestVertex.Create( v, sizeof(v) );

  const unt16 i[] = { 0, 1, 2, 1,2,3 };
  m_TestIndex.Create( i, sizeof(i) );


  const MATRIX4DF trance = MATRIX4DF().SetTranslate(0,0,0) * m_ViewMatrix * m_ProjectionMatrix;

  const IVertex& vertex = m_TestVertex;
  const IIndex& index = m_TestIndex;
  const IInputLayout&  layout = m_TestInputLayout;
  const IVertexShader& vs = m_MQOVertexShader;
  const IPixelShader&  ps = m_MQOPixelShader;

  Graphics::IDrawCommand& Command = GetCommand();

  {
    const IConstant& con = m_ShaderConstant;

    const int sub = 0;
    Graphics::MAPPEDRESOURCE map;
    Command.ResourceMap( con.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map );
    memcpy( map.pData, &(trance.GetTranspose()), sizeof(trance) );
    Command.ResourceUnmap( con.Get(), sub );
    Command.VSSetConstant( 0, con.Get() );
  }

  Command.SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLELIST );

  Command.SetVertex( 0, vertex.Get(), 0, v[0].GetStructSize() );
  Command.SetInputLayout( layout.Get() );

  Command.VSSetShader( vs.Get() );
  Command.PSSetShader( ps.Get() );

//  const IIndex& index = m_TestIndex;

  Command.SetIndex( index.Get(), 0 );
  Command.DrawIndexed( 3*2, 0, 0 );

//  pCommand->Draw( 3, 0 );

}


void Graphics3DRender::_Fill()
{
  CUSTOMVERTEX_COLOR v[] =
  {
    CUSTOMVERTEX_COLOR( POINT3DF(0,0,0), COLOR_R32G32B32A32F(1,0,0,1) ),
    CUSTOMVERTEX_COLOR( POINT3DF(0,100,0), COLOR_R32G32B32A32F(0,1,0,1) ),
    CUSTOMVERTEX_COLOR( POINT3DF(200,100,0), COLOR_R32G32B32A32F(0,0,1,1) ),
    CUSTOMVERTEX_COLOR( POINT3DF(100,0,0), COLOR_R32G32B32A32F(1,0,1,1) ),
  };

  m_TestVertex.Create( v, sizeof(v) );


  const MATRIX4DF trance = MATRIX4DF().SetTranslate(0,0,0) * m_ViewMatrix * m_ProjectionMatrix;

  const IVertex& vertex = m_TestVertex;
  const IInputLayout&  layout = m_SpriteFillLayout;
  const IVertexShader& vs = m_SpriteFillVertexShader;
  const IPixelShader&  ps = m_SpriteFillPixelShader;

  Graphics::IDrawCommand& Command = GetCommand();

  {
    const IConstant& con = m_ShaderConstant;

    const int sub = 0;
    Graphics::MAPPEDRESOURCE map;
    Command.ResourceMap( con.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map );
    memcpy( map.pData, &(trance.GetTranspose()), sizeof(trance) );
    Command.ResourceUnmap( con.Get(), sub );
    Command.VSSetConstant( 0, con.Get() );
  }

  Command.SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLELIST );

  Command.SetVertex( 0, vertex.Get(), 0, v[0].GetStructSize() );
  Command.SetInputLayout( layout.Get() );

  Command.VSSetShader( vs.Get() );
  Command.PSSetShader( ps.Get() );

  {
    IBlendState& state = m_SpriteBlendState[0];
    Command.SetBlendState( state.Get() );
  }
  {
    IDepthStencilState& state = m_DepthOff;
    Command.SetDepthStencilState( state.Get() );
  }
  {
    IRasterizerState& raster = m_Rasterizer;
    Command.SetRasterizerState( raster.Get() );
  }

  Command.Draw( 3, 0 );

/*
  CUSTOMVERTEX_COLOR v[] =
  {
    CUSTOMVERTEX_COLOR( POINT3DF(0,0,0), COLOR_R32G32B32A32F(1,0,0,1) ),
    CUSTOMVERTEX_COLOR( POINT3DF(0,100,0), COLOR_R32G32B32A32F(0,1,0,1) ),
    CUSTOMVERTEX_COLOR( POINT3DF(200,100,0), COLOR_R32G32B32A32F(0,0,1,1) ),
    CUSTOMVERTEX_COLOR( POINT3DF(100,0,0), COLOR_R32G32B32A32F(1,0,1,1) ),
  };

  m_TestVertex.Create( v, sizeof(v) );


  const MATRIX4DF trance = MATRIX4DF().SetTranslate(0,0,0) * m_ViewMatrix * m_ProjectionMatrix;

  const IVertex& vertex = m_TestVertex;
  const IInputLayout&  layout = m_SpriteFillLayout;
  const IVertexShader& vs = m_SpriteFillVertexShader;
  const IPixelShader&  ps = m_SpriteFillPixelShader;

  const Graphics::SPDRAWCOMMAND& pCommand = m_pDrawCommand;

  {
    const IConstant& con = m_ShaderConstant;

    const int sub = 0;
    Graphics::MAPPEDRESOURCE map;
    pCommand->ResourceMap( con.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map );
    memcpy( map.pData, &(trance.GetTranspose()), sizeof(trance) );
    pCommand->ResourceUnmap( con.Get(), sub );
    pCommand->VSSetConstant( 0, con.Get() );
  }

  pCommand->SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLELIST );

  pCommand->SetVertex( 0, vertex.Get(), 0, v[0].GetStructSize() );
  pCommand->SetInputLayout( layout.Get() );

  pCommand->VSSetShader( vs.Get() );
  pCommand->PSSetShader( ps.Get() );

  {
    IBlendState& state = m_SpriteBlendState[0];
    pCommand->SetBlendState( state.Get() );
  }
  {
    IDepthStencilState& state = m_DepthOff;
    pCommand->SetDepthStencilState( state.Get() );
  }
  {
    IRasterizerState& raster = m_Rasterizer;
    pCommand->SetRasterizerState( raster.Get() );
  }

  pCommand->Draw( 3, 0 );
*/
}


//  MQO に座標と色が入っている場合のシェーダ
static const char* VSCODE0100 = 
"\n cbuffer cbPerObject"
"\n {"
"\n   matrix mWVP  : packoffset( c0 );"
"\n   float4 Color : packoffset( c4 );"
"\n };"
"\n"
"\n struct VS_INPUT"
"\n {"
"\n   float4 Position    : POSITION;"    //頂点座標
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n };"
"\n"
"\n struct VS_OUTPUT"
"\n {"
"\n   float4 Position    : SV_Position; "   //頂点座標
"\n   float4 Diffuse     : COLOR0;  "    //デフューズ色
"\n };"
"\n"
"\n VS_OUTPUT main(VS_INPUT Input)"
"\n {"
"\n   VS_OUTPUT Out = (VS_OUTPUT)0;"
"\n   Out.Position = mul( Input.Position, mWVP );"
"\n   Out.Diffuse = Input.Diffuse * Color;"
"\n"
"\n   return Out;"
"\n }"
;


static const char* PSCODE0100 = 
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position : SV_Position;"
"\n     float4 Color    : COLOR0;"
"\n };"
"\n"
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     return input.Color;"
"\n }"
;

void Graphics3DRender::MQOShaderCreate()
{
  //  MQOMATERIAL.Texture無 MQOMATERIAL.Bump無 MQOMATERIAL.Color * MQOOBJECT.Color
  //  MQOMATERIAL.Texture有 MQOMATERIAL.Bump無 MQOMATERIAL.Color * MQOOBJECT.Color
  //  MQOMATERIAL.Texture有 MQOMATERIAL.Bump有 MQOMATERIAL.Color * MQOOBJECT.Color

  //  ↑の３タイプ*ライトの設定個だけシェーダを作成する
  //  まあめんどくさいので　平行光源１個でいいかな

  {
    m_MQOVertexShader.Create( MAIDTEXT(VSCODE0100) );
  }

  {
    Graphics::INPUT_ELEMENT element[] =
    {
      {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 1, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
    };

    m_MQOLayout.Create( element, NUMELEMENTS(element), MAIDTEXT(VSCODE0100) );
  }

  {
    m_MQOPixelShader.Create( MAIDTEXT(PSCODE0100) );
  }

}

bool Graphics3DRender::MQOShaderIsLoading() const
{
  return  m_MQOLayout.IsCompiling()
    ||    m_MQOVertexShader.IsCompiling()
    ||    m_MQOPixelShader.IsCompiling()
    ;
}

void Graphics3DRender::MQOShaderSetup( const MATRIX4DF& wvp, const MQOMATERIAL& mat )
{
  //  マテリアルの状況からシェーダ、パラメータを設定する
  int MaterialType = 0;

  {
    if( mat.Texture.IsEmpty() )
    {
      MaterialType = 0;
    }else
    {
      if( mat.Bump.IsEmpty() ){ MaterialType = 1; }
      else                    { MaterialType = 2; }
    }
  }
  {
    int LightingType = 0;
  }


  Graphics::IDrawCommand& Command = GetCommand();
  {
    const IConstant& con = m_ShaderConstant;

    const int sub = 0;
    Graphics::MAPPEDRESOURCE map;
    Command.ResourceMap( con.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map );
    CONSTANT0100& dst = *((CONSTANT0100*)map.pData);
    dst.mWVP  = wvp.GetTranspose();
    dst.Color = mat.Color;
    Command.ResourceUnmap( con.Get(), sub );
    Command.VSSetConstant( 0, con.Get() );
  }

  const IInputLayout&  layout = m_MQOLayout;
  const IVertexShader& vs = m_MQOVertexShader;
  const IPixelShader&  ps = m_MQOPixelShader;

  Command.SetInputLayout( layout.Get() );
  Command.VSSetShader( vs.Get() );
  Command.PSSetShader( ps.Get() );

}



void Graphics3DRender::Blt( const POINT3DF& Pos, const ModelMQO& model )
{
  const MATRIX4DF wvp = MATRIX4DF().SetTranslate(Pos.x,Pos.y,Pos.z) * m_ViewMatrix * m_ProjectionMatrix;
  Graphics::IDrawCommand& Command = GetCommand();

  //  このステートは固定
  Command.SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLELIST );

  const MQOSCENE& Scene = model.GetScene();
  const std::vector<MQOMATERIAL>& matlist = model.GetMaterialList();
  const std::vector<boost::shared_ptr<MQOOBJECT> >& objlist = model.GetObjectList();

  for( int i=0; i<(int)objlist.size(); ++i )
  {
    const MQOOBJECT& Obj = *(objlist[i]);
    const IVertex& point = Obj.Point;
    const IVertex& color = Obj.Color;
    const IVertex& normal = Obj.Normal;
    const IVertex& uv = Obj.UV;

    Command.SetVertex( 0, point.Get(), 0, sizeof(POINT3DF) );
    Command.SetVertex( 1, color.Get(), 0, sizeof(COLOR_R32G32B32A32F) );
    Command.SetVertex( 2, normal.Get(), 0, sizeof(VECTOR3DF) );
    Command.SetVertex( 3, uv.Get(), 0, sizeof(POINT2DF) );



    for( int j=0; j<(int)Obj.Primitive.size(); ++j )
    {
      const MQOOBJECT::PRIMITIVE& prim = Obj.Primitive[j];
      const IIndex& index = prim.Index;
      const size_t size = prim.Index.GetSize();

      if( prim.MaterialNo!=MQOOBJECT::PRIMITIVE::MATERIAL_NONE )
      {
        //  マテリアルの状態から vshader, pshader, m_ShaderConstant を決める
        const MQOMATERIAL& mat = matlist[prim.MaterialNo];
        MQOShaderSetup( wvp, mat );
        Command.SetIndex( index.Get(), 0 );
        Command.DrawIndexed( size/2, 0, 0 );
      }else
      {
        //  マテリアルがない時は描画しない
      }
    }
  }

}


}

