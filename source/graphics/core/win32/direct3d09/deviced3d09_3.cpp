#include"deviced3d09.h"

#include"../defaultshader.h"


namespace Maid { namespace Graphics {


void DeviceD3D09::CreateDefaultVertexShader( int no, std::vector<unt08>& Binary )
{
  LPD3DXBUFFER  pShader=NULL;
  LPD3DXBUFFER  pErrorMsgs=NULL;
  HRESULT ret = S_OK;

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

  ret = m_ShaderCompilerDefault(
    str.c_str(),
    str.length(),
    NULL,
    NULL,
    "main",
    "vs_2_0",
    0,
    &pShader,
    &pErrorMsgs,
    NULL );


  if( FAILED(ret) ) 
  {
    std::string text = (char*)pErrorMsgs->GetBufferPointer();

    pErrorMsgs->Release();
    MAID_ASSERT( true, "VertexShaderのコンパイルに失敗" << text << str );
    return ;
  }
  const int len = pShader->GetBufferSize();

  Binary.resize( pShader->GetBufferSize() );
  memcpy( &(Binary[0]), pShader->GetBufferPointer(), pShader->GetBufferSize()  );
  pShader->Release();
}


}}


