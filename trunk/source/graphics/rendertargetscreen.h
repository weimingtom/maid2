#ifndef maid2_graphics_rendertargetscreen_h
#define maid2_graphics_rendertargetscreen_h

/*!
    @file
    @brief  スクリーンが持っている RenderTarget
 */


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"rendertargetbase.h"

namespace Maid
{
  class GraphicsCore;
  class RenderTargetScreen
    : public RenderTargetBase
    , private GlobalPointer<GraphicsCore>
  {
  public:
    RenderTargetScreen();

    void Initialize();
    void Finalize();
    void Present();
    void SetAspectSize( const SIZE2DI& size );

  private:
    void Update();


  private:
    SIZE2DI  m_AspectSize;
  };	
	
}

#endif