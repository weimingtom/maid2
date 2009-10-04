#include"graphics2drender.h"
#include"customvertextemplate.h"




namespace Maid
{

  /*!
      @class  Graphics2DRender Graphics2DRender.h
      @brief  標準的な描画ルーチンを集めたクラス
  \n          ここに書いてある感じで各種ゲームに追加実装するのが吉
  */

Graphics2DRender::Graphics2DRender( GraphicsCommandControl& com )
  :m_State(STATE_EMPTY)
  ,m_BltState(BLENDSTATE_NONE)
  ,m_Filter(TEXTUREFILTER_LINEAR)
  ,m_CommandCtl(com)
{

}

Graphics2DRender::~Graphics2DRender()
{

}

//! このクラスを使えるように初期化
/*!
    設定を勝手に調べて、自動的に行います
*/
void Graphics2DRender::Initialize()
{
  GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get();

  const SIZE2DI Size = pCore->GetSwapChainFormat().Size;

  Initialize( Size );
}

//! このクラスを使えるように初期化
/*!
    この関数を呼んだあと、IsInitializing() がfalseを返すまで待機すること

    @param  ScreenSize  [i ]  スクリーンサイス
 */
void Graphics2DRender::Initialize( const SIZE2DI& ScreenSize )
{
  {
    //  1頂点あたり128バイトも使わないでしょう
    m_CommonVertex.Create( 128*4 );
  }

  {
    {
      const String vs = MAIDTEXT("0100");
      m_FillVertexShader.Create( vs );

      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };

      m_FillLayout.Create( element, NUMELEMENTS(element), vs );
    }

    {
      m_FillPixelShader.Create( MAIDTEXT("0200") );
    }
  }

  {
    {
      const String vs = MAIDTEXT("0101");

      m_BltVertexShader.Create( vs );

      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"TEXCOORD", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT2, 0, 28, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };

      m_BltLayout.Create( element, NUMELEMENTS(element), vs );
    }

    {
      m_BltPixelShader210.Create( MAIDTEXT("0210") );
      m_BltPixelShader211.Create( MAIDTEXT("0211") );
      m_BltPixelShader212.Create( MAIDTEXT("0212") );
    }
  }

  {
    {
      const String vs = MAIDTEXT("0102");

      m_BltMaskVertexShader.Create( vs );

      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"TEXCOORD", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT2, 0, 28, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"TEXCOORD", 1, Graphics::INPUT_ELEMENT::TYPE_FLOAT2, 0, 36, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };

      m_BltMaskLayout.Create( element, NUMELEMENTS(element), vs );
    }

    {
      m_BltMaskPixelShader220.Create( MAIDTEXT("0220") );
      m_BltMaskPixelShader221.Create( MAIDTEXT("0221") );
      m_BltMaskPixelShader222.Create( MAIDTEXT("0222") );
      m_BltMaskPixelShader223.Create( MAIDTEXT("0223") );
      m_BltMaskPixelShader224.Create( MAIDTEXT("0224") );
      m_BltMaskPixelShader225.Create( MAIDTEXT("0225") );
    }
  }

  {
    {
      const String vs = MAIDTEXT("0102");

      m_BltMixVertexShader.Create( vs );

      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"TEXCOORD", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT2, 0, 28, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"TEXCOORD", 1, Graphics::INPUT_ELEMENT::TYPE_FLOAT2, 0, 36, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };

      m_BltMixLayout.Create( element, NUMELEMENTS(element), vs );
    }

    {
      m_BltMixPixelShader230.Create( MAIDTEXT("0230") );
      m_BltMixPixelShader231.Create( MAIDTEXT("0231") );
      m_BltMixPixelShader232.Create( MAIDTEXT("0232") );
    }
  }


  {
    Graphics::RASTERIZERSTATEPARAM param;

    param.Fill = Graphics::RASTERIZERSTATEPARAM::FILL_SOLID;
    param.Culling = Graphics::RASTERIZERSTATEPARAM::CULLING_NONE;
    param.MultiSample = false;

    m_Rasterizer.Create( param );
  }
  {
    Graphics::DEPTHSTENCILSTATEPARAM param;
    param.DepthTest = false;
    m_DepthOff.Create( param );
  }

  {
    m_BlendState.resize( 9 );

    m_BlendState[0].Create( BlendState::TEMPLATE_NORMAL );
    m_BlendState[1].Create( BlendState::TEMPLATE_ALPHA );
    m_BlendState[2].Create( BlendState::TEMPLATE_ADD );
    m_BlendState[3].Create( BlendState::TEMPLATE_SUB );
    m_BlendState[4].Create( BlendState::TEMPLATE_MUL );
    m_BlendState[5].Create( BlendState::TEMPLATE_SCREEN );
    m_BlendState[6].Create( BlendState::TEMPLATE_DARKEN );
    m_BlendState[7].Create( BlendState::TEMPLATE_LIGHTTEN );
    m_BlendState[8].Create( BlendState::TEMPLATE_ALPHA );
  }

  {
    Graphics::SAMPLERSTATEPARAM state;
    state.MinFilter = Graphics::SAMPLERSTATEPARAM::FILTER_LINEAR;
    state.MagFilter = Graphics::SAMPLERSTATEPARAM::FILTER_LINEAR;
    state.MipFilter = Graphics::SAMPLERSTATEPARAM::FILTER_LINEAR;

    m_SamplerLinar.Create( state );

    state.MinFilter = Graphics::SAMPLERSTATEPARAM::FILTER_POINT;
    state.MagFilter = Graphics::SAMPLERSTATEPARAM::FILTER_POINT;
    state.MipFilter = Graphics::SAMPLERSTATEPARAM::FILTER_POINT;

    m_SamplerPoint.Create( state );
  }

  {
    m_VirtualScreenSize = ScreenSize;
  }
  {
    m_FontManager.Initialize();
    m_FontManager.SetCacheSize(1024);
  }

  m_State = STATE_LOADING;
}

