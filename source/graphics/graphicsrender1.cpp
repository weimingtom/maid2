#include"graphicsrender.h"
#include"customvertextemplate.h"




namespace Maid
{

GraphicsRender::GraphicsRender()
  :m_State(STATE_EMPTY)
{

}

GraphicsRender::~GraphicsRender()
{

}

void GraphicsRender::Initialize()
{
  const GraphicsCore& Core = *(GlobalPointer<GraphicsCore>::Get());

  {
    //  1頂点あたり128バイトも使わないでしょう
    m_CommonVertex.Create( 128*4 );
  }


  {

    {
      const char* vs =
        "vs.1.1\n"
        "dcl_position v0\n"
        "dcl_color v1\n"
        "mov oPos, v0\n"
        "mov oD0, v1\n"
        ;

      m_FillVertexShader.Create( String::ConvertSJIStoMAID(vs) );

      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };

      m_FillLayout.Create( element, NUMELEMENTS(element), String::ConvertSJIStoMAID(vs) );
    }

    {
      const char* ps =
        "0"
        ;

      m_FillPixelShader.Create( String::ConvertSJIStoMAID(ps) );
    }
  }

  {
    {
      const char* vs =
        "vs.1.1\n"
        "dcl_position v0\n"
        "dcl_color    v1\n"
        "dcl_texcoord v2\n"
        "mov oPos, v0\n"
        "mov oD0,  v1\n"
			  "mov oT0,  v2\n"
        ;

      m_BltVertexShader.Create( String::ConvertSJIStoMAID(vs) );

      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"TEXCOORD", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT2, 0, 28, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };

      m_BltLayout.Create( element, NUMELEMENTS(element), String::ConvertSJIStoMAID(vs) );
    }

    {
      const char* ps =
        "10"
        ;

      m_BltPixelShader.Create( String::ConvertSJIStoMAID(ps) );
    }
  }

  {
    {
      const char* ps =
        "20"
        ;

      m_FontPixelShader.Create( String::ConvertSJIStoMAID(ps) );
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
    m_BlendState.resize( 5 );
    m_BlendState[0].Create( BlendState::TEMPLATE_NORMAL );
    m_BlendState[1].Create( BlendState::TEMPLATE_ALPHA );
    m_BlendState[2].Create( BlendState::TEMPLATE_ADD );
    m_BlendState[3].Create( BlendState::TEMPLATE_SUB );
    m_BlendState[4].Create( BlendState::TEMPLATE_MUL );
  }

  {
    m_VirtualScreenSize = Core.GetScreenMode().Format.Size;
  }
  {
    m_FontManager.Initialize();
  }

  m_BeginCount = 0;
  m_pDrawCommand = Core.GetDevice()->GetDrawCommandExecute();
  m_State = STATE_LOADING;
}

void GraphicsRender::Begin()
{
  if( m_BeginCount!=0 ) { return ; }
  ++m_BeginCount;
  m_pDrawCommand->Begin();
  SetDefaultSetting();
}

void GraphicsRender::End()
{
  if( m_BeginCount==0 ) { return ; }
  m_pDrawCommand->End();
  m_FontManager.Garbage();
  --m_BeginCount;
}

void GraphicsRender::ClearRenderTarget( const COLOR_R32G32B32A32F& Col )
{
  m_pDrawCommand->ClearRenderTarget( Col );
}

void GraphicsRender::ClearDepth( float Depth )
{
  m_pDrawCommand->ClearDepthStencil( Graphics::IDrawCommand::CLEARFLAG_DEPTH, Depth, 0 );
}

void GraphicsRender::ClearStencil( unt08 Stencil )
{
  m_pDrawCommand->ClearDepthStencil( Graphics::IDrawCommand::CLEARFLAG_STENCIL, 0, Stencil );
}

