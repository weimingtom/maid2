#include"deviceopengl.h"

namespace Maid { namespace Graphics {

static const char* s_SHADERCODE0100 = 
"void main(void)"
"{"
"  gl_Position = gl_Vertex;"
"  gl_FrontColor= gl_Color;"
"}"
;

static const char* s_SHADERCODE0101 = 
"void main(void)"
"{"
"  gl_Position = gl_Vertex;"
"  gl_FrontColor= gl_Color;"
"  gl_TexCoord[0] = gl_MultiTexCoord0;"
"}"
;


void DeviceOpenGL::CreateDefaultVertexShader( int no, std::string& Binary )
{
  Binary = s_SHADERCODE0100;

  switch( no )
  {
  case 100: { Binary = s_SHADERCODE0100; }break;
  case 101: { Binary = s_SHADERCODE0101; }break;
/*
  case 102: { str = s_SHADERCODE0102; }break;
  case 110: { str = s_SHADERCODE0110; }break;
  case 111: { str = s_SHADERCODE0111; }break;
  case 112: { str = s_SHADERCODE0112; }break;
*/
//  default: { MAID_ASSERT( true, "範囲外です " << no ); }break;
  }

/*
  std::string str;
  switch( no )
  {
  case 100: { str = s_SHADERCODE0100; }break;
  case 101: { str = s_SHADERCODE0101; }break;
  case 102: { str = s_SHADERCODE0102; }break;
  case 110: { str = s_SHADERCODE0110; }break;
  case 111: { str = s_SHADERCODE0111; }break;
  case 112: { str = s_SHADERCODE0112; }break;

  default: { MAID_ASSERT( true, "範囲外です " << no ); }break;
  }

*/
}

}}


