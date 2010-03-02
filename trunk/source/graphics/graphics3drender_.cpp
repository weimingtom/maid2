#include"graphics3drender.h"

#include"customvertextemplate.h"
#include"graphics3drenderlocal.h"

namespace Maid
{


//  MQO に座標と色があって、ライトがない
static const char* VSCODE_COLOR_LIGHT0 = 
"\n cbuffer cbPerObject"
"\n {"
"\n   matrix s_mWVP  : packoffset( c0 );"
"\n   float4 s_MaterialColor : packoffset( c4 );"
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
"\n   Out.Position = mul( Input.Position, s_mWVP );"
"\n   Out.Diffuse = Input.Diffuse * s_MaterialColor;"
"\n"
"\n   return Out;"
"\n }"
;

//  MQO に座標と色があって、平行光源のみ
/*
static const char* VSCODE_COLOR_DIRECTIONALLIGHT =
"\n cbuffer cbPerObject"
"\n {"
"\n   matrix s_mWVP  : packoffset( c0 );"
"\n   float4 s_MaterialColor : packoffset( c4 );"
"\n   float4 s_LightDir : packoffset( c5 );"
"\n };"
"\n"
"\n struct VS_INPUT"
"\n {"
"\n   float4 Position    : POSITION;"    //頂点座標
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n   float4 Normal      : NORMAL;"      //法線
"\n };"
"\n"
"\n struct VS_OUTPUT"
"\n {"
"\n   float4 Position    : SV_Position;"  //頂点座標
"\n   float4 Diffuse     : COLOR0;"     //デフューズ色
"\n };"
"\n"
"\n VS_OUTPUT main(VS_INPUT Input)"
"\n {"
"\n   VS_OUTPUT Out = (VS_OUTPUT)0;"
"\n"
"\n   float3 l = -s_LightDir;"
"\n   float3 n = Input.Normal;"
"\n"
"\n   Out.Position = mul( Input.Position, s_mWVP );"
"\n   Out.Diffuse = Input.Diffuse * s_MaterialColor * dot(n,l);"
"\n" 
"\n   return Out;"
"\n }"
;
*/

static const char* VSCODE_COLOR_DIRECTIONALLIGHT =
"\ncbuffer cbPerObject"
"\n{"
"\n  matrix s_mWVP  : packoffset( c0 );"
"\n  matrix s_World  : packoffset( c4 );"
"\n  float4 s_Color : packoffset( c7 );"
"\n  float4 s_LightDir : packoffset( c8 );"
"\n  float4 s_LightColor : packoffset( c9 );"
"\n};"
"\n"
"\nstruct VS_INPUT"
"\n{"
"\n  float4 Position    : POSITION; "   //頂点座標
"\n  float4 Diffuse     : COLOR0;"      //デフューズ色
"\n  float4 Normal      : NORMAL;"      //法線
"\n};"
"\n"
"\nstruct VS_OUTPUT"
"\n{"
"\n  float4 Position    : SV_Position;"  //頂点座標
"\n  float4 Diffuse     : COLOR0;"     //デフューズ色
"\n};"
"\nVS_OUTPUT main(VS_INPUT Input)"
"\n{"
"\n  VS_OUTPUT Out = (VS_OUTPUT)0;"
"\n  Out.Position = mul( Input.Position, s_mWVP );"
"\n"
"\n  float3 l = -s_LightDir;"
"\n  float3 n = normalize(mul( Input.Normal, (float3x3)s_World ));"
"\n"
"\n  Out.Diffuse = Input.Diffuse * s_Color * dot(n,l);"
"\n"
"\n  return Out;"
"\n}"
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
  //A
  //0  MQOMATERIAL.Texture無 MQOMATERIAL.Bump無 MQOMATERIAL.Color * MQOOBJECT.Color
  //1  MQOMATERIAL.Texture有 MQOMATERIAL.Bump無 MQOMATERIAL.Color * MQOOBJECT.Color
  //2  MQOMATERIAL.Texture有 MQOMATERIAL.Bump有 MQOMATERIAL.Color * MQOOBJECT.Color

  //B
  //0 光源無し
  //1 平行光源

  //  A*10+B でidを割り振る

