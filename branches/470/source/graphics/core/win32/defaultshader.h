#ifndef graphic_core_win32_defaultshader_h
#define graphic_core_win32_defaultshader_h


 //  頂点と色を流し込むシェーダー
static const char* s_SHADERCODE0100 = 
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
"\n VS_OUTPUT main(VS_INPUT In)"
"\n {"
"\n   VS_OUTPUT Out = (VS_OUTPUT)0;"
"\n   Out.Position = In.Position;"
"\n   Out.Diffuse = In.Diffuse;"
"\n"
"\n   return Out;"
"\n }"
;


 //  頂点と色とUVを流し込むシェーダー
static const char* s_SHADERCODE0101 = 
"\n struct VS_INPUT"
"\n {"
"\n   float4 Position    : POSITION;"    //頂点座標
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n   float2 TexCoords   : TEXCOORD0;"   //テクスチャUV
"\n };"
"\n"
"\n struct VS_OUTPUT"
"\n {"
"\n   float4 Position    : SV_Position; "   //頂点座標
"\n   float4 Diffuse     : COLOR0;  "    //デフューズ色
"\n   float2 TexCoords   : TEXCOORD0;"   //テクスチャUV
"\n };"
"\n"
"\n VS_OUTPUT main(VS_INPUT In)"
"\n {"
"\n   VS_OUTPUT Out = (VS_OUTPUT)0;"
"\n   Out.Position = In.Position;"
"\n   Out.Diffuse = In.Diffuse;"
"\n   Out.TexCoords = In.TexCoords;"
"\n"
"\n   return Out;"
"\n }"
;


 //  頂点と色とUVを流し込むシェーダー
static const char* s_SHADERCODE0102 = 
"\n struct VS_INPUT"
"\n {"
"\n   float4 Position    : POSITION;"    //頂点座標
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n   float2 TexCoords0  : TEXCOORD0;"   //マスク
"\n   float2 TexCoords1  : TEXCOORD1;"   //テクスチャUV
"\n };"
"\n"
"\n struct VS_OUTPUT"
"\n {"
"\n   float4 Position    : SV_Position; "   //頂点座標
"\n   float4 Diffuse     : COLOR0;  "    //デフューズ色
"\n   float2 TexCoords0  : TEXCOORD0;"   //マスク
"\n   float2 TexCoords1  : TEXCOORD1;"   //テクスチャUV
"\n };"
"\n"
"\n VS_OUTPUT main(VS_INPUT In)"
"\n {"
"\n   VS_OUTPUT Out = (VS_OUTPUT)0;"
"\n   Out.Position = In.Position;"
"\n   Out.Diffuse = In.Diffuse;"
"\n   Out.TexCoords0 = In.TexCoords0;"
"\n   Out.TexCoords1 = In.TexCoords1;"
"\n"
"\n   return Out;"
"\n }"
;








//	world,view,projectionを掛け合わせる頂点カラー
static const char* s_SHADERCODE0110 = 
"\n cbuffer cbPerObject"
"\n {"
"\n   matrix mWVP	: packoffset( c0 );"
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
"\n   Out.Diffuse = Input.Diffuse;"
"\n"
"\n   return Out;"
"\n }"
;



//	world,view,projectionを掛け合わせるテクスチャ付
static const char* s_SHADERCODE0111 = 
"\n cbuffer cbPerObject"
"\n {"
"\n   matrix mWVP	: packoffset( c0 );"
"\n };"
"\n"
"\n struct VS_INPUT"
"\n {"
"\n   float4 Position    : POSITION;"    //頂点座標
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n   float2 TexCoords   : TEXCOORD0;"   //テクスチャUV
"\n };"
"\n"
"\n struct VS_OUTPUT"
"\n {"
"\n   float4 Position    : SV_Position; "   //頂点座標
"\n   float4 Diffuse     : COLOR0;  "    //デフューズ色
"\n   float2 TexCoords   : TEXCOORD0;"   //テクスチャUV
"\n };"
"\n"
"\n VS_OUTPUT main(VS_INPUT Input)"
"\n {"
"\n   VS_OUTPUT Out = (VS_OUTPUT)0;"
"\n   Out.Position = mul( Input.Position, mWVP );"
"\n   Out.Diffuse = Input.Diffuse;"
"\n   Out.TexCoords = Input.TexCoords;"
"\n"
"\n   return Out;"
"\n }"
;




