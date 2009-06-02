#ifndef maid2_graphics_graphicscommandcontrol_h
#define maid2_graphics_graphicscommandcontrol_h

/*!
    @file
    @brief  描画ではなく初期設定を行うクラス
 */


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"

#include"graphicscore.h"

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

    void Begin();
    void End();

    void SetRenderTarget( const RenderTargetBase& Target );
    void SetRenderTarget( const RenderTargetBase& Target, const IDepthStencil& Depth );

    Graphics::IDrawCommand& GetCommand();

    float GetTexelMapValue() const;

  protected:
    Graphics::SPDRAWCOMMAND m_pDrawCommand;

    float m_TexelMapValue;
  };
}

#endif