  {
    const int id = 0*10+0;
    m_MQOVertexShader[id].Create( MAIDTEXT(VSCODE_COLOR_LIGHT0) );
    Graphics::INPUT_ELEMENT element[] =
    {
      {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 1, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
    };
    m_MQOLayout[id].Create( element, NUMELEMENTS(element), MAIDTEXT(VSCODE_COLOR_LIGHT0) );

    m_MQOPixelShader[id].Create( MAIDTEXT(PSCODE0100) );
  }
  {
    const int id = 0*10+1;

    m_MQOVertexShader[id].Create( MAIDTEXT(VSCODE_COLOR_DIRECTIONALLIGHT) );
    Graphics::INPUT_ELEMENT element[] =
    {
      {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 1, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      {"NORMAL",   0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 2, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
    };

    m_MQOLayout[id].Create( element, NUMELEMENTS(element), MAIDTEXT(VSCODE_COLOR_DIRECTIONALLIGHT) );
    m_MQOPixelShader[id].Create( MAIDTEXT(PSCODE0100) );
  }

  {
    Graphics::RASTERIZERSTATEPARAM param;

    param.Fill = Graphics::RASTERIZERSTATEPARAM::FILL_SOLID;
    param.Culling = Graphics::RASTERIZERSTATEPARAM::CULLING_LEFT;
    param.MultiSample = false;

    m_MQORasterizer.Create( param );
  }
  {
    m_MQOBlendState.Create( BlendState::TEMPLATE_NORMAL );
  }

}

bool Graphics3DRender::MQOShaderIsLoading() const
{
  for( std::map<int,InputLayout>::const_iterator ite=m_MQOLayout.begin();
                            ite!=m_MQOLayout.end(); ++ite )
  {
    if( ite->second.IsCompiling() ) { return true; }
  }

  for( std::map<int,VertexShader>::const_iterator ite=m_MQOVertexShader.begin();
                            ite!=m_MQOVertexShader.end(); ++ite )
  {
    if( ite->second.IsCompiling() ) { return true; }
  }

  for( std::map<int,PixelShader>::const_iterator ite=m_MQOPixelShader.begin();
                            ite!=m_MQOPixelShader.end(); ++ite )
  {
    if( ite->second.IsCompiling() ) { return true; }
  }

  return false;
}

void Graphics3DRender::MQOShaderSetup( const MATRIX4DF& world, const MATRIX4DF& wvp, const MQOMATERIAL& mat )
{
  //  マテリアルの状況からシェーダ、パラメータを設定する
  int MaterialType = 0;
  if( mat.Texture.IsEmpty() )
  {
    MaterialType = 0;
  }else
  {
    if( mat.Bump.IsEmpty() ){ MaterialType = 1; }
    else                    { MaterialType = 2; }
  }
  int LightingType = 0;
  {
    if( m_Light.empty() ) { LightingType = 0; }
    else                  { LightingType = 1; }
  }

  Graphics::IDrawCommand& Command = GetCommand();
  const int ShaderID = MaterialType*10 + LightingType;

  {
    const IConstant& con = m_ShaderConstant;

    const int sub = 0;
    Graphics::MAPPEDRESOURCE map;
    Command.ResourceMap( con.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map );
    switch( ShaderID )
    {
    case 0:
      {
        CONSTANT0100& dst = *((CONSTANT0100*)map.pData);
        dst.s_mWVP  = wvp.GetTranspose();
        dst.s_MaterialColor = mat.Color;
      }break;

    case 1:
      {
        const LIGHT& light = m_Light[0];
        MAID_ASSERT( light.Type!=LIGHT::DIRECTIONAL, "平行光源のみサポート" );

        MATRIX4DF m = world.GetInverse();

        VECTOR4DF v = 
          VECTOR4DF(light.Direction.x, light.Direction.y, light.Direction.z, 0 )
//          * world;
//          * m
          ;

        v.Normalize();

        CONSTANT_COLOR_DIRECTIONALLIGHT& dst = *((CONSTANT_COLOR_DIRECTIONALLIGHT*)map.pData);
        dst.s_mWVP  = wvp.GetTranspose();
        dst.s_World  = world;
        dst.s_MaterialColor = mat.Color;
        dst.s_Light = v;
        dst.s_LightColor = light.Diffuse;

      }break;
    }
    Command.ResourceUnmap( con.Get(), sub );
    Command.VSSetConstant( 0, con.Get() );
  }
  {
    const IInputLayout&  layout = m_MQOLayout[ShaderID];
    const IVertexShader& vs = m_MQOVertexShader[ShaderID];
    const IPixelShader&  ps = m_MQOPixelShader[ShaderID];

    Command.SetInputLayout( layout.Get() );
    Command.VSSetShader( vs.Get() );
    Command.PSSetShader( ps.Get() );
  }
}

void Graphics3DRender::Blt( const POINT3DF& Pos, const ModelMQO& model )
{
  const MATRIX4DF world = MATRIX4DF().SetTranslate(Pos.x,Pos.y,Pos.z);
  Blt( world, model );
}


void Graphics3DRender::Blt( const MATRIX4DF& world, const ModelMQO& model )
{
  const MATRIX4DF wvp = world * m_ViewMatrix * m_ProjectionMatrix;
  Graphics::IDrawCommand& Command = GetCommand();

  //  このステートは固定
  {
    Command.SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    {
      IBlendState& state = m_MQOBlendState;
      Command.SetBlendState( state.Get() );
    }

    {
      IDepthStencilState& state = m_DepthOn;
      Command.SetDepthStencilState( state.Get() );
    }

    {
      IRasterizerState& raster = m_MQORasterizer;
      Command.SetRasterizerState( raster.Get() );
    }
  }


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
        MQOShaderSetup( world, wvp, mat );
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
