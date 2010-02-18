#include"drawcommandexecuted3d10.h"
#include"deviced3d10_0.h"

#include"bufferd3d10.h"
#include"texture2dd3d10.h"
#include"rendertargetd3d10.h"
#include"depthstencild3d10.h"
#include"materiald3d10.h"
#include"inputlayoutd3d10.h"
#include"rasterizerstated3d10.h"
#include"samplerstated3d10.h"
#include"blendstated3d10.h"
#include"vertexshaderd3d10.h"
#include"pixelshaderd3d10.h"
#include"debug.h"


//  ここは各種Drawを書く

namespace Maid { namespace Graphics {

void DrawCommandExecuteD3D10::Draw( size_t UseVertexCount, size_t StartVertex )
{
  GetDevice()->Draw( UseVertexCount, StartVertex );
}

void DrawCommandExecuteD3D10::DrawIndexed( size_t UseIndexCount, size_t StartIndex, size_t OffsetVertex )
{
  GetDevice()->DrawIndexed( UseIndexCount, StartIndex, OffsetVertex );
}

ID3D10Resource* DrawCommandExecuteD3D10::GetResource( const SPRESOURCE& pResource )
{
  ID3D10Resource* pRet = NULL;

  switch( pResource->GetType() )
  {
  case IResource::TYPE_BUFFER:
    {
      BufferD3D10* pBuffer = static_cast<BufferD3D10*>( pResource.get() );
      pRet = pBuffer->pBuffer.get();
    }break;

  case IResource::TYPE_TEXTURE2D:
    {
      Texture2DD3D10* pBuffer = static_cast<Texture2DD3D10*>( pResource.get() );
      pRet = pBuffer->pTexture.get();
    }break;
  }

  return pRet;
}


void DrawCommandExecuteD3D10::CopyResource( const SPRESOURCE& pDstResource, const SPRESOURCE& pSrcResource )
{
  ID3D10Resource* pDst = GetResource(pDstResource);
  ID3D10Resource* pSrc = GetResource(pSrcResource);

  GetDevice()->CopyResource( pDst, pSrc );
}


void DrawCommandExecuteD3D10::GenerateMips( const SPMATERIAL& pMaterial )
{
  ID3D10ShaderResourceView* pView = static_cast<MaterialD3D10*>(pMaterial.get())->pView.get();

  GetDevice()->GenerateMips( pView );
}

}}

