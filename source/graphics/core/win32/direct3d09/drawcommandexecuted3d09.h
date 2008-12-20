#ifndef maid2_graphics_core_win32_direct3d09_drawcommandexecuted3d09_h
#define maid2_graphics_core_win32_direct3d09_drawcommandexecuted3d09_h

#include"../../../../config/define.h"

#include"../../idrawcommandexecute.h"
#include"common.h"


namespace Maid { namespace Graphics {

  /*!	
      @class	DrawCommandExecuteD3D09 drawcommandplayerd3d09.h
      @brief	描画コンテキスト 即時実行タイプ
  */

  class DeviceD3D09;
  class DrawCommandExecuteD3D09 : public IDrawCommandExecute
  {
  public:
    DrawCommandExecuteD3D09( const DeviceD3D09& Device );

    virtual void Begin();
    virtual void End();

    virtual void ClearDepthStencil( CLEARFLAG flag, float Depth, unt08 Stencil );
    virtual void ClearRenderTarget( const COLOR_A32B32G32R32F& Col );

    virtual void ResourceMap( const hResource& Resource, int SubLevel, MAPTYPE Type, unt32 Flag, MAPRESOURCE& Mapped );
    virtual void ResourceUnmap( const hResource& Resource, int SubLevel );

    virtual void SetIndex( const hIndex& Buffer );
    virtual void SetVertex( int Slot, const hVertex& Buffer, int Offset, int Stride );
    virtual void SetInputLayout( const hInputLayout& Layout );

    virtual void SetPrimitiveTopology( PRIMITIVE_TOPOLOGY Topology );

    virtual void VSSetMaterial( int stage, const hMaterial& Material );
    virtual void VSSetSamplerState( int stage, const hSamplerState& Sampler );
    virtual void VSSetShader( const hVertexShader& Shader );

    virtual void PSSetMaterial( int stage, const hMaterial& Material );
    virtual void PSSetSamplerState( int stage, const hSamplerState& Sampler );
    virtual void PSSetShader( const hPixelShader& Shader );

    virtual void SetViewPort( const VIEWPORT& port );
    virtual void SetRasterizerState( const hRasterizerState& State );
    virtual void SetBlendState( const hBlendState& State );
    virtual void SetRenderTarget( int no, const hRenderTarget& Color, const hDepthStencil& Depth );

    virtual void GetRenderTarget( int no, hRenderTarget& Color, hDepthStencil& Depth );

    virtual void Draw( int UseVertexCount, int StartVertex );

  private:
    int CalcPrimitiveCount( int VertexCount );
    const SPD3D09DEVICE& GetDevice();

  private:
    const DeviceD3D09& m_Device;
    D3DPRIMITIVETYPE   m_PrimitiveType;

    hRenderTarget      m_NowRenderTarget[8]; //  現在のターゲット
    hDepthStencil      m_NowDepthStencil[8];
  };

}}

#endif