//	world,view,projectionを掛け合わせるテクスチャ２枚付
static const char* s_SHADERCODE0112 = 
"\n cbuffer cbPerObject"
"\n {"
"\n   matrix mWVP	: packoffset( c0 );"
"\n };"
"\n"
"\n struct VS_INPUT"
"\n {"
"\n   float4 Position    : POSITION;"    //頂点座標
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n   float2 TexCoords0  : TEXCOORD0;"   //テクスチャUV
"\n   float2 TexCoords1  : TEXCOORD1;"   //テクスチャUV
"\n };"
"\n"
"\n struct VS_OUTPUT"
"\n {"
"\n   float4 Position    : SV_Position; "   //頂点座標
"\n   float4 Diffuse     : COLOR0;  "    //デフューズ色
"\n   float2 TexCoords0  : TEXCOORD0;"   //テクスチャUV
"\n   float2 TexCoords1  : TEXCOORD1;"   //テクスチャUV
"\n };"
"\n"
"\n VS_OUTPUT main(VS_INPUT Input)"
"\n {"
"\n   VS_OUTPUT Out = (VS_OUTPUT)0;"
"\n   Out.Position = mul( Input.Position, mWVP );"
"\n   Out.Diffuse = Input.Diffuse;"
"\n   Out.TexCoords0 = Input.TexCoords0;"
"\n   Out.TexCoords1 = Input.TexCoords1;"
"\n"
"\n   return Out;"
"\n }"
;












































static const char* s_SHADERCODE0200 = 
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


static const char* s_SHADERCODE0210 = 
"\n Texture2D<float4> texture_slot0;"
"\n sampler sampler_slot0 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot0>;"
"\n };"
"\n"
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position : SV_Position;"
"\n     float4 Color : COLOR0;"
"\n     float2 TexUV : TEXCOORD0;"
"\n };"
"\n"
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     return texture_slot0.Sample(sampler_slot0,input.TexUV) * input.Color;"
"\n }"
;

static const char* s_SHADERCODE0211 = 
"\n Texture2D<float4> texture_slot0;"
"\n sampler sampler_slot0 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot0>;"
"\n };"
"\n"
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position : SV_Position;"
"\n     float4 Diffuse : COLOR0;"
"\n     float2 TexUV   : TEXCOORD0;"
"\n };"
"\n"
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     float4 base = texture_slot0.Sample(sampler_slot0,input.TexUV) * input.Diffuse;"
"\n     float alpha = base.w;"
"\n     base.xyz *= alpha;"
"\n     return base;"
"\n }"
;

static const char* s_SHADERCODE0212 = 
"\n Texture2D<float4> texture_slot0;"
"\n Texture2D<float4> texture_slot1;"
"\n sampler sampler_slot0 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot0>;"
"\n };"
"\n sampler sampler_slot1 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot1>;"
"\n };"
"\n "
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position: SV_Position;"
"\n     float4 Diffuse : COLOR0;"
"\n     float2 TexUV0  : TEXCOORD0;"
"\n };"
"\n "
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     float4 tex0 = texture_slot0.Sample(sampler_slot0,input.TexUV0);"
"\n     float4 rev  = { 1-tex0.x, 1-tex0.y, 1-tex0.z, tex0.w };"
"\n "
"\n     return rev * input.Diffuse;"
"\n }"
;




static const char* s_SHADERCODE0220 =
"\n Texture2D<float4> texture_slot0;"
"\n Texture2D<float4> texture_slot1;"
"\n sampler sampler_slot0 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot0>;"
"\n };"
"\n sampler sampler_slot1 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot1>;"
"\n };"
"\n "
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position: SV_Position;"
"\n     float4 Diffuse : COLOR0;"
"\n     float2 Mask   : TEXCOORD0;"
"\n     float2 Color  : TEXCOORD1;"
"\n };"
"\n "
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     float4 mask  = texture_slot0.Sample(sampler_slot0,input.Mask);"
"\n     float4 color = texture_slot1.Sample(sampler_slot1,input.Color);"
"\n     float alpha = mask.w + input.Diffuse.w;"
"\n "
"\n     color.xyz *= input.Diffuse.xyz;"
"\n     color.w *= alpha;"
"\n " 
"\n     return color;"
"\n }"
;

