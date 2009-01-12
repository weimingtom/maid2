#include"checkboxsample.h"

using namespace Maid;



CheckBoxSample::CheckBoxSample( GraphicsRender& r )
    :m_Render( r )
{

}


void CheckBoxSample::OnInitialize( const Mouse* pMouse, const Keybord* pKey )
{
  m_hFont.Create( SIZE2DI(8,16), true );
  m_Size = SIZE2DI(100,100);
}

void CheckBoxSample::OnFinalize()
{

}

bool CheckBoxSample::LocalIsMouseIn( const POINT2DI& pos ) const
{
  return Collision<int>::IsPointToRect( pos, RECT2DI(POINT2DI(0,0),m_Size) );
}

void CheckBoxSample::OnUpdateFrame()
{
}

void CheckBoxSample::OnUpdateDraw( const  RenderTargetBase& Target, const IDepthStencil& Depth, const VECTOR2DI& offset  )
{
  String str;
  const bool in = IsMouseIn();
  const bool down = IsButtonDown();

  if( in )
  {
    if( down ) { str = MAIDTEXT("押し中"); }
    else { str = MAIDTEXT("入った"); }
  }else
  {
    if( down ) { str = MAIDTEXT("押し中外"); }
    else { str = MAIDTEXT("通常"); }
  }

  if( IsCheck() )
  {
    str += MAIDTEXT(":check");
  }else
  {
    str += MAIDTEXT(":free");
  }

  const POINT2DI pos = POINT2DI(0,0)+offset;
  m_Render.Fill( pos, COLOR_R32G32B32A32F(1,0,0,1), m_Size, POINT2DI(0,0) );
  m_Render.BltText( pos, m_hFont, str, COLOR_R32G32B32A32F(1,1,1,1) );
}

void CheckBoxSample::OnMouseIn( const POINT2DI& pos )
{
}

void CheckBoxSample::OnMouseOut( const POINT2DI& pos )
{
}

void CheckBoxSample::OnMouseDown( const POINT2DI& pos, int Button )
{
}

void CheckBoxSample::OnMouseUp( const POINT2DI& pos, int Button )
{
}

void CheckBoxSample::OnStateChange( bool IsCheck )
{

}
