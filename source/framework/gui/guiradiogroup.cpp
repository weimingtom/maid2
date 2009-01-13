#include"guiradiogroup.h"


namespace Maid
{

GUIRadioGroup::~GUIRadioGroup()
{
  Clear();
}

void GUIRadioGroup::SetRadioButton( const std::vector<GUIRadioButton*>& RadioButtonList )
{
  m_Button = RadioButtonList;

  //  私が親ですよーとメッセージを送る
  for( size_t i=0; i<m_Button.size(); ++i )
  {
    GUIMESSAGE_INITIALIZE m;
    m.pParent = this;
    m.ID = (IGUIParts::ID)i;
    m_Button[i]->SendMessage( m );
  }

  m_SelectNo = SELECT_NONE;
}

void GUIRadioGroup::SetRadioButton( GUIRadioButton* Data, size_t Length )
{
  std::vector<GUIRadioButton*> v(Length);

  for( size_t i=0; i<v.size(); ++i )
  {
    v[i] = Data+i;
  }

  SetRadioButton( v );
}

void GUIRadioGroup::Clear()
{
  const GUIMESSAGE_FINALIZE m;

  for( size_t i=0; i<m_Button.size(); ++i )
  {
    m_Button[i]->SendMessage( m );
  }

  m_Button.clear();
  m_SelectNo = SELECT_NONE;
}


IGUIParts* GUIRadioGroup::LocalFindChildParts( const POINT2DI& pos )
{
  for( size_t i=0; i<m_Button.size(); ++i )
  {
    IGUIParts* pParts = m_Button[i];
    if( pParts->IsCollision(pos) ) { return pParts; }
  }

  return NULL;
}


int GUIRadioGroup::GetCheckNo() const
{
  return m_SelectNo;
}

void GUIRadioGroup::SetCheck( int pos )
{
  MAID_ASSERT( pos<=SELECT_NONE, "値が小ささすぎます " << pos );
  MAID_ASSERT( !(pos<(int)m_Button.size()), "範囲が大きすぎます " << pos );

  if( m_SelectNo==pos ) { return ; }

  const int old_no = m_SelectNo;
  const int new_no = pos;

  m_SelectNo = new_no;

  if( old_no!=SELECT_NONE )
  {
    m_Button[old_no]->SetCheck(false);
  }

  if( new_no!=SELECT_NONE )
  {
    m_Button[new_no]->SetCheck(true);
  }

  if( OnChangeSelect( old_no, new_no )==MESSAGERETURN_SENDPARENT )
  {
    GUIMESSAGE_RADIDGROUP_CHANGESELECT m;
    m.OldNo = old_no;
    m.NewNo = new_no;

    SendMessageParent( m );
  }
}

void GUIRadioGroup::OnUpdateFrame()
{
  for( size_t i=0; i<m_Button.size(); ++i )
  {
    m_Button[i]->UpdateFrame();
  }
}

void GUIRadioGroup::OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
{
  const POINT2DI p = pos - GetOffset();
  for( size_t i=0; i<m_Button.size(); ++i )
  {
    m_Button[i]->UpdateDraw(Target,Depth,p);
  }
}

void GUIRadioGroup::ReciveMessage( ID src, const IGUIParam& Param )
{
  switch( Param.Message )
  {
  case IGUIParam::MESSAGE_MOUSEMOVE:
    {
      const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(Param);
      const POINT2DI pos = p.Position-GetOffset();

      if( OnGroupMouseMove( src, pos )==MESSAGERETURN_SENDPARENT )
      {
        GUIMESSAGE_RADIDGROUP_MOUSEMOVE m;
        m.ButtonNo = src;
        m.Position = p.Position;
        SendMessageParent( m );
      }
    }break;

  case IGUIParam::MESSAGE_MOUSEDOWN:
    {
      const GUIMESSAGE_MOUSEDOWN& p = static_cast<const GUIMESSAGE_MOUSEDOWN&>(Param);
      const POINT2DI pos = p.Position-GetOffset();
      if( OnGroupMouseDown( src, pos, p.Button )==MESSAGERETURN_SENDPARENT )
      {
        GUIMESSAGE_RADIDGROUP_MOUSEDOWN m;
        m.ButtonNo = src;
        m.Position = p.Position;
        m.ButtonNo = p.Button;
        SendMessageParent( m );
      }
    }break;

  case IGUIParam::MESSAGE_MOUSEUP:
    {
      const GUIMESSAGE_MOUSEUP& p = static_cast<const GUIMESSAGE_MOUSEUP&>(Param);
      const POINT2DI pos = p.Position-GetOffset();
      if( OnGroupMouseUp( src, pos, p.Button )==MESSAGERETURN_SENDPARENT )
      {
        GUIMESSAGE_RADIDGROUP_MOUSEUP m;
        m.ButtonNo = src;
        m.Position = p.Position;
        m.ButtonNo = p.Button;
        SendMessageParent( m );
      }
    }break;

  case IGUIParam::MESSAGE_MOUSEIN:
    {
      const GUIMESSAGE_MOUSEIN& p = static_cast<const GUIMESSAGE_MOUSEIN&>(Param);
      const POINT2DI pos = p.Position-GetOffset();
      if( OnGroupMouseIn( src, pos )==MESSAGERETURN_SENDPARENT )
      {
        GUIMESSAGE_RADIDGROUP_MOUSEIN m;
        m.ButtonNo = src;
        m.Position = p.Position;
        SendMessageParent( m );
      }
    }break;

  case IGUIParam::MESSAGE_MOUSEOUT:
    {
      const GUIMESSAGE_MOUSEOUT& p = static_cast<const GUIMESSAGE_MOUSEOUT&>(Param);
      const POINT2DI pos = p.Position-GetOffset();
      if( OnGroupMouseOut( src, pos )==MESSAGERETURN_SENDPARENT )
      {
        GUIMESSAGE_RADIDGROUP_MOUSEOUT m;
        m.ButtonNo = src;
        m.Position = p.Position;
        SendMessageParent( m );
      }
    }break; 

  case IGUIParam::MESSAGE_RADIOBUTTON_SETCHECK:
    {
      const GUIMESSAGE_RADIOBUTTON_SETCHECK& p = static_cast<const GUIMESSAGE_RADIOBUTTON_SETCHECK&>(Param);

      //  新しいのはチェックされているので、古いほうをＯＦＦにするだけにしておく

      if( m_SelectNo!=SELECT_NONE )
      {
        m_Button[m_SelectNo]->SetCheck(false);
      }

      m_SelectNo = src;

      if( OnChangeSelect( m_SelectNo, src )==MESSAGERETURN_SENDPARENT )
      {
        GUIMESSAGE_RADIDGROUP_CHANGESELECT m;
        m.OldNo = m_SelectNo;
        m.NewNo = src;

        SendMessageParent( m );
      }
    }break;
  }
}

}
