#include"graphics3drender.h"

#include"customvertextemplate.h"
#include"graphics3drenderlocal.h"

namespace Maid
{



//  シャドウマップ用シェーダ(1pass目)
static const char* CODE_SHADOW1_VS = 
"cbuffer cbPerObject"
"{"
"  matrix s_mWLightP  : packoffset( c0 );"
"};"
""
"struct VS_INPUT"
"{"
"  float4 Position    : POSITION;"    //頂点座標
"};"
""
"struct VS_OUTPUT"
"{"
"  float4 Position : SV_Position;"   //頂点座標
"  float4 Depth    : TEXCOORD0;"     //シャドウマップ用デプス
"};"
""
"VS_OUTPUT main(VS_INPUT Input)"
"{"
"  VS_OUTPUT Out = (VS_OUTPUT)0;"
""
"  float4 pos = mul( Input.Position, s_mWLightP );"
""
"  Out.Position = pos;"
"  Out.Depth    = pos;"
"  return Out;"
"}"
;


static const char* CODE_SHADOW1_PS = 
"struct PS_INPUT"
"{"
"  float4 Position : SV_Position;"
"  float4 Depth    : TEXCOORD0;"
"};"
""
"float4 main( PS_INPUT input) : SV_Target"
"{"
"  float4 Out;"
""
"  Out = input.Depth.z / input.Depth.w;"
""
"  return Out;"
"}"
;






//  シャドウマップ用シェーダ(2pass目・テクスチャ無し)
static const char* CODE_COLOR_SHADOW2_VS = 
"cbuffer cbPerObject"
"{"
"  matrix s_mWVP        : packoffset( c0 );" // ローカルから射影空間への座標変換
"  matrix s_mWLightP    : packoffset( c4 );" // ローカルからライト射影空間への座標変換
"  matrix s_mWLightPTex : packoffset( c8 );" // ローカルからライト射影空間->テクスチャ座標への座標変換
"  float4 s_Eye   : packoffset( c12 );" // カメラの向き
"};"
""
"struct VS_INPUT"
"{"
"  float4 Position    : POSITION;"   //頂点座標
"  float4 Diffuse     : COLOR0;"     //頂点色影響度
"  float4 Normal      : NORMAL;"     //法線
"};"
""
"struct VS_OUTPUT"
"{"
"  float4 Position    : SV_Position;" //頂点座標
"  float4 Diffuse     : COLOR0;"     //頂点色影響度
"  float4 Normal      : TEXCOORD0;"
"  float3 Eye         : TEXCOORD1;"   // 視線ベクトル
"  float4 ShadowMapUV : TEXCOORD2;"
"  float4 Depth       : TEXCOORD3;"
"};"
""
"VS_OUTPUT main(VS_INPUT Input)"
"{"
"  VS_OUTPUT Out = (VS_OUTPUT)0;"
""
"  Out.Position = mul( Input.Position, s_mWVP );"
"  Out.Diffuse = Input.Diffuse;"
"  Out.Normal  = Input.Normal;"
"  Out.Eye  = s_Eye - Input.Position.xyz;"
"  Out.ShadowMapUV  = mul( Input.Position, s_mWLightPTex );"
"  Out.Depth  = mul( Input.Position, s_mWLightP );"
""
"  return Out;"
"}"
;



//  シャドウマップ用シェーダ(2pass目・テクスチャ無し)
static const char* CODE_COLOR_SHADOW2_PS = 
"Texture2D<float4> texture_slot0;"
"sampler sampler_slot0 = sampler_state"
"{"
"    Texture   = <texture_slot0>;"
"};"
""
"cbuffer cbPerObject"
"{"
"  float4 s_MaterialColor : packoffset( c0 );"  // 素材そのものの色
"  float4 s_MaterialLight : packoffset( c1 );" // 素材の光源反射率
"  float4 s_MaterialEmissive : packoffset( c2 );" // 素材の自己発光量
"  float4 s_LightDir     : packoffset( c3 );"  // 平行光源の向き
"  float4 s_LightColor   : packoffset( c4 );"  // 平行光源の色
"  float4 s_Ambient      : packoffset( c5 );"  // ワールド全体の明るさ
"  float1 s_Alpha        : packoffset( c6 );" // プログラム側で調節する透明度
"  float2 s_Speculer     : packoffset( c7 );" // スペキュラの強さ
"};"
""
"struct PS_INPUT"
"{"
"  float4 Position    : SV_Position;" //頂点座標
"  float4 Color       : COLOR0;"      //頂点色影響度
"  float4 Normal      : TEXCOORD0;"
"  float3 Eye         : TEXCOORD1;"   // 視線ベクトル
"  float4 ShadowMapUV : TEXCOORD2;"
"  float4 Depth       : TEXCOORD3;"
"};"
""
"float4 main(PS_INPUT Input) : SV_Target"
"{"
"  float3 N = normalize(Input.Normal);"
""
"  float  lightpow = dot(N,-s_LightDir);"
"  float4 lightcol = s_LightColor * lightpow;"
""
"  float LightZ = texture_slot0.Sample(sampler_slot0, Input.ShadowMapUV.xy/Input.ShadowMapUV.ww ).r;" // ライト方向からの最前面Ｚ値
"  float PointZ  = Input.Depth.z / Input.Depth.w;" // ライト方向からの現在Ｚ値
"  float shadow = (LightZ < PointZ-0.03f)? 0.5:1.0;"
"  float4 matcolor = max(0.0,  s_MaterialLight * lightcol );"
"  float4 emi = s_MaterialEmissive;"
"  float4 amb = s_Ambient;"
""
"  float3 eye  = normalize(Input.Eye);"
"  float3 half = normalize(-s_LightDir+eye);"
"  float  spc  = pow(max(0.0,dot(N,half)),s_Speculer.y) * s_Speculer.x;"
""
"  float4 worldcolor = s_MaterialColor*(emi + matcolor + s_Ambient) + spc;"
"  worldcolor *= shadow;"
"  worldcolor.a = s_MaterialColor.a;"
""
"  float4 ret = Input.Color * worldcolor;"
"  ret.a *= s_Alpha;"
""
"  return ret;"
"}"
;













//  シャドウマップ用シェーダ(2pass目・テクスチャあり)
static const char* CODE_TEXTURE_SHADOW2_VS = 
"cbuffer cbPerObject"
"{"
"  matrix s_mWVP        : packoffset( c0 );" // ローカルから射影空間への座標変換
"  matrix s_mWLightP    : packoffset( c4 );" // ローカルからライト射影空間への座標変換
"  matrix s_mWLightPTex : packoffset( c8 );" // ローカルからライト射影空間->テクスチャ座標への座標変換
"  float4 s_Eye   : packoffset( c12 );" // カメラの向き
"  float2 s_TextureScale : packoffset( c13 );"   //テクスチャのＵＶとデータ上のＵＶを調節するための値
"};"
""
"struct VS_INPUT"
"{"
"  float4 Position    : POSITION;"   //頂点座標
"  float4 Diffuse     : COLOR0;"     //頂点色影響度
"  float4 Normal      : NORMAL;"     //法線
"  float2 TexCoords   : TEXCOORD0;"  //テクスチャUV
"};"
""
"struct VS_OUTPUT"
"{"
"  float4 Position    : SV_Position;" //頂点座標
"  float4 Diffuse     : COLOR0;"      //頂点色影響度
"  float2 TexCoords   : TEXCOORD0;"   //テクスチャUV
"  float4 Normal      : TEXCOORD1;"
"  float3 Eye         : TEXCOORD2;"   // 視線ベクトル
"  float4 ShadowMapUV : TEXCOORD3;"
"  float4 Depth       : TEXCOORD4;"
"};"
""
"VS_OUTPUT main(VS_INPUT Input)"
"{"
"  VS_OUTPUT Out = (VS_OUTPUT)0;"
""
"  Out.Position = mul( Input.Position, s_mWVP );"
"  Out.Diffuse = Input.Diffuse;"
"  Out.TexCoords  = Input.TexCoords* s_TextureScale;"
"  Out.Normal  = Input.Normal;"
"  Out.Eye  = s_Eye - Input.Position.xyz;"
"  Out.ShadowMapUV  = mul( Input.Position, s_mWLightPTex );"
"  Out.Depth  = mul( Input.Position, s_mWLightP );"
""
"  return Out;"
"}"
;

//  シャドウマップ用シェーダ(2pass目・テクスチャあり)
static const char* CODE_TEXTURE_SHADOW2_PS = 
"Texture2D<float4> texture_slot0;"
"sampler sampler_slot0 = sampler_state"
"{"
"    Texture   = <texture_slot0>;"
"};"
""
"Texture2D<float4> texture_slot1;"
"sampler sampler_slot1 = sampler_state"
"{"
"    Texture   = <texture_slot1>;"
"};"
""
"cbuffer cbPerObject"
"{"
"  float4 s_MaterialColor : packoffset( c0 );"  // 素材そのものの色
"  float4 s_MaterialLight : packoffset( c1 );" // 素材の光源反射率
"  float4 s_MaterialEmissive : packoffset( c2 );" // 素材の自己発光量
"  float4 s_LightDir     : packoffset( c3 );"  // 平行光源の向き
"  float4 s_LightColor   : packoffset( c4 );"  // 平行光源の色
"  float4 s_Ambient      : packoffset( c5 );"  // ワールド全体の明るさ
"  float1 s_Alpha        : packoffset( c6 );" // プログラム側で調節する透明度
"  float2 s_Speculer     : packoffset( c7 );" // スペキュラの強さ
"};"
""
"struct PS_INPUT"
"{"
"  float4 Position    : SV_Position;" //頂点座標
"  float4 Color       : COLOR0;"      //頂点色影響度
"  float2 TexCoords   : TEXCOORD0;"   //テクスチャUV
"  float4 Normal      : TEXCOORD1;"
"  float3 Eye         : TEXCOORD2;"   // 視線ベクトル
"  float4 ShadowMapUV : TEXCOORD3;"
"  float4 Depth       : TEXCOORD4;"
"};"
""
"float4 main(PS_INPUT Input) : SV_Target"
"{"
"  float4 MatColor = texture_slot0.Sample(sampler_slot0,Input.TexCoords) * s_MaterialColor;"
"  float3 N = normalize(Input.Normal);"
""
"  float  lightpow = dot(N,-s_LightDir);"
"  float4 lightcol = s_LightColor * lightpow;"
""
"  float LightZ = texture_slot1.Sample(sampler_slot1, Input.ShadowMapUV.xy/Input.ShadowMapUV.ww ).r;" // ライト方向からの最前面Ｚ値
"  float PointZ  = Input.Depth.z / Input.Depth.w;" // ライト方向からの現在Ｚ値
"  float shadow = (LightZ < PointZ-0.03f)? 0.1:1.0;"
"  float4 matcolor = max(0.0,  s_MaterialLight * lightcol );"
"  float4 emi = s_MaterialEmissive;"
"  float4 amb = s_Ambient;"
""
"  float3 eye  = normalize(Input.Eye);"
"  float3 half = normalize(-s_LightDir+eye);"
"  float  spc  = pow(max(0.0,dot(N,half)),s_Speculer.y) * s_Speculer.x;"
""
"  float4 worldcolor = MatColor*(emi + matcolor + s_Ambient) + spc;"
"  worldcolor *= shadow;"
"  worldcolor.a = s_MaterialColor.a;"
""
"  float4 ret = Input.Color * worldcolor;"
"  ret.a *= s_Alpha;"
""
"  return ret;"
"}"
;





void Graphics3DRender::MQOShadowShaderCreate()
{
  { //  シャドウマップ用準備
    {
      const int passid = 1*10 + 0;  //  1pass目は共通
      const String vs_code = MAIDTEXT(CODE_SHADOW1_VS);
      const String ps_code = MAIDTEXT(CODE_SHADOW1_PS);
      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };
      m_ShadowVertexShader[passid].Create( vs_code );
      m_ShadowLayout[passid].Create( element, NUMELEMENTS(element), vs_code );
      m_ShadowPixelShader[passid].Create( ps_code );
    }
    {
      const int passid = 2*10 + 0;  //  2pass目・色のみ
      const String vs_code = MAIDTEXT(CODE_COLOR_SHADOW2_VS);
      const String ps_code = MAIDTEXT(CODE_COLOR_SHADOW2_PS);
      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 1, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"NORMAL",   0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 2, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };
      m_ShadowVertexShader[passid].Create( vs_code );
      m_ShadowLayout[passid].Create( element, NUMELEMENTS(element), vs_code );
      m_ShadowPixelShader[passid].Create( ps_code );
    }
    {
      const int passid = 2*10 + 1;  //  2pass目・テクスチャアリ
      const String vs_code = MAIDTEXT(CODE_TEXTURE_SHADOW2_VS);
      const String ps_code = MAIDTEXT(CODE_TEXTURE_SHADOW2_PS);
      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 1, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"NORMAL",   0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 2, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"TEXCOORD", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT2, 3, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };
      m_ShadowVertexShader[passid].Create( vs_code );
      m_ShadowLayout[passid].Create( element, NUMELEMENTS(element), vs_code );
      m_ShadowPixelShader[passid].Create( ps_code );
    }
  }
}

