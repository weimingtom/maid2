#include"graphiccore.h"



namespace Maid
{

	/*!
	 	@class GraphicCore graphiccore.h
	 	@brief  グラフィック周りを管理するクラス
            このクラスはコア部分なので、低レベルな機能しかありません
	 */

GraphicCore::GraphicCore()
{
}

GraphicCore::~GraphicCore()
{
	Finalize();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 初期化
/*!
	@param	pDriver	[i ]	使うドライバ
 */
void GraphicCore::Initialize( const SPVIDEODEVICE& pDevice )
{
	m_pDevice = pDevice;
	m_pDevice->Initialize();
}

void GraphicCore::Finalize()
{
	if( m_pDevice.get()==NULL ) { return ; }

	{
		//	残っているデータがないか確認
		for( SURFACEMAP::iterator ite=m_SurfaceMap.begin(); ite!=m_SurfaceMap.end(); ++ite )
		{
			MAID_ASSERT( true, ite->first << MAIDTEXT("が残っています") );
		}
		for( TEXTUREMAP::iterator ite=m_TextureMap.begin(); ite!=m_TextureMap.end(); ++ite )
		{
			MAID_ASSERT( true, ite->first << MAIDTEXT("が残っています") );
		}
		for( INDEXMAP::iterator ite=m_IndexMap.begin(); ite!=m_IndexMap.end(); ++ite )
		{
			MAID_ASSERT( true, ite->first << MAIDTEXT("が残っています") );
		}
		for( VERTEXMAP::iterator ite=m_VertexMap.begin(); ite!=m_VertexMap.end(); ++ite )
		{
			MAID_ASSERT( true, ite->first << MAIDTEXT("が残っています") );
		}
		for( VERTEXSHADERMAP::iterator ite=m_VertexShaderMap.begin(); ite!=m_VertexShaderMap.end(); ++ite )
		{
			MAID_ASSERT( true, ite->first << MAIDTEXT("が残っています") );
		}
		for( PIXELSHADERMAP::iterator ite=m_PixelShaderMap.begin(); ite!=m_PixelShaderMap.end(); ++ite )
		{
			MAID_ASSERT( true, ite->first << MAIDTEXT("が残っています") );
		}
	}

	m_pDevice->Finalize();
	m_pDevice.reset();
}

void GraphicCore::SwapQue( RENDERMESSAGEQUE& Send, RETURNMESSAGEQUE& Return )
{
  Send.swap(m_RenderQue);
  Return.swap(m_ReturnQue);
}


void GraphicCore::DrawFrame()
{
	m_pDevice->BeginScene();
  for( int i=0; i<(int)m_RenderQue.size(); ++i )
  {
    const SPRENDERMESSAGE& pMess = m_RenderQue[i];
    ExecuteMessage( pMess );
  }
	m_pDevice->EndScene();
	m_pDevice->Flip();

  m_RenderQue.resize(0);
}


}

