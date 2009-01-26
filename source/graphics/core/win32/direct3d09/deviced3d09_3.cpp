#include"deviced3d09.h"



namespace Maid { namespace Graphics {

static const char* pCode0100 = 
"\n struct VS_INPUT"
"\n {"
"\n   float4 Position    : POSITION;"    //頂点座標
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n };"
"\n"
"\n struct VS_OUTPUT"
"\n {"
"\n   float4 Position    : POSITION; "   //頂点座標
"\n   float4 Diffuse     : COLOR0;  "    //デフューズ色
"\n };"
"\n"
"\n VS_OUTPUT VSBaseColor(VS_INPUT In)"
"\n {"
"\n   VS_OUTPUT Out = (VS_OUTPUT)0;"
"\n   Out.Position = In.Position;"
"\n   Out.Diffuse = In.Diffuse;"
"\n"
"\n   return Out;"
"\n }"
;




void DeviceD3D09::CreateDefaultVertexShader( int no, std::vector<unt08>& Binary )
{
  LPD3DXBUFFER  pShader=NULL;
  LPD3DXBUFFER  pErrorMsgs=NULL;
  HRESULT ret = S_OK;

  switch( no )
  {
  case 100:
    { //  頂点と色を流し込むシェーダー
      const std::string str(pCode0100);

      ret = D3DXCompileShader(
        str.c_str(),
        str.length(),
        NULL,
        NULL,
        "VSBaseColor",
        "vs_1_1",
        0,
        &pShader,
        &pErrorMsgs,
        NULL );

    }break;

  case 101:
    { //  頂点と色とUVを流し込むシェーダー
      const std::string code =  "vs.1.1\n"
              "dcl_position v0\n"
              "dcl_color    v1\n"
              "dcl_texcoord v2\n"
              "mov oPos, v0\n"
              "mov oD0,  v1\n"
			        "mov oT0,  v2\n"
             ;
    ret = D3DXAssembleShader( code.c_str(), code.length(), 
        NULL,NULL, 0,
        &pShader,&pErrorMsgs );


    }break;

  default: { MAID_ASSERT( true, "範囲外です " << no ); }break;
  }





  if( FAILED(ret) ) 
  {
    std::string str = (char*)pErrorMsgs->GetBufferPointer();

    pErrorMsgs->Release();
    MAID_ASSERT( true, "エラーが起こるのはおかしい" );
    return ;
  }
  const int len = pShader->GetBufferSize();

  Binary.resize( pShader->GetBufferSize() );
  memcpy( &(Binary[0]), pShader->GetBufferPointer(), pShader->GetBufferSize()  );
  pShader->Release();



/*
  std::string code;

  switch( no )
  {
  case 100:
    { //  頂点と色を流し込むシェーダー
      code = "vs.1.1\n"
             "dcl_position v0\n"
             "dcl_color v1\n"
             "mov oPos, v0\n"
             "mov oD0, v1\n"
             ;
    }break;

  case 101:
    { //  頂点と色とUVを流し込むシェーダー
      code =  "vs.1.1\n"
              "dcl_position v0\n"
              "dcl_color    v1\n"
              "dcl_texcoord v2\n"
              "mov oPos, v0\n"
              "mov oD0,  v1\n"
			        "mov oT0,  v2\n"
             ;
    }break;

  default: { MAID_ASSERT( true, "範囲外です " << no ); }break;
  }

  LPD3DXBUFFER  pShader=NULL;
  LPD3DXBUFFER  pErrorMsgs=NULL;
  HRESULT ret = S_OK;

  ret = D3DXAssembleShader( code.c_str(), code.length(), 
      NULL,NULL, 0,
      &pShader,&pErrorMsgs );

  if( FAILED(ret) ) 
  {
    std::string str = (char*)pErrorMsgs->GetBufferPointer();
//    ErrorMessage    = String::ConvertSJIStoMAID(str);
    pErrorMsgs->Release();
    MAID_ASSERT( true, "エラーが起こるのはおかしい" );
    return ;
  }
  const int len = pShader->GetBufferSize();

  Binary.resize( pShader->GetBufferSize() );
  memcpy( &(Binary[0]), pShader->GetBufferPointer(), pShader->GetBufferSize()  );
  pShader->Release();
*/
}


}}


