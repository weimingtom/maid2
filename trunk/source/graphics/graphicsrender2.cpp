#include"graphicsrender.h"
#include"customvertextemplate.h"




namespace Maid
{
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定した色で塗りつぶす
/*!
    @param	rc    [i ]	塗りつぶす範囲
    @param	Color [ o]	塗りつぶす色
 */
void GraphicsRender::Fill( const RECT2DF& rc, const COLOR_R32G32B32A32F& Color )
{
  MAID_ASSERT( IsLoading(), "初期化中は呼ばないこと" );

  const SIZE2DI& screen = m_ScreenSize;
  const float x = float(rc.x-screen.w/2) / float(screen.w/2);
  const float y = float(rc.y-screen.h/2) / float(screen.h/2);
  const float w = float(rc.w*2)        / float(screen.w);
  const float h = float(rc.h*2)        / float(screen.h);

  CUSTOMVERTEX_COLOR v[] =
  {
    CUSTOMVERTEX_COLOR( POINT3DF(x,-(y),0), Color ),
    CUSTOMVERTEX_COLOR( POINT3DF(x,-(y+h),0), Color ),
    CUSTOMVERTEX_COLOR( POINT3DF(x+w,-(y),0), Color ),
    CUSTOMVERTEX_COLOR( POINT3DF(x+w,-(y+h),0), Color ),
  };

  const IVertex&       vertex = m_FillVertex;
  const IInputLayout&  layout = m_FillLayout;
  const IVertexShader& vs = m_FillVertexShader;
  const IPixelShader&  ps = m_FillPixelShader;

  const Graphics::SPDRAWCOMMAND& pCommand = m_pDrawCommand;

  {
    const int sub = 0;
    Graphics::MAPPEDRESOURCE map;
    m_pDrawCommand->ResourceMap( vertex.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE, 0, map );
    memcpy( map.pData, v, map.Pitch );
    pCommand->ResourceUnmap( vertex.Get(), sub );
  }

  pCommand->SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  pCommand->SetVertex( 0, vertex.Get(), 0, v[0].GetStructSize() );
  pCommand->SetInputLayout( layout.Get() );

  pCommand->VSSetShader( vs.Get() );
  pCommand->PSSetShader( ps.Get() );
  pCommand->Draw( 4, 0 );
}
	

}