#include"depthstencilstate.h"


namespace Maid
{

	/*!
	 	@class	DepthStencilState depthstencilstate.h
	 	@brief	DepthStencilState
	 */ 


//! 指定した設定で作成する
/*!
    @param state  [i ]  作成する状態
 */
void DepthStencilState::Create( const Graphics::DEPTHSTENCILSTATEPARAM& state )
{
  Delete();
  const Graphics::SPDEVICE& pDevice = GlobalPointer<GraphicsCore>::Get()->GetDevice();

  Graphics::SPDEPTHSTENCILSTATE obj = pDevice->CreateDepthStencilState( state );
  IDepthStencilState::Set( obj );
}

//! 設定を開放する
/*!
 */
void DepthStencilState::Delete()
{
  IDepthStencilState::Clear();
}

}