static const char* s_SHADERCODE0221 =
"\n Texture2D<float4> texture_slot0;"
"\n Texture2D<float4> texture_slot1;"
"\n sampler sampler_slot0 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot0>;"
"\n };"
"\n sampler sampler_slot1 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot1>;"
"\n };"
"\n "
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position: SV_Position;"
"\n     float4 Diffuse : COLOR0;"
"\n     float2 Mask   : TEXCOORD0;"
"\n     float2 Color  : TEXCOORD1;"
"\n };"
"\n "
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     float4 mask  = texture_slot0.Sample(sampler_slot0,input.Mask);"
"\n     float4 color = texture_slot1.Sample(sampler_slot1,input.Color);"
"\n     float alpha = mask.w - input.Diffuse.w;"
"\n "
"\n     color.xyz *= input.Diffuse.xyz;"
"\n     color.w *= alpha;"
"\n " 
"\n     return color;"
"\n }"
;





static const char* s_SHADERCODE0222 =
"\n Texture2D<float4> texture_slot0;"
"\n Texture2D<float4> texture_slot1;"
"\n sampler sampler_slot0 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot0>;"
"\n };"
"\n sampler sampler_slot1 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot1>;"
"\n };"
"\n "
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position: SV_Position;"
"\n     float4 Diffuse : COLOR0;"
"\n     float2 Mask   : TEXCOORD0;"
"\n     float2 Color  : TEXCOORD1;"
"\n };"
"\n "
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     float4 mask  = texture_slot0.Sample(sampler_slot0,input.Mask);"
"\n     float4 color = texture_slot1.Sample(sampler_slot1,input.Color);"
"\n     float alpha = mask.w + input.Diffuse.w;"
"\n "
"\n     color.xyz  *= input.Diffuse.xyz;"
"\n     color.xyzw *= alpha;"
"\n " 
"\n     return color;"
"\n }"
;

static const char* s_SHADERCODE0223 =
"\n Texture2D<float4> texture_slot0;"
"\n Texture2D<float4> texture_slot1;"
"\n sampler sampler_slot0 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot0>;"
"\n };"
"\n sampler sampler_slot1 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot1>;"
"\n };"
"\n "
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position: SV_Position;"
"\n     float4 Diffuse : COLOR0;"
"\n     float2 Mask   : TEXCOORD0;"
"\n     float2 Color  : TEXCOORD1;"
"\n };"
"\n "
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     float4 mask  = texture_slot0.Sample(sampler_slot0,input.Mask);"
"\n     float4 color = texture_slot1.Sample(sampler_slot1,input.Color);"
"\n     float alpha = mask.w - input.Diffuse.w;"
"\n "
"\n     color.xyz  *= input.Diffuse.xyz;"
"\n     color.xyzw *= alpha;"
"\n " 
"\n     return color;"
"\n }"
;




static const char* s_SHADERCODE0224 =
"\n Texture2D<float4> texture_slot0;"
"\n Texture2D<float4> texture_slot1;"
"\n sampler sampler_slot0 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot0>;"
"\n };"
"\n sampler sampler_slot1 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot1>;"
"\n };"
"\n "
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position: SV_Position;"
"\n     float4 Diffuse : COLOR0;"
"\n     float2 Mask   : TEXCOORD0;"
"\n     float2 Color  : TEXCOORD1;"
"\n };"
"\n "
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     float4 mask  = texture_slot0.Sample(sampler_slot0,input.Mask);"
"\n     float4 color = texture_slot1.Sample(sampler_slot1,input.Color);"
"\n     float alpha = mask.w + input.Diffuse.w;"
"\n "
"\n     color.xyz *= input.Diffuse.xyz;"
"\n     color.w *= alpha;"
"\n " 
"\n     float4 ret = { 1-color.x, 1-color.y, 1-color.z, color.w };"
"\n     return ret;"
"\n }"
;

