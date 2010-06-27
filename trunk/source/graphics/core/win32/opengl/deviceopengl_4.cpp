#include"deviceopengl.h"


namespace Maid { namespace Graphics {

static const char* s_SHADERCODE0200 = 
"void main(void)"
"{"
"  gl_FragColor= gl_Color;"
"}"
;



static const char* s_SHADERCODE0210 = 
"uniform sampler2D texture0;"
"void main(void)"
"{"
"  gl_FragColor= texture2DProj(texture0, gl_TexCoord[0]) * gl_Color;"
"}"
;


static const char* s_SHADERCODE0211 = 
"uniform sampler2D texture0;"
"void main(void)"
"{"
"  vec4 base = texture2DProj(texture0, gl_TexCoord[0]) * gl_Color;"
"  base.rgb *= base.a;"
"  gl_FragColor= base;"
"}"
;

static const char* s_SHADERCODE0212 = 
"uniform sampler2D texture0;"
"void main(void)"
"{"
"  vec4 base = texture2DProj(texture0, gl_TexCoord[0]);"
"  vec4 ret  = vec4(1.0-base.x,1.0-base.y,1.0-base.z,1.0-base.w);"
"  gl_FragColor= ret*gl_Color;"
"}"
;



static const char* s_SHADERCODE0220 = 
"uniform sampler2D texture0;"
"uniform sampler2D texture1;"
"void main(void)"
"{"
"  vec4 mask = texture2DProj(texture0, gl_TexCoord[0]);"
"  vec4 color= texture2DProj(texture1, gl_TexCoord[1]);"
"  float alpha = mask.w + gl_Color.w;"
"  color.xyz *= gl_Color.xyz;"
"  color.w *= alpha;"
"  gl_FragColor= color;"
"}"
;

static const char* s_SHADERCODE0221 = 
"uniform sampler2D texture0;"
"uniform sampler2D texture1;"
"void main(void)"
"{"
"  vec4 mask = texture2DProj(texture0, gl_TexCoord[0]);"
"  vec4 color= texture2DProj(texture1, gl_TexCoord[1]);"
"  float alpha = mask.w - gl_Color.w;"
"  color.xyz *= gl_Color.xyz;"
"  color.w *= alpha;"
"  gl_FragColor= color;"
"}"
;

static const char* s_SHADERCODE0222 = 
"uniform sampler2D texture0;"
"uniform sampler2D texture1;"
"void main(void)"
"{"
"  vec4 mask = texture2DProj(texture0, gl_TexCoord[0]);"
"  vec4 color= texture2DProj(texture1, gl_TexCoord[1]);"
"  float alpha = mask.w + gl_Color.w;"
"  color.xyz  *= gl_Color.xyz;"
"  color.xyzw *= alpha;"
"  gl_FragColor= color;"
"}"
;

static const char* s_SHADERCODE0223 = 
"uniform sampler2D texture0;"
"uniform sampler2D texture1;"
"void main(void)"
"{"
"  vec4 mask = texture2DProj(texture0, gl_TexCoord[0]);"
"  vec4 color= texture2DProj(texture1, gl_TexCoord[1]);"
"  float alpha = mask.w - gl_Color.w;"
"  color.xyz  *= gl_Color.xyz;"
"  color.xyzw *= alpha;"
"  gl_FragColor= color;"
"}"
;

static const char* s_SHADERCODE0224 = 
"uniform sampler2D texture0;"
"uniform sampler2D texture1;"
"void main(void)"
"{"
"  vec4 mask = texture2DProj(texture0, gl_TexCoord[0]);"
"  vec4 color= texture2DProj(texture1, gl_TexCoord[1]);"
"  float alpha = mask.w + gl_Color.w;"
"  color.xyz  *= gl_Color.xyz;"
"  color.w *= alpha;"
"  gl_FragColor= vec4( 1.0-color.x, 1.0-color.y, 1.0-color.z, color.w );"
"}"
;


static const char* s_SHADERCODE0225 = 
"uniform sampler2D texture0;"
"uniform sampler2D texture1;"
"void main(void)"
"{"
"  vec4 mask = texture2DProj(texture0, gl_TexCoord[0]);"
"  vec4 color= texture2DProj(texture1, gl_TexCoord[1]);"
"  float alpha = mask.w - gl_Color.w;"
"  color.xyz  *= gl_Color.xyz;"
"  color.w *= alpha;"
"  gl_FragColor= vec4( 1.0-color.x, 1.0-color.y, 1.0-color.z, color.w );"
"}"
;





static const char* s_SHADERCODE0230 = 
"uniform sampler2D texture0;"
"uniform sampler2D texture1;"
"void main(void)"
"{"
"  vec4 tex0 = texture2DProj(texture0, gl_TexCoord[0]);"
"  vec4 tex1= texture2DProj(texture1, gl_TexCoord[1]);"
"  float pow = gl_Color.x;"
"  float alpha = gl_Color.w;"
""
"  vec4 ret = tex0.xyzw*pow + tex1.xyzw*(1.0-pow);"
"  ret.w *= alpha;"
"  gl_FragColor= ret;"
"}"
;

static const char* s_SHADERCODE0231 = 
"uniform sampler2D texture0;"
"uniform sampler2D texture1;"
"void main(void)"
"{"
"  vec4 tex0 = texture2DProj(texture0, gl_TexCoord[0]);"
"  vec4 tex1 = texture2DProj(texture1, gl_TexCoord[1]);"
"  float pow = gl_Color.x;"
"  float alpha = gl_Color.w;"
""
"  vec4 ret = tex0.xyzw*pow + tex1.xyzw*(1.0-pow);"
"  ret.w *= alpha;"
"  ret.xyzw *= alpha;"
"  gl_FragColor= ret;"
"}"
;

static const char* s_SHADERCODE0232 = 
"uniform sampler2D texture0;"
"uniform sampler2D texture1;"
"void main(void)"
"{"
"  vec4 tex0 = texture2DProj(texture0, gl_TexCoord[0]);"
"  vec4 tex1 = texture2DProj(texture1, gl_TexCoord[1]);"
"  float pow = gl_Color.x;"
"  float alpha = gl_Color.w;"
""
"  vec4 tmp = tex0.xyzw*pow + tex1.xyzw*(1.0-pow);"
"  tmp.w *= alpha;"
"  vec4 ret = vec4( 1.0-tmp.x, 1.0-tmp.y, 1.0-tmp.z, tmp.w );"
"  gl_FragColor= ret;"
"}"
;



void DeviceOpenGL::CreateDefaultPixelShader( int no, std::string& Binary )
{
  Binary = s_SHADERCODE0200;

  switch( no )
  {
  case 200: { Binary = s_SHADERCODE0200; }break;
  case 210: { Binary = s_SHADERCODE0210; }break;
  case 211: { Binary = s_SHADERCODE0211; }break;
  case 212: { Binary = s_SHADERCODE0212; }break;
  case 220: { Binary = s_SHADERCODE0220; }break;
  case 221: { Binary = s_SHADERCODE0221; }break;
  case 222: { Binary = s_SHADERCODE0222; }break;
  case 223: { Binary = s_SHADERCODE0223; }break;
  case 224: { Binary = s_SHADERCODE0224; }break;
  case 225: { Binary = s_SHADERCODE0225; }break;
  case 230: { Binary = s_SHADERCODE0230; }break;
  case 231: { Binary = s_SHADERCODE0231; }break;
  case 232: { Binary = s_SHADERCODE0232; }break;

//  default: { MAID_ASSERT( true, "範囲外です " << no ); }break;
  }
}

}}


