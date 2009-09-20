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
    virtual void ClearRenderTarget( const COLOR_R32G32B32A32F& Col );


    virtual void ResourceMap( const SPRESOURCE& Resource, int SubLevel, MAPTYPE Type, unt32 Flag, MAPPEDRESOURCE& Mapped );
    virtual void ResourceUnmap( const SPRESOURCE& Resource, int SubLevel );
    virtual void SetIndex( const SPBUFFER& Buffer, size_t Offset );
    virtual void SetVertex( int Slot, const SPBUFFER& Buffer, int Offset, int Stride );
    virtual void SetInputLayout( const SPINPUTLAYOUT& Layout );
    virtual void SetPrimitiveTopology( PRIMITIVE_TOPOLOGY Topology );

    virtual void VSSetConstant( int slot, const SPBUFFER& pBuffer );
    virtual void VSSetMaterial( int stage, const SPMATERIAL& Material );
    virtual void VSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler );
    virtual void VSSetShader( const SPVERTEXSHADER& Shader );

    virtual void PSSetConstant( int slot, const SPBUFFER& pBuffer );
    virtual void PSSetMaterial( int stage, const SPMATERIAL& Material );
    virtual void PSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler );
    virtual void PSSetShader( const SPPIXELSHADER& Shader );

    virtual void SetViewPort( const VIEWPORT& port );
    virtual void SetRasterizerState( const SPRASTERIZERSTATE& State );
    virtual void SetDepthStencilState( const SPDEPTHSTENCILSTATE& State );
    virtual void SetBlendState( const SPBLENDSTATE& State );
    virtual void SetRenderTarget( int no, const SPRENDERTARGET& Color, const SPDEPTHSTENCIL& Depth );

    virtual VIEWPORT GetViewport();
    virtual void GetRenderTarget( int no, SPRENDERTARGET& Color, SPDEPTHSTENCIL& Depth );

    virtual void Draw( size_t UseVertexCount, size_t StartVertex );
    virtual void DrawIndexed( size_t UseIndexCount, size_t StartIndex, size_t OffsetVertex );
    virtual void CopyResource( const SPRESOURCE& pDst, const SPRESOURCE& pSrc );
    virtual void GenerateMips( const SPMATERIAL& pMaterial );

  private:
    int CalcPrimitiveCount( int VertexCount );
    const SPD3D09DEVICE& GetDevice();

  private:
    const DeviceD3D09& m_Device;
    D3DPRIMITIVETYPE   m_PrimitiveType;

    SPRENDERTARGET      m_NowRenderTarget[8]; //  現在のターゲット
    SPDEPTHSTENCIL      m_NowDepthStencil[8];

    size_t  m_IndexOffset;
    size_t  m_VertexMax;

  };

}}

#endif