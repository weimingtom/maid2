#include"drawcommandexecuted3d09.h"
#include"deviced3d09.h"

#include"vertexd3d09.h"
#include"indexd3d09.h"
#include"constantd3d09.h"
#include"texture2dd3d09.h"
#include"rendertargetd3d09.h"
#include"depthstencild3d09.h"
#include"materiald3d09.h"
#include"inputlayoutd3d09.h"
#include"rasterizerstated3d09.h"
#include"samplerstated3d09.h"
#include"blendstated3d09.h"
#include"vertexshaderd3d09.h"
#include"pixelshaderd3d09.h"
#include"pixelshaderd3d09fixed.h"


//  ここは各種Drawを書く

namespace Maid { namespace Graphics {

void DrawCommandExecuteD3D09::Draw( size_t UseVertexCount, size_t StartVertex )
{
  const D3DPRIMITIVETYPE type = m_PrimitiveType;
  const int count = CalcPrimitiveCount(UseVertexCount);

  const HRESULT ret = m_Device.GetDevice()->DrawPrimitive( type, StartVertex, count );
  if( FAILED(ret) )
  {
    MAID_WARNING(MAIDTEXT("IDirect3DDevice9::DrawPrimitive()"));
    return; 
  }
}

void DrawCommandExecuteD3D09::DrawIndexed( size_t UseIndexCount, size_t StartIndex, size_t OffsetVertex )
{
  const D3DPRIMITIVETYPE type = m_PrimitiveType;
  const int count = CalcPrimitiveCount(UseIndexCount);


  const HRESULT ret = m_Device.GetDevice()->DrawIndexedPrimitive(
                        type,
                        StartIndex,
                        0,
                        m_VertexMax-StartIndex,
                        0,
                        count
                        );


  if( FAILED(ret) ) {MAID_WARNING(MAIDTEXT("IDirect3DDevice9::DrawIndexedPrimitive()")); return; }
}


void DrawCommandExecuteD3D09::CopyResource( const SPRESOURCE& pDst, const SPRESOURCE& pSrc )
{
  MAID_ASSERT( pDst->GetType()!=IResource::TYPE_TEXTURE2D, "テクスチャ専用です" );
  MAID_ASSERT( pSrc->GetType()!=IResource::TYPE_TEXTURE2D, "テクスチャ専用です" );

  const SPD3D09SURFACE& dst = static_cast<Texture2DD3D09*>(pDst.get())->pSurface;
  const SPD3D09SURFACE& src = static_cast<Texture2DD3D09*>(pSrc.get())->pSurface;

	const HRESULT ret = m_Device.GetDevice()->GetRenderTargetData( src.get(), dst.get() );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetRenderTargetData()")); }
}

void DrawCommandExecuteD3D09::GenerateMips( const SPMATERIAL& pMaterial )
{

    MAID_ASSERT( true, "未実装" );
}

}}