void Graphics2DRender::UpdateFrame()
{
  m_FontManager.Garbage();
}


//! 仮想サイズの変更
/*!
    ゲーム中の描画に使っている座標をすべて仮想サイズで決めることで
    解像度の違いをすべて吸収できるようになります。

    @param  Size [i ] 新しいサイズ
 */
void Graphics2DRender::SetVirtualScreenSize( const SIZE2DI& Size )
{
  m_VirtualScreenSize = Size;
}


//! 初期化中か？
/*!
    @return   初期化中なら true
              まだなら false
 */
bool Graphics2DRender::IsInitializing() const
{
  if( m_State!=STATE_LOADING ) { return false; }


  //  メンバが読み込み中なら当然 true
  if( IsMemberLoading() ) { return true; }

  //  読み込み終わったので、残りの処理を行う

  const_cast<Graphics2DRender*>(this)->m_State = STATE_WORKING;
  const_cast<Graphics2DRender*>(this)->SetDefaultSetting();
  return false;
}


//! 最低限の設定を行う
/*!
    このクラスの描画周りで困ったらこれを呼んでリセットする感じ
 */
void Graphics2DRender::SetDefaultSetting()
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  SetBlendState( BLENDSTATE_ALPHA );
  SetFilterState( TEXTUREFILTER_LINEAR );
}


//! ２Ｄの描画モードを変更する
/*!
    @param  State [i ]  描画モード
 */
void Graphics2DRender::SetBlendState( BLENDSTATE State )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );
  m_BltState = State;
}

void Graphics2DRender::SetFilterState( TEXTUREFILTER filter )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );
  m_Filter = filter;
}

void Graphics2DRender::SetupState()
{
  Graphics::IDrawCommand& Command = GetCommand();

  {
    IBlendState& state = m_BlendState[m_BltState];
    Command.SetBlendState( state.Get() );
  }

  {
    ISamplerState* state = NULL;
    
    switch( m_Filter )
    {
    case TEXTUREFILTER_POINT:  { state = &m_SamplerPoint; }break;
    case TEXTUREFILTER_LINEAR: { state = &m_SamplerLinar; }break;
    }

    Command.PSSetSamplerState( 0, state->Get() );
    Command.PSSetSamplerState( 1, state->Get() );
  }

  {
    IDepthStencilState& state = m_DepthOff;
    Command.SetDepthStencilState( state.Get() );
  }

  {
    IRasterizerState& raster = m_Rasterizer;
    Command.SetRasterizerState( raster.Get() );
  }

}