bool Graphics3DRender::MQOShadowShaderIsLoading() const
{
  {
    for( std::map<int,InputLayout>::const_iterator ite=m_ShadowLayout.begin();
                              ite!=m_ShadowLayout.end(); ++ite )
    {
      if( ite->second.IsCompiling() ) { return true; }
    }

    for( std::map<int,VertexShader>::const_iterator ite=m_ShadowVertexShader.begin();
                              ite!=m_ShadowVertexShader.end(); ++ite )
    {
      if( ite->second.IsCompiling() ) { return true; }
    }

    for( std::map<int,PixelShader>::const_iterator ite=m_ShadowPixelShader.begin();
                              ite!=m_ShadowPixelShader.end(); ++ite )
    {
      if( ite->second.IsCompiling() ) { return true; }
    }
  }

  return false;
}






//! シャドウマップにレンダリングするための関数(１パス目)
/*!
    @param	Pos   [i ]	描画開始座標
    @param	model [i ]	描画するもの
 */
void Graphics3DRender::BltShadow1( const MATRIX4DF& LightP, const POINT3DF& Pos, const ModelMQO& model )
{
  const MATRIX4DF world = MATRIX4DF().SetTranslate(Pos.x,Pos.y,Pos.z);
  BltShadow1( LightP, world, model );
}

