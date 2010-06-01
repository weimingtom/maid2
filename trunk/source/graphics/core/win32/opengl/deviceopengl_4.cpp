#include"deviceopengl.h"


namespace Maid { namespace Graphics {

static const char* s_SHADERCODE0200 = 
"void main(void)"
"{"
"  gl_FragColor= gl_Color;"
"}"
;


/*
static const char* s_SHADERCODE0210 = 
"void main(void)"
"{"
"  gl_FragColor= texture2DProj(0, gl_TexCoord[0]);"
"}"
;
*/

//*
static const char* s_SHADERCODE0210 = 
"uniform sampler2D texture;"
"void main(void)"
"{"
"  gl_FragColor= texture2DProj(texture, gl_TexCoord[0]);"
"}"
;


void DeviceOpenGL::CreateDefaultPixelShader( int no, std::string& Binary )
{
  Binary = s_SHADERCODE0200;

  switch( no )
  {
  case 200: { Binary = s_SHADERCODE0200; }break;
  case 210: { Binary = s_SHADERCODE0210; }break;
/*
  case 211: { str = s_SHADERCODE0211; }break;
  case 212: { str = s_SHADERCODE0212; }break;
  case 220: { str = s_SHADERCODE0220; }break;
  case 221: { str = s_SHADERCODE0221; }break;
  case 222: { str = s_SHADERCODE0222; }break;
  case 223: { str = s_SHADERCODE0223; }break;
  case 224: { str = s_SHADERCODE0224; }break;
  case 225: { str = s_SHADERCODE0225; }break;
  case 230: { str = s_SHADERCODE0230; }break;
  case 231: { str = s_SHADERCODE0231; }break;
  case 232: { str = s_SHADERCODE0232; }break;
*/
//  default: { MAID_ASSERT( true, "範囲外です " << no ); }break;
  }
}

}}


