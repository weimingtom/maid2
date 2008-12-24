#include"graphicscore.h"


Maid::GraphicsCore*  Maid::GlobalPointer<Maid::GraphicsCore>::s_pPointer;

namespace Maid {

	/*!
	 	@class Core core.h
	 	@brief  グラフィック周りを管理するクラス
	 */


GraphicsCore::GraphicsCore()
{
}

GraphicsCore::~GraphicsCore()
{
	Finalize();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 初期化
/*!
	@param	pDriver	[i ]	使うドライバ
 */
void GraphicsCore::Initialize( const SPDEVICE& pDevice )
{
  GlobalPointer<Core>::Set();

	m_pDevice = pDevice;
	m_pDevice->Initialize();
  UpdateMember();
}

void GraphicsCore::Finalize()
{
	if( m_pDevice.get()==NULL ) { return ; }

	m_pDevice->Finalize();
	m_pDevice.reset();
  GlobalPointer<Core>::Clear();
}

const SPDEVICE& Core::GetDevice()
{
  return m_pDevice;
}


PIXELFORMAT GraphicsCore::GetUseFormat( PIXELFORMAT& fmt )const
{
  PIXELFORMAT ret = PIXELFORMAT_NONE;



  return ret;
}


void GraphicsCore::UpdateMember()
{

}


}

