#include"pixelshaderbufferd3d09.h"


namespace Maid
{

PixelShaderBufferD3D09HARD::PixelShaderBufferD3D09HARD( IDirect3DPixelShader9* p )
{
  pBuffer.reset( p );
}

void PixelShaderBufferD3D09HARD::Setup( const SPD3DDEVICE& pDevice )
{
  HRESULT ret = pDevice->SetPixelShader( pBuffer.get() );
 	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::SetPixelShader()")); }
}
	





//  以下固定パイプライン向けシェーダー
void PixelShaderBufferD3D09_NORMAL::Setup( const SPD3DDEVICE& pDevice )
{

}


}

