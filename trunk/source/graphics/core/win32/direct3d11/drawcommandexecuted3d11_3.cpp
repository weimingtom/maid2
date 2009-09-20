#include"drawcommandexecuted3d11.h"
#include"deviced3d11.h"

#include"bufferd3d11.h"
#include"texture2dd3d11.h"
#include"rendertargetd3d11.h"
#include"depthstencild3d11.h"
#include"materiald3d11.h"
#include"inputlayoutd3d11.h"
#include"rasterizerstated3d11.h"
#include"samplerstated3d11.h"
#include"blendstated3d11.h"
#include"vertexshaderd3d11.h"
#include"pixelshaderd3d11.h"
#include"debug.h"


//  ここは各種Drawを書く

namespace Maid { namespace Graphics {

void DrawCommandExecuteD3D11::Draw( size_t UseVertexCount, size_t StartVertex )
{
  m_pDevice->Draw( UseVertexCount, StartVertex );
}

void DrawCommandExecuteD3D11::DrawIndexed( size_t UseIndexCount, size_t StartIndex, size_t OffsetVertex )
{
  m_pDevice->DrawIndexed( UseIndexCount, StartIndex, OffsetVertex );
}

ID3D11Resource* DrawCommandExecuteD3D11::GetResource( const SPRESOURCE& pResource )
{
  ID3D11Resource* pRet = NULL;

  switch( pResource->GetType() )
  {
  case IResource::TYPE_BUFFER:
    {
      BufferD3D11* pBuffer = static_cast<BufferD3D11*>( pResource.get() );
      pRet = pBuffer->pBuffer.get();
    }break;

  case IResource::TYPE_TEXTURE2D:
    {
      Texture2DD3D11* pBuffer = static_cast<Texture2DD3D11*>( pResource.get() );
      pRet = pBuffer->pTexture.get();
    }break;
  }

  return pRet;
}


void DrawCommandExecuteD3D11::CopyResource( const SPRESOURCE& pDstResource, const SPRESOURCE& pSrcResource )
{
  ID3D11Resource* pDst = GetResource(pDstResource);
  ID3D11Resource* pSrc = GetResource(pSrcResource);

  m_pDevice->CopyResource( pDst, pSrc );
}


void DrawCommandExecuteD3D11::GenerateMips( const SPMATERIAL& pMaterial )
{
  ID3D11ShaderResourceView* pView = static_cast<MaterialD3D11*>(pMaterial.get())->pView.get();
  m_pDevice->GenerateMips( pView );
}


}}

