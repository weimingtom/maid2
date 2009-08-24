#include"iguicombobox.h"


namespace Maid
{




IGUIComboBox::IGUIComboBox()
  :m_State(STATE_NORMAL)
  ,m_SelectListMax(1)
  ,m_SelectID(0)
  ,m_MouseInID(0)
  ,m_SliderTop(0)
  ,m_SelectBoxOffset(0,0)
  ,m_SliderBarLength(1)
  ,m_SliderButtonLength(0)
{

}

void IGUIComboBox::Insert( int id, IElement& parts )
{
  MAID_ASSERT( m_ElementList.find(id)!=m_ElementList.end(), "id:" << id << "は存在しています" );

  m_ElementList[id] = &parts;

  if( m_ElementList.size()==1 )
  {
    SetSelectID(id);
  }
}

void IGUIComboBox::Delete( int id )
{
  m_ElementList.erase( m_ElementList.find(id) );

  if( GetSelectID()==id )
  {
    if( !m_ElementList.empty() )
    {
      SetSelectID(m_ElementList.begin()->first);
    }
  }
}

void IGUIComboBox::DeleteAll()
{
  m_ElementList.clear();
}

IGUIComboBox::STATE IGUIComboBox::GetState() const
{
  return m_State;
}

void IGUIComboBox::ChangeState( STATE s )
{
  m_State = s;
}


void IGUIComboBox::SetSelectID( int new_id )
{
  const int old_id = GetSelectID();

  if( old_id==new_id ) { return ; }

  m_SelectID = new_id;
}

int IGUIComboBox::GetSelectID() const
{
  MAID_ASSERT( m_ElementList.empty(), "まだ登録されていません" );

  return m_SelectID;
}

void IGUIComboBox::SetSelectListMax( int count )
{
  m_SelectListMax = count;
}

IGUIComboBox::IElement& IGUIComboBox::GetSelectElement()
{
  for( ELEMENTLIST::iterator ite=m_ElementList.begin(); ite!=m_ElementList.end(); ++ite )
  {
    if( m_SelectID==ite->first ) { return *(ite->second); }
  }

  return m_Tmp;
}

void IGUIComboBox::SetSelectBoxOffset( const VECTOR2DI& offset )
{
  m_SelectBoxOffset = offset;
}

const VECTOR2DI& IGUIComboBox::GetSelectBoxOffset() const
{
  return m_SelectBoxOffset;
}

void IGUIComboBox::SetSliderOffset( const VECTOR2DI& offset )
{
  m_SliderOffset = offset;
}

const VECTOR2DI& IGUIComboBox::GetSliderOffset() const
{
  return m_SliderOffset;
}

void IGUIComboBox::SetSliderBarLength( int len )
{
  m_SliderBarLength = len;
}

int IGUIComboBox::GetSliderBarLength() const
{
  return m_SliderBarLength;
}

void IGUIComboBox::SetSliderButtonLength( int len )
{
  m_SliderButtonLength = len;
}

int IGUIComboBox::GetSliderButtonLength() const
{
  return m_SliderButtonLength;
}


bool IGUIComboBox::LocalIsCollision( const POINT2DI& pos ) const
{
  if( IsBoxCollision( pos ) ) { return true; }

  if( GetState()==STATE_NORMAL ) { return false; }

  //  項目選択時は、スライダ、選択項目も調べる

  {
    const POINT2DI SliderPos = pos - GetSliderOffset();

    if( IsSliderButtonCollision( SliderPos ) ) { return true; }
    if( IsSliderCollision( SliderPos ) ) { return true; }
  }

  POINT2DI ElementPos = pos - GetSelectBoxOffset();
  ELEMENTLIST::const_iterator ite = GetSliderTopIte();

  int height = 0;
  for( int i=0; i<m_SelectListMax; ++i )
  {
    if( ite==m_ElementList.end() ) { break; }

    const IElement* p = ite->second;

    if( p->IsCollision( ElementPos ) ) { return true; }

    ElementPos.y -= p->GetBoxSize().h;
    ++ite;
  }


  return false;
}
/*
int  IGUIComboBox::CalcSliderHeight() const
{
  ELEMENTLIST::const_iterator ite = GetSliderTopIte();

  int height = 0;
  for( int i=0; i<m_SelectListMax; ++i )
  {
    if( ite==m_ElementList.end() ) { break; }

    const IElement* p = ite->second;

    height += p->GetBoxSize().h;
    ++ite;
  }

  return height;
}
*/

IGUIComboBox::ELEMENTLIST::const_iterator IGUIComboBox::GetSliderTopIte() const
{
  //  現在のスライダボタンの位置にある、elementを取得
  ELEMENTLIST::const_iterator ite = m_ElementList.begin();

  for( int i=0; i<m_SliderTop; ++i )
  {
    ++ite;
  }

  return ite;
}


void IGUIComboBox::DrawElement( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
{
  const STATE state = GetState();

  //  選択項目の表示
  //  一番上は現在選択されているもの
  //  ２行目以降はidの順に表示していく
  //  ただし２行目以降は状態で表示非表示が変わる

  {
    IElement& ele = GetSelectElement();
    ele.DrawSelect( Target, Depth, pos );
  }

  if( state==STATE_NORMAL ) { return ; }

  {
    POINT2DI DrawPos = pos + GetSelectBoxOffset();

    ELEMENTLIST::const_iterator ite = GetSliderTopIte();

    for( int i=0; i<m_SelectListMax; ++i )
    {
      if( ite==m_ElementList.end() ) { break; }

      const int id = ite->first;
      IElement* pEle = ite->second;
      const SIZE2DI size = pEle->GetBoxSize();

      if( id==m_MouseInID ) { pEle->DrawMouseIn( Target, Depth, DrawPos ); }
      else                  { pEle->DrawNormal( Target, Depth, DrawPos );  }

      DrawPos.y += size.h;

      ++ite;
    }
  }

  //  スライダの表示
  POINT2DI SliderPos = pos + GetSliderOffset();
  DrawSlider( Target, Depth, SliderPos );
}


int IGUIComboBox::CalcSliderButtonOffset() const
{
  if( (int)m_ElementList.size() <= m_SelectListMax ) { return 0; }

  const int slidermax = m_ElementList.size()-m_SelectListMax;
  const int slidernow = m_SliderTop;

  const int height = GetSliderBarLength() - GetSliderButtonLength();

  const int ret = height * slidernow / slidermax;
  return ret;
}

int IGUIComboBox::CalcSliderValue( int PosY ) const
{
  const int len = GetSliderBarLength()-GetSliderButtonLength();
  const int value = m_ElementList.size()-m_SelectListMax;

  const int no = PosY  * value / len;

  if( no<0 ) { return 0; }

  return std::min(no,value);
}



IGUIComboBox::MESSAGERETURN IGUIComboBox::MessageExecuting( SPGUIPARAM& pParam )
{
  switch( pParam->Message )
  {
  case IGUIParam::MESSAGE_MOUSEDOWN:
    {
      const GUIMESSAGE_MOUSEDOWN& m = static_cast<const GUIMESSAGE_MOUSEDOWN&>(*pParam);
      const POINT2DI pos  = CalcLocalPosition(m.Position);

      switch( GetState() )
      {
      case STATE_NORMAL:
        {
          if( IsBoxCollision(pos) )
          {
            ChangeState(STATE_SELECTING);
          }
        }break;

      case STATE_SELECTING:
        {
          const POINT2DI SliderPos = pos - GetSliderOffset();

          if( IsSliderButtonCollision(SliderPos) )
          {
            ChangeState(STATE_SLIDERBUTTONDOWN);
          }
          else if( IsSliderCollision(SliderPos) )
          {
            m_SliderTop = CalcSliderValue( SliderPos.y );
          }
          else
          {
            //  押してマウス選択を決定する前に
            //  MESSAGE_MOUSEMOVE 側で決定できるのでここは無視する
          }

        }break;

      case STATE_SLIDERBUTTONDOWN:
        {

        }break;
      }
    }break;

  case IGUIParam::MESSAGE_MOUSEMOVE:
    {
      const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(*pParam);
      const POINT2DI pos  = CalcLocalPosition(p.Position);

      switch( GetState() )
      {
      case STATE_NORMAL:
        {
        }break;

      case STATE_SELECTING:
        {
          const POINT2DI SliderPos = pos - GetSliderOffset();

          if( IsSliderCollision(SliderPos) )
          {
          }
          else if( IsSliderButtonCollision(SliderPos) )
          {
          }
          else
          {
            POINT2DI ElementPos = pos - GetSelectBoxOffset();
            ELEMENTLIST::const_iterator ite = GetSliderTopIte();

            for( int i=0; i<m_SelectListMax; ++i )
            {
              if( ite==m_ElementList.end() ) { break; }

              const IElement* p = ite->second;

              if( p->IsCollision( ElementPos ) )
              {
                m_MouseInID = ite->first;
                break; 
              }

              ElementPos.y -= p->GetBoxSize().h;
              ++ite;
            }
          }

        }break;

      case STATE_SLIDERBUTTONDOWN:
        {
          const POINT2DI SliderPos = pos - GetSliderOffset();

          m_SliderTop = CalcSliderValue( SliderPos.y );
        }break;
      }
    }break;

  case IGUIParam::MESSAGE_MOUSEUP:
    {
      const GUIMESSAGE_MOUSEUP& m = static_cast<const GUIMESSAGE_MOUSEUP&>(*pParam);
      const POINT2DI pos  = CalcLocalPosition(m.Position);

      switch( GetState() )
      {
      case STATE_NORMAL:
        {
          //  なんもしない。する必要がない
        }break;

      case STATE_SELECTING:
        {
          //  elementの中であがったらそれを選択
          //  そうでなかったら状態を維持
            POINT2DI ElementPos = pos - GetSelectBoxOffset();
            ELEMENTLIST::const_iterator ite = GetSliderTopIte();

            for( int i=0; i<m_SelectListMax; ++i )
            {
              if( ite==m_ElementList.end() ) { break; }

              const IElement* p = ite->second;

              if( p->IsCollision( ElementPos ) )
              {
                m_SelectID = ite->first;
                ChangeState(STATE_NORMAL);

                GUIMESSAGE_COMBOBOX_CHANGEELEMENT mess;
                mess.SelectID = m_SelectID;

                PostMessage( mess );
                break; 
              }

              ElementPos.y -= p->GetBoxSize().h;
              ++ite;
            }
 
        }break;

      case STATE_SLIDERBUTTONDOWN:
        {
          ChangeState(STATE_SELECTING);
        }break;
      }
    }break;

  case IGUIParam::MESSAGE_SETKEYBORDFOCUS:
    {
      if( GetState()!=STATE_NORMAL )
      {
        ChangeState(STATE_NORMAL);
      }
    }break;
  }

  return IGUIParts::MessageExecuting( pParam );

}



}