static const char* s_SHADERCODE0225 =
"\n Texture2D<float4> texture_slot0;"
"\n Texture2D<float4> texture_slot1;"
"\n sampler sampler_slot0 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot0>;"
"\n };"
"\n sampler sampler_slot1 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot1>;"
"\n };"
"\n "
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position: SV_Position;"
"\n     float4 Diffuse : COLOR0;"
"\n     float2 Mask   : TEXCOORD0;"
"\n     float2 Color  : TEXCOORD1;"
"\n };"
"\n "
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     float4 mask  = texture_slot0.Sample(sampler_slot0,input.Mask);"
"\n     float4 color = texture_slot1.Sample(sampler_slot1,input.Color);"
"\n     float alpha = mask.w - input.Diffuse.w;"
"\n "
"\n     color.xyz *= input.Diffuse.xyz;"
"\n     color.w *= alpha;"
"\n " 
"\n     float4 ret = { 1-color.x, 1-color.y, 1-color.z, color.w };"
"\n     return ret;"
"\n }"
;











static const char* s_SHADERCODE0230 =
"\n Texture2D<float4> texture_slot0;"
"\n Texture2D<float4> texture_slot1;"
"\n sampler sampler_slot0 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot0>;"
"\n };"
"\n sampler sampler_slot1 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot1>;"
"\n };"
"\n "
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position: SV_Position;"
"\n     float4 Diffuse : COLOR0;"
"\n     float2 TexUV0  : TEXCOORD0;"
"\n     float2 TexUV1  : TEXCOORD1;"
"\n };"
"\n "
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     float4 tex0 = texture_slot0.Sample(sampler_slot0,input.TexUV0);"
"\n     float4 tex1 = texture_slot1.Sample(sampler_slot1,input.TexUV1);"
"\n     float pow = input.Diffuse.x;"
"\n     float alpha = input.Diffuse.w;"
"\n     "
"\n     float4 ret = tex0.xyzw*pow + tex1.xyzw*(1-pow);"
"\n     ret.w *= alpha;"
"\n     "
"\n     return ret;"
"\n }"
;

static const char* s_SHADERCODE0231 =
"\n Texture2D<float4> texture_slot0;"
"\n Texture2D<float4> texture_slot1;"
"\n sampler sampler_slot0 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot0>;"
"\n };"
"\n sampler sampler_slot1 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot1>;"
"\n };"
"\n "
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position: SV_Position;"
"\n     float4 Diffuse : COLOR0;"
"\n     float2 TexUV0  : TEXCOORD0;"
"\n     float2 TexUV1  : TEXCOORD1;"
"\n };"
"\n "
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     float4 tex0 = texture_slot0.Sample(sampler_slot0,input.TexUV0);"
"\n     float4 tex1 = texture_slot1.Sample(sampler_slot1,input.TexUV1);"
"\n     float pow = input.Diffuse.x;"
"\n     float alpha = input.Diffuse.w;"
"\n     "
"\n     float4 ret = tex0.xyzw*pow + tex1.xyzw*(1-pow);"
"\n     ret.w *= alpha;"
"\n     ret.xyzw *= alpha;"
"\n     "
"\n     return ret;"
"\n }"
;



static const char* s_SHADERCODE0232 =
"\n Texture2D<float4> texture_slot0;"
"\n Texture2D<float4> texture_slot1;"
"\n sampler sampler_slot0 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot0>;"
"\n };"
"\n sampler sampler_slot1 = sampler_state"
"\n {"
"\n     Texture   = <texture_slot1>;"
"\n };"
"\n "
"\n struct PS_INPUT"
"\n {"
"\n     float4 Position: SV_Position;"
"\n     float4 Diffuse : COLOR0;"
"\n     float2 TexUV0  : TEXCOORD0;"
"\n     float2 TexUV1  : TEXCOORD1;"
"\n };"
"\n "
"\n float4 main( PS_INPUT input) : SV_Target"
"\n {"
"\n     float4 tex0 = texture_slot0.Sample(sampler_slot0,input.TexUV0);"
"\n     float4 tex1 = texture_slot1.Sample(sampler_slot1,input.TexUV1);"
"\n     float pow = input.Diffuse.x;"
"\n     float alpha = input.Diffuse.w;"
"\n     "
"\n     float4 tmp = tex0.xyzw*pow + tex1.xyzw*(1-pow);"
"\n     tmp.w *= alpha;"
"\n     "
"\n     float4 ret = { 1-tmp.x, 1-tmp.y, 1-tmp.z, tmp.w };"
"\n     return ret;"
"\n }"
;
#endif