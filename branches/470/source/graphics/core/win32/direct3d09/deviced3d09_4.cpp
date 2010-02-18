#include"deviced3d09.h"

#include"../defaultshader.h"


namespace Maid { namespace Graphics {

void DeviceD3D09::CreateDefaultPixelShader( int no, std::vector<unt08>& Binary )
{
  LPD3DXBUFFER  pShader=NULL;
  LPD3DXBUFFER  pErrorMsgs=NULL;
  HRESULT ret = S_OK;

  std::string str;
  switch( no )
  {
  case 200: { str = s_SHADERCODE0200; }break;
  case 210: { str = s_SHADERCODE0210; }break;
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

  default: { MAID_ASSERT( true, "範囲外です " << no ); }break;
  }

  ret = m_ShaderCompilerDefault(
    str.c_str(),
    str.length(),
    NULL,
    NULL,
    "main",
    "ps_2_0",
    0,
    &pShader,
    &pErrorMsgs,
    NULL );


  if( FAILED(ret) ) 
  {
    std::string text = (char*)pErrorMsgs->GetBufferPointer();

    pErrorMsgs->Release();
    MAID_ASSERT( true, "PixelShaderのコンパイルに失敗" << text << str );
    return ;
  }
  const int len = pShader->GetBufferSize();

  Binary.resize( pShader->GetBufferSize() );
  memcpy( &(Binary[0]), pShader->GetBufferPointer(), pShader->GetBufferSize()  );
  pShader->Release();
}


}}


