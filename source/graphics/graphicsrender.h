#ifndef maid2_graphics_graphicsrender_h
#define maid2_graphics_graphicsrender_h

/*!
    @file
    @brief  描画を受け持つクラス
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/mathematics/size.h"

#include"core/idrawcommand.h"
#include"vertex.h"
#include"vertexshader.h"
#include"pixelshader.h"
#include"inputlayout.h"
#include"rasterizerstate.h"
#include"depthstencilstate.h"
#include"blendstate.h"

namespace Maid
{
  class GraphicsRender
  {
  public:
    void Initialize( const Graphics::SPDRAWCOMMAND& pDrawCommand );

    void ClearRenderTarget( const COLOR_R32G32B32A32F& Col );


    void Begin();
    void End();


    //  2D系描画な関数
    //  左上が(0,0)で右下が(w,h)です
    void SetDefaultSetting();
    void SetDepthTest( bool Enable );

    enum BLENDSTATE
    {
      BLENDSTATE_NONE,  //  何もしない
      BLENDSTATE_ALPHA, //  α合成のみ
      BLENDSTATE_ADD,   //  加算
      BLENDSTATE_SUB,   //  減算
      BLENDSTATE_MUL,   //  乗算
    };

    void SetBlendState( BLENDSTATE State );

		void Fill( const RECT2DF&  rc, const COLOR_R32G32B32A32F& Color );

    bool IsLoading() const;

  protected:
    SIZE2DI   m_ScreenSize; //  仮想スクリーンサイズ
    Graphics::SPDRAWCOMMAND m_pDrawCommand;

    Vertex       m_FillVertex;
    VertexShader m_FillVertexShader;
    PixelShader  m_FillPixelShader;
    InputLayout  m_FillLayout;

    RasterizerState m_Rasterizer;

    DepthStencilState m_DepthOn;
    DepthStencilState m_DepthOff;

    BlendState m_BlendNone;
    BlendState m_BlendAlpha;
    BlendState m_BlendAdd;
    BlendState m_BlendSub;
    BlendState m_BlendMul;
  };	
	
}

#endif