bool Graphics2DRender::IsMemberLoading() const
{
  //  読み込みするメンバーがいるか？
  return
       m_FillVertexShader.IsCompiling()
    || m_FillLayout.IsCompiling()
    || m_FillPixelShader.IsCompiling()

    || m_BltLayout.IsCompiling()
    || m_BltVertexShader.IsCompiling()
    || m_BltPixelShader210.IsCompiling()
    || m_BltPixelShader211.IsCompiling()
    || m_BltPixelShader212.IsCompiling()

    || m_BltMaskLayout.IsCompiling()
    || m_BltMaskVertexShader.IsCompiling()
    || m_BltMaskPixelShader220.IsCompiling()
    || m_BltMaskPixelShader221.IsCompiling()
    || m_BltMaskPixelShader222.IsCompiling()
    || m_BltMaskPixelShader223.IsCompiling()
    || m_BltMaskPixelShader224.IsCompiling()
    || m_BltMaskPixelShader225.IsCompiling()

    || m_BltMixLayout.IsCompiling()
    || m_BltMixVertexShader.IsCompiling()
    || m_BltMixPixelShader230.IsCompiling()
    || m_BltMixPixelShader231.IsCompiling()
    || m_BltMixPixelShader232.IsCompiling()

    ;
}

Graphics::IDrawCommand& Graphics2DRender::GetCommand()
{
  return m_CommandCtl.GetCommand();
}


//! 与えられたパラメーターからスクリーンサイズを加味した平面モデルを作成する
/*!
    @param	Pos    [i ]	配置する座標
    @param	Size   [i ]	平面の大きさ
    @param	Center [i ]	平面の中心点
    @param	pos    [ o]	出来上がったモデル(4頂点であること)
 */
void Graphics2DRender::CreateVirtualScreenModel( const POINT2DI& Pos, const SIZE2DI& Size, const POINT2DI& Center, MODELINFO& m )
{
  const SIZE2DI& screen = m_VirtualScreenSize;

  const float w = float(Size.w) / float(screen.w/2);
  const float h = float(Size.h) / float(screen.w/2);
  const float x = float(Center.x) / float(screen.w/2);
  const float y = float(Center.y) / float(screen.w/2);

  m.Model[0] = VECTOR4DF(-x,  -y,  0,1);  //  左上
  m.Model[1] = VECTOR4DF(-x+w,-y,  0,1);  //  右上
  m.Model[2] = VECTOR4DF(-x,  -y+h,0,1);  //  左下
  m.Model[3] = VECTOR4DF(-x+w,-y+h,0,1);  //  右下

  const float texel = m_CommandCtl.GetTexelMapValue();

  const float aspect = float(screen.w) / float(screen.h);
  const float TranceX = (float(Pos.x-screen.w/2)-texel) / float(screen.w/2);
  const float TranceY = (float(Pos.y-screen.h/2)-texel) / float(screen.h/2);

  m.Translate = MATRIX4DF().SetScale(1,aspect,1)
              * MATRIX4DF().SetTranslate(TranceX,TranceY,0);

}

//! モデルデータから頂点を作成する
/*!
    @param	Model   [i ]	基本モデルデータ
    @param	Mat     [i ]	作用させる行列
    @param	pVertex [ o]	出力先
    @param	Stride  [i ]	pVertex の１頂点あたりの大きさ(バイト単位)
 */
void Graphics2DRender::SetupVertex( const VECTOR4DF* Model, const MATRIX4DF& Mat, void* pVertex, int Stride )
{
  unt08* p = (unt08*)pVertex;
  for( int i=0; i<4; ++i )
  {
    const VECTOR4DF r   = Model[i] * Mat;
    POINT3DF* pos = (POINT3DF*)p;
    pos->x = r.x;
    pos->y = -r.y;    //  ←値を反転させてるの、なんでだっけな？　３Ｄと２Ｄの座標系が反転してるから？
//    pos->z = r.z;
    pos->z = 0; //  2Dだし r.z の値はいらない（クリップされてしまうというのもある）

    p += Stride;
  }
}

//! このクラスで使われている共通の VertexBuffer に流し込む
/*!
    @param	pVertex [i ]	流し込むデータ
    @param	size    [i ]	データの大きさ（バイト単位）
    @param	Stride  [i ]	１頂点あたりの大きさ（バイト単位）
 */
void Graphics2DRender::SetCommonVertex( void* pVertex, int size, int Stride )
{
  const IVertex& vertex = m_CommonVertex;
  Graphics::IDrawCommand& Command = GetCommand();

  const int sub = 0;
  Graphics::MAPPEDRESOURCE map;
  Command.ResourceMap( vertex.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map );
  memcpy( map.pData, pVertex, size );
  Command.ResourceUnmap( vertex.Get(), sub );

  Command.SetVertex( 0, vertex.Get(), 0, Stride );
}


void Graphics2DRender::AddGaizi( unt32 no, const Texture2D& tex )
{
	m_GaiziMap[no] = tex;
}

}