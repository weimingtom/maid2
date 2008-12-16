#ifndef maid2_graphics_graphicsrender_h
#define maid2_graphics_graphicsrender_h

/*!
    @file
    @brief  描画を受け持つクラス
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"

#include"core/idrawcommand.h"

namespace Maid
{
  class GraphicsRender
  {
  public:
    void Initialize( const Graphics::SPDRAWCOMMAND& pDrawCommand );
    void Finalize();

    void ClearRenderTarget( const COLOR_A32B32G32R32F& Col );

  protected:
    Graphics::SPDRAWCOMMAND m_pDrawCommand;
  };	
	
}

#endif