void Graphics3DRender::BltShadow1S ( const MATRIX4DF& LightP, const POINT3DF& Pos, const ModelMQO& model, const SIZE3DF& Scale )
{
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x, Pos.y, Pos.z );
  const MATRIX4DF s = MATRIX4DF().SetScale( Scale.w, Scale.h, Scale.d );

  const MATRIX4DF world = s*t;
  BltShadow1( LightP, world, model );
}

void Graphics3DRender::BltShadow1R ( const MATRIX4DF& LightP, const POINT3DF& Pos, const ModelMQO& model, float Rotate, const VECTOR3DF& vec )
{
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x, Pos.y, Pos.z );
  const MATRIX4DF r = MATRIX4DF().SetRotationXYZ( Rotate, vec );

  const MATRIX4DF world = r*t;
  BltShadow1( LightP, world, model );
}

void Graphics3DRender::BltShadow1SR( const MATRIX4DF& LightP, const POINT3DF& Pos, const ModelMQO& model, const SIZE3DF& Scale, float Rotate, const VECTOR3DF& vec )
{
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x, Pos.y, Pos.z );
  const MATRIX4DF s = MATRIX4DF().SetScale( Scale.w, Scale.h, Scale.d );
  const MATRIX4DF r = MATRIX4DF().SetRotationXYZ( Rotate, vec );

  const MATRIX4DF world = s*r*t;
  BltShadow1( LightP, world, model );
}



