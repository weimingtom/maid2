#include"graphicsrender.h"




namespace Maid
{
void GraphicsRender::Initialize( const Graphics::SPDRAWCOMMAND& pDrawCommand )
{
  m_pDrawCommand = pDrawCommand;
}

void GraphicsRender::Finalize()
{

}


void GraphicsRender::ClearRenderTarget( const COLOR_A32B32G32R32F& Col )
{
  m_pDrawCommand->ClearRenderTarget( Col );
}
	
}