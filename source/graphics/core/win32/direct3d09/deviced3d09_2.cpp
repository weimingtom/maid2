#include"deviced3d09.h"

namespace Maid { namespace Graphics {

hVertex DeviceD3D09::CreateVertex( const CREATEVERTEXPARAM& param )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  const UINT  length = param.Length;
  const DWORD usage = 0;
  const DWORD fvf = VERTEXFORMAT2FVF( param.Format );
  const D3DPOOL pool = D3DPOOL_DEFAULT;

  IDirect3DVertexBuffer9* p=NULL;
  {
    const HRESULT ret = pDev->CreateVertexBuffer( length, usage, fvf, pool, &p, NULL );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreateVertexBuffer()")); goto MAID_LABEL; }
  }

  {
    VOID* pDst = NULL;

    {
      const HRESULT ret = p->Lock( 0, 0, &pDst, D3DLOCK_DISCARD );
      if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::Lock()")); goto MAID_LABEL; }
    }

    ::memcpy( pDst, param.pData, param.Length );

    {
      const HRESULT ret = p->Unlock();
      if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::Unlock()")); goto MAID_LABEL; }
    }
  }

  return hVertex((hObject::INSTANCEID)p);

MAID_LABEL:
  if( p!=NULL ) { p->Release(); }
  return hVertex(hObject::ID_NONE);
}


hIndex DeviceD3D09::CreateIndex( const CREATEREINDEXPARAM& param )
{
  return hIndex(hObject::ID_NONE);
}

hTexture2D DeviceD3D09::CreateTexture2D( const CREATERETEXTURE2DPARAM& param )
{
  return hTexture2D(hObject::ID_NONE);
}

hConstant DeviceD3D09::CreateConstant( const CREATERECONSTANTPARAM& param )
{
  return hConstant(hObject::ID_NONE);
}



hRenderTarget DeviceD3D09::CreateRenderTarget( const hResource& resource, const CREATERENDERTARGETPARAM& param )
{
  return hRenderTarget(hObject::ID_NONE);
}

hDepthStencil DeviceD3D09::CreateDepthStencil( const hResource& resource, const CREATEDEPTHSTENCILPARAM& param )
{
  return hDepthStencil(hObject::ID_NONE);
}
hMaterial DeviceD3D09::CreateShaderMaterial( const hResource& resource, const CREATESHADERMATERIALPARAM& param )
{
  return hMaterial(hObject::ID_NONE);
}

hVertexShader DeviceD3D09::CreateVertexShader( const void* pData, size_t Length )
{
  return hVertexShader(hObject::ID_NONE);
}
hPixelShader DeviceD3D09::CreatePixelShader( const void* pData, size_t Length )
{
  return hPixelShader(hObject::ID_NONE);
}

hSamplerState DeviceD3D09::CreateSamplerState( const SAMPLERSTATEPARAM& Option )
{
  return hSamplerState(hObject::ID_NONE);
}

hRasterizerState DeviceD3D09::CreateRasterizerState( const RASTERIZERSTATEPARAM& Option )
{
  return hRasterizerState(hObject::ID_NONE);
}

hBlendState DeviceD3D09::CreateBlendState( const BLENDSTATEPARAM& Option )
{
  return hBlendState(hObject::ID_NONE);
}

hInputLayout DeviceD3D09::CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )
{
  return hInputLayout(hObject::ID_NONE);
}


}}