void Graphics3DRender::BltShadow1( const MATRIX4DF& LightP, const MATRIX4DF& world, const ModelMQO& model )
{
  const MATRIX4DF wvp = world * LightP;
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

  const IConstant& con_vs = m_ShaderConstantVS;
  {
    const int sub = 0;
    Graphics::MAPPEDRESOURCE map_vs;
    Command.ResourceMap( con_vs.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map_vs );
    CONSTANT_SHADOWMAP_1pass_VS& dst = *((CONSTANT_SHADOWMAP_1pass_VS*)map_vs.pData);
    dst.s_mWVP  = wvp.GetTranspose();
    Command.ResourceUnmap( con_vs.Get(), sub );
  }

  Command.VSSetConstant( 0, con_vs.Get() );

  const IInputLayout&  layout = m_ShadowLayout[10];
  const IVertexShader& vs = m_ShadowVertexShader[10];
  const IPixelShader&  ps = m_ShadowPixelShader[10];

  Command.SetInputLayout( layout.Get() );
  Command.VSSetShader( vs.Get() );
  Command.PSSetShader( ps.Get() );

  const MQOSCENE& Scene = model.GetScene();
  const std::vector<MQOMATERIAL>& matlist = model.GetMaterialList();
  const std::vector<boost::shared_ptr<MQOOBJECT> >& objlist = model.GetObjectList();

  for( int i=0; i<(int)objlist.size(); ++i )
  {
    const MQOOBJECT& Obj = *(objlist[i]);
    const IVertex& point = Obj.Point;

    Command.SetVertex( 0, point.Get(), 0, sizeof(POINT3DF) );

    for( int j=0; j<(int)Obj.Primitive.size(); ++j )
    {
      const MQOOBJECT::PRIMITIVE& prim = Obj.Primitive[j];
      const IIndex& index = prim.Index;
      const size_t size = prim.Index.GetSize();

      if( prim.MaterialNo!=MQOOBJECT::PRIMITIVE::MATERIAL_NONE )
      {
        Command.SetIndex( index.Get(), 0 );
        Command.DrawIndexed( size/2, 0, 0 );
      }else
      {
        //  マテリアルがない時は描画しない
      }
    }
  }

}



