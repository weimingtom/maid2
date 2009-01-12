#include"iguiparts.h"

namespace Maid
{

IGUIParts::IGUIParts()
  :m_ID(PARTSID_NONE)
  ,m_pParent(NULL)
  ,m_IsEnable(true)
  ,m_IsVisible(true)
  ,m_Offset(0,0)
{

}

void IGUIParts::SendMessageParent( const IGUIParam& Param )
{
  if( m_pParent==NULL ) { return ; }
  m_pParent->ReciveMessage( m_ID, Param );
}


void IGUIParts::UpdateFrame()
{
  {
    //  この処理は SendMessage() 内でやるべきなんだけど、パフォーマンスに影響でそうなのでここでやる
    if( !IsEnable() ) { return ; }
    OnUpdateFrame();
  }
//  ここで処理してるからなにもしないだろうということで省く
//  GUIMESSAGE_UPDATEFRAME m;
//  SendMessage( m );
}

void IGUIParts::UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth )
{
  {
    //  この処理は SendMessage() 内でやるべきなんだけど、パフォーマンスに影響でそうなのでここでやる
    if( !IsVisible() ) { return ; }
    OnUpdateDraw( Target, Depth, m_Offset );
  }

//  ここで処理してるからなにもしないだろうということで省く
//  GUIMESSAGE_UPDATEDRAW m(Target, Depth, m_Offset);
//  SendMessage( m );
}


void IGUIParts::SetPosition( const POINT2DI& pos )
{
  GUIMESSAGE_SETPOSITION m;
  m.Position = pos;
  SendMessage( m );
}

void IGUIParts::SetEnable ( bool IsEnable )
{
  GUIMESSAGE_ENABLE m;
  m.IsEnable = IsEnable;
  SendMessage( m );
}

void IGUIParts::SetVisible( bool IsVisible )
{
  GUIMESSAGE_VISIBLE m;
  m.IsVisible = IsVisible;
  SendMessage( m );
}

POINT2DI IGUIParts::GetPosition() const
{
  return POINT2DI(m_Offset.x,m_Offset.y);
}

bool IGUIParts::IsEnable () const
{
  return m_IsEnable;
}

bool IGUIParts::IsVisible() const
{
  return m_IsVisible;
}

IGUIParts::ID   IGUIParts::GetID() const
{
  return m_ID;
}


bool IGUIParts::IsMouseIn( const POINT2DI& pos ) const
{
  if( !IsEnable() ) { return false; }

  const POINT2DI p = pos-m_Offset;
  return LocalIsMouseIn( p );
}

void IGUIParts::SendMessage( const IGUIParam& Param )
{
  SendMessageParent( Param );

  switch( Param.Message )
  {
  case IGUIParam::MESSAGE_INITIALIZE:
    {
      const GUIMESSAGE_INITIALIZE& p = static_cast<const GUIMESSAGE_INITIALIZE&>(Param);
      m_ID = p.ID;
      m_pParent = p.pParent;
      OnInitialize( p.pMouse, p.pKey );
    }break;

  case IGUIParam::MESSAGE_FINALIZE:
    {
      const GUIMESSAGE_FINALIZE& p = static_cast<const GUIMESSAGE_FINALIZE&>(Param);
      OnFinalize();
    }break;

  case IGUIParam::MESSAGE_UPDATEFRAME:
    {
      const GUIMESSAGE_UPDATEFRAME& p = static_cast<const GUIMESSAGE_UPDATEFRAME&>(Param);
    }break;

  case IGUIParam::MESSAGE_UPDATEDRAW:
    {
      const GUIMESSAGE_UPDATEDRAW& p = static_cast<const GUIMESSAGE_UPDATEDRAW&>(Param);
    }break;

  case IGUIParam::MESSAGE_SETPOSITION:
    {
      const GUIMESSAGE_SETPOSITION& p = static_cast<const GUIMESSAGE_SETPOSITION&>(Param);
      m_Offset.Reset( p.Position );
    }break;

  case IGUIParam::MESSAGE_ENABLE:
    {
      const GUIMESSAGE_ENABLE& p = static_cast<const GUIMESSAGE_ENABLE&>(Param);
      m_IsEnable = p.IsEnable;
    }break;

  case IGUIParam::MESSAGE_VISIBLE:
    {
      const GUIMESSAGE_VISIBLE& p = static_cast<const GUIMESSAGE_VISIBLE&>(Param);
      m_IsVisible = p.IsVisible;
    }break;
  }

  OnSendMessage(Param);

}

}

