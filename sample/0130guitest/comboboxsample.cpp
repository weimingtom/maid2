#include"comboboxsample.h"


using namespace Maid;


static const RECT2DI BOXRECT(0,0,200,20);                 //  最初のボックスの位置、大きさ
static const RECT2DI SELECTBOXRECT(0,0,BOXRECT.w-20,20);  //  選択項目ボックスの位置、大きさ
static const SIZE2DI SLIDERBARSIZE(20,SELECTBOXRECT.h*3);  //  スライダバーの大きさ
static const RECT2DI SLIDERBUTTONRECT(0,0,SLIDERBARSIZE.w,20); //  スライダボタンの大きさ


void ComboBoxElementSample::Initialize( Maid::Graphics2DRender* r, const Maid::String& Text )
{
  m_pRender = r;
  m_hFont.Create( SIZE2DI(8,16), true );
  m_Text = Text;
}

void ComboBoxElementSample::UpdateFrame()
{

}

void ComboBoxElementSample::DrawNormal( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
{
  m_pRender->BltText( pos, m_hFont, m_Text+MAIDTEXT("no"), COLOR_R32G32B32A32F(1,1,1,1) );
}

void ComboBoxElementSample::DrawMouseIn( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
{
  m_pRender->BltText( pos, m_hFont, m_Text+MAIDTEXT("in"), COLOR_R32G32B32A32F(0,1,1,1) );
}

void ComboBoxElementSample::DrawSelect( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
{
  m_pRender->BltText( pos, m_hFont, m_Text+MAIDTEXT("se"), COLOR_R32G32B32A32F(1,1,1,1) );
}

bool ComboBoxElementSample::IsCollision( const POINT2DI& pos ) const
{
  return Collision<int>::IsPointToRect( pos, SELECTBOXRECT );
}

SIZE2DI ComboBoxElementSample::GetBoxSize() const
{
  return SELECTBOXRECT.GetSize();
}









void ComboBoxSample::Initialize( Graphics2DRender* r )
{
  m_pRender = r;
  m_hFont.Create( SIZE2DI(8,16), true );

  SetSelectBoxOffset( VECTOR2DI(0,BOXRECT.h) );
  SetSliderOffset( VECTOR2DI(SELECTBOXRECT.w,BOXRECT.h) );

  SetSliderBarLength( SLIDERBARSIZE.h );
  SetSliderButtonLength( SLIDERBUTTONRECT.h );
}




void ComboBoxSample::OnUpdateFrame()
{

}

void ComboBoxSample::OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
{
  String str = MAIDTEXT("コンボ:");
  const bool in = IsMouseIn();
  const STATE state = GetState();

  if( in ){ str += MAIDTEXT("in");  }
  else    { str += MAIDTEXT("out"); }


  switch( state )
  {
  case STATE_NORMAL:          { str += MAIDTEXT("通常"); }break;
  case STATE_SELECTING:       { str += MAIDTEXT("選択"); }break;
  case STATE_SLIDERBUTTONDOWN:{ str += MAIDTEXT("ｽﾗｲﾀﾞ"); }break;
  }

  {
    POINT2DI p = pos;
    p.x += BOXRECT.x;
    p.y += BOXRECT.y;

    //  下地
    m_pRender->Fill( p, COLOR_R32G32B32A32F(1,0,0,1), BOXRECT.GetSize(), POINT2DI(0,0) );
  }

  {
    //  現在の状態表示(デバッグ目的)
    m_pRender->BltText( pos+VECTOR2DI(0,-16), m_hFont, str, COLOR_R32G32B32A32F(1,1,1,1) );
  }

  DrawElement( Target, Depth, pos );
}


void ComboBoxSample::DrawSlider( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
{
  const POINT2DI ButtonPos( pos.x+SLIDERBUTTONRECT.x, pos.y+CalcSliderButtonOffset()+SLIDERBUTTONRECT.y );

  m_pRender->Fill( pos, COLOR_R32G32B32A32F(1,0,0,1), SLIDERBARSIZE, POINT2DI(0,0) );

  m_pRender->Fill( ButtonPos, COLOR_R32G32B32A32F(0,1,0,1), SLIDERBUTTONRECT.GetSize(), POINT2DI(0,0) );
}



bool ComboBoxSample::IsBoxCollision( const POINT2DI& pos ) const
{
  return Collision<int>::IsPointToRect( pos, BOXRECT );
}

bool ComboBoxSample::IsSelectBoxCollision( const Maid::POINT2DI& pos ) const
{
  return false;
}

bool ComboBoxSample::IsSliderCollision( const POINT2DI& pos ) const
{
  const RECT2DI rc(POINT2DI(0,0), SLIDERBARSIZE );
  return Collision<int>::IsPointToRect( pos, rc );
}

bool ComboBoxSample::IsSliderButtonCollision( const POINT2DI& pos ) const
{
  const POINT2DI ButtonPos( pos.x, pos.y-CalcSliderButtonOffset() );
  return Collision<int>::IsPointToRect( ButtonPos, SLIDERBUTTONRECT );
}