void Graphics3DRender::MQOShadowShaderSetup( const MATRIX4DF& world, const MATRIX4DF& wvp, const MQOMATERIAL& mat, float alpha, const MATRIX4DF& LightMat, const Texture2DBase& ShadowMap )
{
  int MaterialType = 0;
  if( mat.Texture.IsEmpty() )
  {
    MaterialType = 0;
  }else
  {
    MaterialType = 1;
//    if( mat.Bump.IsEmpty() ){ MaterialType = 1; }
//    else                    { MaterialType = 2; }
  }
  Graphics::IDrawCommand& Command = GetCommand();
  const int ShaderID = 20+MaterialType;

  const IConstant& con_vs = m_ShaderConstantVS;
  const IConstant& con_ps = m_ShaderConstantPS;

  const MATRIX4DF wlp = world * LightMat;
        MATRIX4DF wlptx;
  {
    const SIZE2DI size = ShadowMap.GetTextureSize();

    const float x = 0.5f + (0.5f / (float)size.w);
    const float y = 0.5f + (0.5f / (float)size.h);
    MATRIX4DF bias(	0.5f, 0.0f, 0.0f, 0.0f,
                    0.0f,-0.5f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f,	0.0f,
                       x,    y, 0.0f, 1.0f );
    wlptx  = (wlp * bias).GetTranspose();
  }

  const VECTOR4DF eye = VECTOR4DF(0,0,0,1) * (world*m_Camera.GetViewMatrix()).GetInverse();


  switch( ShaderID )
  {
  case 20:
    {
      {
        const int sub = 0;
        Graphics::MAPPEDRESOURCE map_vs;
        Command.ResourceMap( con_vs.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map_vs );
        {
          CONSTANT_SHADOWMAP_2pass_COLOR_VS& dst = *((CONSTANT_SHADOWMAP_2pass_COLOR_VS*)map_vs.pData);
          dst.s_mWVP  = wvp.GetTranspose();
          dst.s_mWLightP    = wlp.GetTranspose();
          dst.s_mWLightPTex = wlptx.GetTranspose();
          dst.s_Eye = eye;
        }
        Command.ResourceUnmap( con_vs.Get(), sub );
      }

      {
        const int sub = 0;
        Graphics::MAPPEDRESOURCE map_ps;
        Command.ResourceMap( con_ps.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map_ps );

        {
          //  ライトをまわしてシェーダ演算量を減らす
          CONSTANT_SHADOWMAP_2pass_COLOR_PS& dst = *((CONSTANT_SHADOWMAP_2pass_COLOR_PS*)map_ps.pData);
          dst.s_MaterialColor = mat.Color;
          dst.s_MaterialLight = COLOR_R32G32B32A32F(mat.Diffuse,mat.Diffuse,mat.Diffuse,1);
          dst.s_MaterialEmissive = COLOR_R32G32B32A32F(mat.Emissive,mat.Emissive,mat.Emissive,1);

          if( !m_Light.empty() )
          {
            const LIGHT& light = m_Light[0];
            const VECTOR4DF v = 
              (VECTOR4DF(light.Direction.x, light.Direction.y, light.Direction.z, 0 )
              * world.GetInverse()).Normalize()
              ;
            dst.s_LightDir = v;
            dst.s_LightColor = light.Diffuse;
          }else
          {
            dst.s_LightDir = VECTOR4DF(1,0,0,1);
            dst.s_LightColor = COLOR_R32G32B32A32F(1,1,1,1);
          }

          dst.s_Ambient = m_Ambient;
          dst.s_Alpha = alpha;

          dst.s_Speculer = mat.Specular;
          dst.s_SpeculerPow = mat.Power;
        }
        Command.ResourceUnmap( con_ps.Get(), sub );
      }

      const IMaterial&  m = ShadowMap;
      Command.PSSetMaterial(0, m.Get() );
    }break;

  case 21:
    {
      {
        const int sub = 0;
        Graphics::MAPPEDRESOURCE map_vs;
        Command.ResourceMap( con_vs.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map_vs );
        {
          CONSTANT_SHADOWMAP_2pass_TEXTURE_VS& dst = *((CONSTANT_SHADOWMAP_2pass_TEXTURE_VS*)map_vs.pData);
          dst.s_mWVP  = wvp.GetTranspose();
          dst.s_mWLightP    = wlp.GetTranspose();
          dst.s_mWLightPTex = wlptx.GetTranspose();
          dst.s_Eye = eye;
          if( mat.Texture.IsSetupped() )
          {
            const SIZE2DF real = mat.Texture.GetRealSize();
            const SIZE2DF tex  = mat.Texture.GetTextureSize();
            dst.s_TextureScale = SIZE2DF(real.w/tex.w,real.h/tex.h);
          }
        }
        Command.ResourceUnmap( con_vs.Get(), sub );
      }

      {
        const int sub = 0;
        Graphics::MAPPEDRESOURCE map_ps;
        Command.ResourceMap( con_ps.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map_ps );

        {
          //  ライトをまわしてシェーダ演算量を減らす
          CONSTANT_SHADOWMAP_2pass_TEXTURE_PS& dst = *((CONSTANT_SHADOWMAP_2pass_TEXTURE_PS*)map_ps.pData);
          dst.s_MaterialColor = mat.Color;
          dst.s_MaterialLight = COLOR_R32G32B32A32F(mat.Diffuse,mat.Diffuse,mat.Diffuse,1);
          dst.s_MaterialEmissive = COLOR_R32G32B32A32F(mat.Emissive,mat.Emissive,mat.Emissive,1);

          if( !m_Light.empty() )
          {
            const LIGHT& light = m_Light[0];
            const VECTOR4DF v = 
              (VECTOR4DF(light.Direction.x, light.Direction.y, light.Direction.z, 0 )
              * world.GetInverse()).Normalize()
              ;
            dst.s_LightDir = v;
            dst.s_LightColor = light.Diffuse;
          }else
          {
            dst.s_LightDir = VECTOR4DF(1,0,0,1);
            dst.s_LightColor = COLOR_R32G32B32A32F(1,1,1,1);
          }

          dst.s_Ambient = m_Ambient;
          dst.s_Alpha = alpha;

          dst.s_Speculer = mat.Specular;
          dst.s_SpeculerPow = mat.Power;
        }
        Command.ResourceUnmap( con_ps.Get(), sub );
      }

      const IMaterial&  t = mat.Texture;
      Command.PSSetMaterial(0, t.Get() );
      const IMaterial&  s = ShadowMap;
      Command.PSSetMaterial(1, s.Get() );

    }break;
  }

  Command.VSSetConstant( 0, con_vs.Get() );
  Command.PSSetConstant( 0, con_ps.Get() );

  const IInputLayout&  layout = m_ShadowLayout[ShaderID];
  const IVertexShader& vs = m_ShadowVertexShader[ShaderID];
  const IPixelShader&  ps = m_ShadowPixelShader[ShaderID];

  Command.SetInputLayout( layout.Get() );
  Command.VSSetShader( vs.Get() );
  Command.PSSetShader( ps.Get() );

}






