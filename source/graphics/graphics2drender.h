#ifndef maid2_graphics_graphics2drender_h
#define maid2_graphics_graphics2drender_h

/*!
    @file
    @brief  描画を受け持つクラス
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/mathematics/size.h"
#include"../auxiliary/globalpointer.h"


#include"graphicscore.h"
#include"vertex.h"
#include"texture2dbase.h"
#include"texture2d.h"
#include"vertexshader.h"
#include"pixelshader.h"
#include"inputlayout.h"
#include"rasterizerstate.h"
#include"depthstencilstate.h"
#include"blendstate.h"
#include"font.h"
#include"fonttexturemanager.h"
#include"rendertargetbase.h"
#include"samplerstate.h"


#include"graphicscommandcontrol.h"


namespace Maid
{
  class Graphics2DRender
    : public GlobalPointer<GraphicsCore>
  {
  public:
    Graphics2DRender( GraphicsCommandControl& com );
    virtual ~Graphics2DRender();

    void Initialize();
    void Initialize( const SIZE2DI& ScreenSize );

    void SetVirtualScreenSize( const SIZE2DI& Size );

    //  2D系描画な関数
    //  左上が(0,0)で右下が(w,h)です
    void SetDefaultSetting();

    void UpdateFrame();

    enum BLENDSTATE
    {
      BLENDSTATE_NONE=0,  //  何もしない
      BLENDSTATE_ALPHA, //  α合成のみ
      BLENDSTATE_ADD,   //  加算
      BLENDSTATE_SUB,   //  減算
      BLENDSTATE_MUL,   //  乗算
      BLENDSTATE_SCREEN,   //  スクリーン
      BLENDSTATE_DARKEN,   //  比較（暗）
      BLENDSTATE_LIGHTTEN, //  比較（明）
      BLENDSTATE_REVERSE,   //  反転
    };

    enum TEXTUREFILTER
    {
      TEXTUREFILTER_POINT,
      TEXTUREFILTER_LINEAR,
    };

    void SetFilterState( TEXTUREFILTER filter );
    void SetBlendState( BLENDSTATE State );

    void Fill  ( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center );
    void FillS ( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale );
    void FillR ( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, float Rotate, const VECTOR3DF& vec );
    void FillSR( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec );
    void FillMatrix( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, const MATRIX4DF& mat );

    void Blt  ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha );
    void BltS ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale );
    void BltR ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, float Rotate, const VECTOR3DF& vec );
    void BltSR( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec );
    void Blt  ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const MATRIX4DF& mat );

    void BltMask  ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const Texture2DBase& Mask );
    void BltMaskS ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, const Texture2DBase& Mask );
    void BltMaskR ( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, float Rotate, const VECTOR3DF& vec, const Texture2DBase& Mask );
    void BltMaskSR( const POINT2DI& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec, const Texture2DBase& Mask );

    void BltMix  ( const POINT2DI& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow );
    void BltMixS ( const POINT2DI& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow );
    void BltMixR ( const POINT2DI& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, float Rotate, const VECTOR3DF& vec, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow );
    void BltMixSR( const POINT2DI& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow );

    void BltText( const POINT2DI& Base, const Font& f, const String& Text, const COLOR_R32G32B32A32F& pow, size_t Len );
    void BltText( const POINT2DI& Base, const Font& f, const String& Text, const COLOR_R32G32B32A32F& pow );
    void BltText( const POINT2DI& Base, const Font& f, const String& Text );
		void AddGaizi( unt32 no, const Texture2D& tex );

    bool IsInitializing() const;


    struct MODELINFO
    {
      VECTOR4DF Model[4];   //  座標情報
      MATRIX4DF Translate;  //  移動量をあらわした行列
      float     Acpect;
    };

    void CreateVirtualScreenModel( const POINT2DI& Pos, const SIZE2DI& Size, const POINT2DI& Center, MODELINFO& Model );

    void Fill( const VECTOR4DF* Model, const MATRIX4DF& mat, const COLOR_R32G32B32A32F& Color );
    void Blt( const VECTOR4DF* Model, const MATRIX4DF& mat, const Texture2DBase& Texture, const RECT2DI& rc, const COLOR_R32G32B32A32F& Color );
    void BltMask( const VECTOR4DF* Model, const MATRIX4DF& mat, const Texture2DBase& Texture, const RECT2DI& rc, const COLOR_R32G32B32A32F& Color, const Texture2DBase& Mask );
    void BltMix ( const VECTOR4DF* Model, const MATRIX4DF& mat, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, float alpha, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow );
  protected:

    void SetupVertex( const VECTOR4DF* Model, const MATRIX4DF& Mat, void* pVertex, int Stride );
    void SetCommonVertex( void* pVertex, int size, int Stride );


	  void SetupState();
    const Texture2DBase&	GetFont( const Font& pFont, unt32 Code );

  protected:
    bool IsMemberLoading() const;
    Graphics::IDrawCommand& GetCommand();

    enum STATE
    {
      STATE_EMPTY,    //  まだ初期化されてない
      STATE_LOADING,  //  初期設定中
      STATE_WORKING,  //  稼動できる
    };

    STATE m_State;

    BLENDSTATE		m_BltState;
    TEXTUREFILTER	m_Filter;
    SIZE2DI   m_VirtualScreenSize; //  仮想スクリーンサイズ

		typedef std::map<unt32,Texture2D> GAIZIMAP;
		GAIZIMAP			m_GaiziMap;

  protected:

    GraphicsCommandControl&  m_CommandCtl;

    FontTextureManager  m_FontManager;

    Vertex       m_CommonVertex;  //  一時的な書き込みで使うバッファ

    InputLayout  m_FillLayout;
    VertexShader m_FillVertexShader;
    PixelShader  m_FillPixelShader;

    InputLayout  m_BltLayout;
    VertexShader m_BltVertexShader;
    PixelShader  m_BltPixelShader210;
    PixelShader  m_BltPixelShader211;
    PixelShader  m_BltPixelShader212;

    InputLayout  m_BltMaskLayout;
    VertexShader m_BltMaskVertexShader;
    PixelShader  m_BltMaskPixelShader220;
    PixelShader  m_BltMaskPixelShader221;
    PixelShader  m_BltMaskPixelShader222;
    PixelShader  m_BltMaskPixelShader223;
    PixelShader  m_BltMaskPixelShader224;
    PixelShader  m_BltMaskPixelShader225;

    InputLayout  m_BltMixLayout;
    VertexShader m_BltMixVertexShader;
    PixelShader  m_BltMixPixelShader230;
    PixelShader  m_BltMixPixelShader231;
    PixelShader  m_BltMixPixelShader232;



    RasterizerState m_Rasterizer;

    SamplerState  m_SamplerPoint;
    SamplerState  m_SamplerLinar;

    DepthStencilState m_DepthOff;

    std::vector<BlendState> m_BlendState;

  };	
	
}

#endif