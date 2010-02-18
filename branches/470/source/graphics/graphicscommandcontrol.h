#ifndef maid2_graphics_graphicscommandcontrol_h
#define maid2_graphics_graphicscommandcontrol_h

/*!
    @file
    @brief  描画ではなく初期設定を行うクラス
 */


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"
#include"../framework/movie/core/isample.h"

#include"graphicscore.h"
#include"texture2ddynamic.h"
#include"texture2dmemory.h"
#include"surface.h"

namespace Maid
{
  class GraphicsCommandControl
    : public GlobalPointer<GraphicsCore>
  {
  public:
    GraphicsCommandControl();
    virtual ~GraphicsCommandControl();

    void Initialize();
    void Initialize( const Graphics::SPDRAWCOMMAND& pCommand );

    void ClearRenderTarget( const COLOR_R32G32B32A32F& Col );
    void ClearDepth( float Depth );
    void ClearStencil( unt08 Stencil );
    void ClearDepthStencil( float Depth, unt08 Stencil );

    void ClearRenderTarget();
    void ClearDepth();
    void ClearDepthStencil();

    void Begin();
    void End();

    void SetRenderTarget( const RenderTargetBase& Target );
    void SetRenderTarget( const RenderTargetBase& Target, const IDepthStencil& Depth );

    Graphics::IDrawCommand& GetCommand();

    float GetTexelMapValue() const;


    enum MAPTYPE
    {
      MAPTYPE_READ = Graphics::IDrawCommand::MAPTYPE_READ,
      MAPTYPE_WRITE = Graphics::IDrawCommand::MAPTYPE_WRITE,
      MAPTYPE_READ_WRITE = Graphics::IDrawCommand::MAPTYPE_READ_WRITE,
      MAPTYPE_WRITE_DISCARD = Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD,
      MAPTYPE_WRITE_NO_OVERWRITE = Graphics::IDrawCommand::MAPTYPE_WRITE_NO_OVERWRITE,
    };

    enum MAPFLAG
    {
      MAPFLAG_DO_NOT_WAIT = Graphics::IDrawCommand::MAPFLAG_DO_NOT_WAIT,
    };
    struct MAPPEDRESOURCE
    {
      void *pData;
      int Pitch;
      int Slice;

      MAPPEDRESOURCE()
        :pData(NULL)
        ,Pitch(0)
        ,Slice(0)
      {}
    };

    void ResourceMap( const GraphicsObject<Graphics::SPBUFFER>& res, int SubLevel, MAPTYPE Type, unt32 Flag, MAPPEDRESOURCE& Mapped );
    void ResourceUnmap( const GraphicsObject<Graphics::SPBUFFER>& res, int SubLevel );

    void SetIndex( const IIndex& Buffer, size_t Offset );
    void SetVertex( int Slot, const IVertex& Buffer, int Offset, int Stride );
    void SetInputLayout( const IInputLayout& Layout );

    enum PRIMITIVE_TOPOLOGY
    {
      PRIMITIVE_TOPOLOGY_POINTLIST = Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_POINTLIST,
      PRIMITIVE_TOPOLOGY_LINELIST = Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_LINELIST,
      PRIMITIVE_TOPOLOGY_LINESTRIP = Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_LINESTRIP,
      PRIMITIVE_TOPOLOGY_TRIANGLELIST = Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLELIST,
      PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
    };
    void SetPrimitiveTopology( PRIMITIVE_TOPOLOGY Topology );

    void VSSetConstant( int slot, const IConstant& buffer );
    void VSSetMaterial( int stage, const IMaterial& mat );
    void VSSetSamplerState( int stage, const ISamplerState& sampler );
    void VSSetShader( const IVertexShader& shader );

    void PSSetConstant( int slot, const IConstant& buffer );
    void PSSetMaterial( int stage, const IMaterial& mat );
    void PSSetSamplerState( int stage, const ISamplerState& sampler );
    void PSSetShader( const IPixelShader& shader );

    void SetRasterizerState( const IRasterizerState& state );
    void SetDepthStencilState( const IDepthStencilState& state );
    void SetBlendState( const IBlendState& state );

    struct VIEWPORT
    {
      RECT2DI Screen; //!<  表示範囲
      float   MinZ;   //!<  カリングＺ
      float   MaxZ;   //!<  カリングＺ

      VIEWPORT():Screen(0,0,0,0),MinZ(0),MaxZ(1){}
      VIEWPORT( const RECT2DI& s, float min, float max ):Screen(s),MinZ(min),MaxZ(max){}
    };
    void SetViewPort( const VIEWPORT& port );
    VIEWPORT GetViewport()const;
    void Draw( size_t UseVertexCount, size_t StartVertex );
    void DrawIndexed( size_t UseIndexCount, size_t StartIndex, size_t OffsetVertex );

    void UpdateTexture( Texture2DDynamic& dst, const Movie::SPSAMPLEFRAME& pSrc );
    void UpdateTexture( Texture2DDynamic& dst, const SurfaceInstance& Src );
    void CopyRenderTarget( SurfaceInstance& dst, const RenderTargetBase& src );


  protected:
    Graphics::SPDRAWCOMMAND m_pDrawCommand;

    float m_TexelMapValue;

    int m_BeginCount;

  };
}

#endif