void Graphics3DRender::BltShadow2( const MATRIX4DF& world, const ModelMQO& model, float alpha, const MATRIX4DF& LightMat, const Texture2DBase& ShadowMap )
{
  const MATRIX4DF wvp = world * m_Camera.GetViewMatrix() * m_Camera.GetProjectionMatrix();
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
    const IVertex& tan = Obj.Tangent;

    Command.SetVertex( 0, point.Get(), 0, sizeof(POINT3DF) );
    Command.SetVertex( 1, color.Get(), 0, sizeof(COLOR_R32G32B32A32F) );
    Command.SetVertex( 2, normal.Get(), 0, sizeof(VECTOR3DF) );
    Command.SetVertex( 3, uv.Get(), 0, sizeof(POINT2DF) );
    Command.SetVertex( 4, tan.Get(), 0, sizeof(VECTOR3DF) );

    for( int j=0; j<(int)Obj.Primitive.size(); ++j )
    {
      const MQOOBJECT::PRIMITIVE& prim = Obj.Primitive[j];
      const IIndex& index = prim.Index;
      const size_t size = prim.Index.GetSize();

      if( prim.MaterialNo!=MQOOBJECT::PRIMITIVE::MATERIAL_NONE )
      {
        //  マテリアルの状態から vshader, pshader, m_ShaderConstant を決める
        const MQOMATERIAL& mat = matlist[prim.MaterialNo];

        MQOShadowShaderSetup( world, wvp, mat, alpha, LightMat, ShadowMap );
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
