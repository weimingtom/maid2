#include"deviced3d11.h"
#include"../defaultshader.h"


namespace Maid { namespace Graphics {


void DeviceD3D11::CreateDefaultVertexShader( int no, std::vector<unt08>& Binary )
{
  ID3D10Blob*  pShader=NULL;
  ID3D10Blob*  pErrorMsgs=NULL;
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
    NULL,
    "main",
    "vs_4_0",
    0,
    0,
    NULL,
    &pShader,
    &pErrorMsgs,
    NULL
    );

  if( FAILED(ret) ) 
  {
    std::string str = (char*)pErrorMsgs->GetBufferPointer();

    pErrorMsgs->Release();
    MAID_ASSERT( true, "エラーが起こるのはおかしい " << str );
    return ;
  }
/*
  {
    ID3D11Blob* pDst=NULL;

    D3DXDisassembleShader(
      pShader->GetBufferPointer(),
      pShader->GetBufferSize(),
      FALSE,
      NULL,
      &pDst );

    std::string text = (char*)pDst->GetBufferPointer();
    pDst->Release();

    MAID_WARNING( text );

  }
*/

  const int len = pShader->GetBufferSize();

  Binary.resize( pShader->GetBufferSize() );
  memcpy( &(Binary[0]), pShader->GetBufferPointer(), pShader->GetBufferSize()  );
  pShader->Release();

}

}}


