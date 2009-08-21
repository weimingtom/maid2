#include"iguicombobox.h"


namespace Maid
{




IGUIComboBox::IGUIComboBox()
  :m_State(STATE_NORMAL)
  ,m_SelectListMax(1)
  ,m_SelectID(0)
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

IGUIComboBox::IElement& IGUIComboBox::GetSelectElement()
{
  return m_Tmp;
}


bool IGUIComboBox::LocalIsCollision( const POINT2DI& pos ) const
{
  if( IsBoxCollision( pos ) ) { return true; }

  //  項目選択時は、スライダ、選択項目も調べる
  if( GetState()==STATE_SELECTING )
  {
    if( IsSliderCollision( pos ) ) { return true; }
    if( IsSliderButtonCollision( pos ) ) { return true; }

    for( ELEMENTLIST::const_iterator ite=m_ElementList.begin(); ite!=m_ElementList.end(); ++ite )
    {
      const IElement* p = ite->second;

      if( p->IsCollision(pos) ) { return true; }
    }
  }

  return false;
}


void IGUIComboBox::DrawElement( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
{
  POINT2DI DrawPos = pos;

  //  選択項目の表示
  //  一番上は現在選択されているもの
  //  ２行目以降はidの順に表示していく
  //  ただし２行目以降は状態で表示非表示が変わる

  {
    const IElement& ele = GetSelectElement();
    ele.DrawNormal( Target, Depth, DrawPos );

    DrawPos.y += ele.GetBoxSize().h;
  }

  if( state==STATE_NORMAL ) { return ; }

  {
    for( ELEMENTLIST::iterator ite=m_ElementList.begin(); ite!=m_ElementList.end(); ++ite )
    {
      const int id = ite->first;
      IElement* pEle = ite->second;

 //     if( id==GetSelectID() ) { pEle->DrawNormal( Target, Depth, DrawPos ); }
 //     else { }

      DrawPos.y += pEle->GetBoxSize().h;
    }

  }


  //  スライダの表示


/*
    void SetSelectID( int id );
    int GetSelectID() const;
*/
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
          if( IsSliderCollision(pos) )
          {
          }
          else if( IsSliderButtonCollision(pos) )
          {
            ChangeState(STATE_SLIDERBUTTONDOWN);
          }
          else
          {
            //  elementを押したか？

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
          if( IsSliderCollision(pos) )
          {
          }
          else if( IsSliderButtonCollision(pos) )
          {
          }
          else
          {
            //  element にカーソルを合わせる

          }

        }break;

      case STATE_SLIDERBUTTONDOWN:
        {
          //  ページを送る

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
 
        }break;

      case STATE_SLIDERBUTTONDOWN:
        {

        }break;
      }
    }break;
  }

  return IGUIParts::MessageExecuting( pParam );

}



}

