#ifndef maid2_graphics_core_win32_direct3d09_drawcommandplayerd3d09_h
#define maid2_graphics_core_win32_direct3d09_drawcommandplayerd3d09_h

#include"../../../../config/define.h"

#include"../../idrawcommandplayer.h"
#include"d3d09include.h"
#include"object.h"

namespace Maid { namespace Graphics {

  /*!	
      @class	DrawCommandPlayerD3D09 drawcommandplayerd3d09.h
      @brief	描画コンテキスト 即時実行タイプ
  */

  class DrawCommandPlayerD3D09 : public IDrawCommandPlayer
  {
  public:
    DrawCommandPlayerD3D09( const SPD3D09DEVICE& pDev, const std::vector<SPOBJECTINFO>& ObjectList );

    virtual void Begin();
    virtual void End();

    virtual void ClearDepthStencil( CLEARFLAG flag, float Depth, unt08 Stencil );
    virtual void ClearRenderTarget( const COLOR_A32B32G32R32F& Col );

    virtual void SetIndex( const IDevice::RESOURCE& Buffer );
    virtual void SetVertex( int Slot, const IDevice::RESOURCE& Buffer );
    virtual void SetInputLayout( const IDevice::INPUTLAYOUT& Layout );

    virtual void VSSetMaterial( int stage, const IDevice::SHADERMATERIAL& Buffer );
    virtual void VSSetShader( const IDevice::VERTEXSHADER& Shader );
    virtual void VSSetSamplerState( const IDevice::SAMPLERSTATE& Sampler );

    virtual void PSSetMaterial( int stage, const IDevice::SHADERMATERIAL& Buffer );
    virtual void PSSetShader( const IDevice::PIXELSHADER& Shader );
    virtual void PSSetSamplerState( const IDevice::SAMPLERSTATE& Sampler );

    virtual void SetViewPort( const RECT2DI& screen, float MinZ, float MaxZ );
    virtual void SetRasterizerState( const IDevice::RASTERIZERSTATE& State );
    virtual void SetBlendState( const IDevice::BLENDSTATE& State );
    virtual void SetRenderTarget( int no, const IDevice::RENDERTARGET& Color, const IDevice::DEPTHSTENCIL Depth );
    virtual void ResetRenderTarget( int no );

  private:
    const SPD3D09DEVICE&                m_pDevice;
    const std::vector<SPOBJECTINFO>& m_ObjectList;

  };

}}

#endif