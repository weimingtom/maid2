#include"deviced3d10_0.h"


namespace Maid { namespace Graphics {

static const char* pCode0200 = 
"\n struct PS_INPUT"
"\n {"
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n };"
"\n"
"\n struct PS_OUTPUT"
"\n {"
"\n   float4 Color     : COLOR0;  "    //デフューズ色
"\n };"
"\n"
"\n PS_OUTPUT main(PS_INPUT In)"
"\n {"
"\n   PS_OUTPUT Out = (PS_OUTPUT)0;"
"\n   Out.Color = In.Diffuse;"
"\n"
"\n   return Out;"
"\n }"
;


static const char* pCode0210 = 
"\n struct PS_INPUT"
"\n {"
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n };"
"\n"
"\n struct PS_OUTPUT"
"\n {"
"\n   float4 Color     : COLOR0;  "    //デフューズ色
"\n };"
"\n"
"\n PS_OUTPUT main(PS_INPUT In)"
"\n {"
"\n   PS_OUTPUT Out = (PS_OUTPUT)0;"
"\n   Out.Color = In.Diffuse;"
"\n"
"\n   return Out;"
"\n }"

;

static const char* pCode0211 = 
"\n struct PS_INPUT"
"\n {"
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n };"
"\n"
"\n struct PS_OUTPUT"
"\n {"
"\n   float4 Color     : COLOR0;  "    //デフューズ色
"\n };"
"\n"
"\n PS_OUTPUT main(PS_INPUT In)"
"\n {"
"\n   PS_OUTPUT Out = (PS_OUTPUT)0;"
"\n   Out.Color = In.Diffuse;"
"\n"
"\n   return Out;"
"\n }"

;
void DeviceD3D10_0::CreateDefaultPixelShader( int no, std::vector<unt08>& Binary )
{
  ID3D10Blob*  pShader=NULL;
  ID3D10Blob*  pErrorMsgs=NULL;
  HRESULT ret = S_OK;

  std::string str;
  switch( no )
  {
  case 200: { str = pCode0200; }break;
  case 210: { str = pCode0210; }break;
  case 211: { str = pCode0211; }break;

  default: { MAID_ASSERT( true, "範囲外です " << no ); }break;
  }
  ret = D3DX10CompileFromMemory(
    str.c_str(),
    str.length(),
    NULL,
    NULL,
    NULL,
    "main",
    "ps_4_0",
    0,
    0,
    NULL,
    &pShader,
    &pErrorMsgs,
    NULL
    );

  if( FAILED(ret) ) 
  {
    std::string text = (char*)pErrorMsgs->GetBufferPointer();

    pErrorMsgs->Release();
    MAID_ASSERT( true, "エラーが起こるのはおかしい" << text << str );
    return ;
  }
  const int len = pShader->GetBufferSize();

  Binary.resize( pShader->GetBufferSize() );
  memcpy( &(Binary[0]), pShader->GetBufferPointer(), pShader->GetBufferSize()  );
  pShader->Release();

}

}}