void GraphicsRender::ClearDepthStencil( float Depth, unt08 Stencil )
{
  m_pDrawCommand->ClearDepthStencil( Graphics::IDrawCommand::CLEARFLAG_DEPTHSTENCIL, Depth, Stencil );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 仮想サイズの変更
/*!
    @param  Size [i ] 新しいサイズ
 */
void GraphicsRender::SetVirtualScreenSize( const SIZE2DI& Size )
{
  m_VirtualScreenSize = Size;
}


void GraphicsRender::SetRenderTarget( const RenderTargetBase& Target )
{
  m_pDrawCommand->SetRenderTarget( 0, Target.Get(), Graphics::SPDEPTHSTENCIL() );

  const Graphics::VIEWPORT view( RECT2DI(POINT2DI(0,0),Target.GetSize()), 0, 1 );

  m_pDrawCommand->SetViewPort( view );
}

void GraphicsRender::SetRenderTarget( const RenderTargetBase& Target, const IDepthStencil& Depth )
{
  m_pDrawCommand->SetRenderTarget( 0, Target.Get(), Depth.Get() );

  const Graphics::VIEWPORT view( RECT2DI(POINT2DI(0,0),Target.GetSize()), 0, 1 );

  m_pDrawCommand->SetViewPort( view );
}



bool GraphicsRender::IsInitializing() const
{
  if( m_State!=STATE_LOADING ) { return false; }


  //  メンバが読み込み中なら当然 true
  if( IsMemberLoading() ) { return true; }

  //  読み込み終わったので、残りの処理を行う

  const_cast<GraphicsRender*>(this)->m_State = STATE_WORKING;
  const_cast<GraphicsRender*>(this)->SetDefaultSetting();
  return false;
}

void GraphicsRender::SetDefaultSetting()
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  {
    IDepthStencilState& state = m_DepthOff;
    m_pDrawCommand->SetDepthStencilState( state.Get() );
  }
  SetBlendState( BLENDSTATE_ALPHA );
  IRasterizerState& raster = m_Rasterizer;
  m_pDrawCommand->SetRasterizerState( raster.Get() );
}



void GraphicsRender::SetBlendState( BLENDSTATE State )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );
  IBlendState& state = m_BlendState[State];

  m_pDrawCommand->SetBlendState( state.Get() );
}


bool GraphicsRender::IsMemberLoading() const
{
  //  読み込みするメンバーがいるか？
  return m_FillVertexShader.IsCompiling()
    || m_FillLayout.IsCompiling()
    || m_FillPixelShader.IsCompiling()
    || m_BltLayout.IsCompiling()
    || m_BltVertexShader.IsCompiling()
    || m_BltPixelShader.IsCompiling()
    || m_FontPixelShader.IsCompiling()
    ;
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 与えられたパラメーターからスクリーンサイズを加味した平面モデルを作成する
/*!
    @param	Pos    [i ]	配置する座標
    @param	Size   [i ]	平面の大きさ
    @param	Center [i ]	平面の中心点
    @param	pos    [ o]	出来上がったモデル(4頂点であること)
 */
void GraphicsRender::CreateVirtualScreenModel( const POINT2DI& Pos, const SIZE2DI& Size, const POINT2DI& Center, MODELINFO& m )
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

  const float aspect = float(screen.w) / float(screen.h);
  const float TranceX = (float(Pos.x-screen.w/2)-0.5f) / float(screen.w/2);
  const float TranceY = (float(Pos.y-screen.h/2)-0.5f) / float(screen.h/2);

  m.Translate = MATRIX4DF().SetScale(1,aspect,1)
              * MATRIX4DF().SetTranslate(TranceX,TranceY,0);

}

void GraphicsRender::SetupVertex( const VECTOR4DF* Model, const MATRIX4DF& Mat, void* pVertex, int Stride )
{
  unt08* p = (unt08*)pVertex;
  for( int i=0; i<4; ++i )
  {
    const VECTOR4DF r   = Model[i] * Mat;
    POINT3DF* pos = (POINT3DF*)p;
    pos->x = r.x;
    pos->y = -r.y;
//    pos->z = r.z;
    pos->z = 0; //  2Dだし r.z の値はいらない

    p += Stride;
  }
}


void GraphicsRender::SetCommonVertex( void* pVertex, int size, int Stride )
{
  const IVertex& vertex = m_CommonVertex;
  const Graphics::SPDRAWCOMMAND& pCommand = m_pDrawCommand;

  const int sub = 0;
  Graphics::MAPPEDRESOURCE map;
  pCommand->ResourceMap( vertex.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE, 0, map );
  memcpy( map.pData, pVertex, size );
  pCommand->ResourceUnmap( vertex.Get(), sub );

  pCommand->SetVertex( 0, vertex.Get(), 0, Stride );

}

}