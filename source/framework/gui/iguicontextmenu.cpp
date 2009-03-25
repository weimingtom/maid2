#include"iguicontextmenu.h"
#include"../../auxiliary/collision.h"
#include"../../auxiliary/debug/warning.h"

namespace Maid
{
  IGUIContextMenu::IElement::IElement()
    :m_IsSelect(false)
    ,m_pParent(NULL)
    ,m_Offset(0,0)
  {
  }

  void IGUIContextMenu::IElement::Insert( int id, IElement& parts )
  {
    parts.m_pParent = this;
    m_Child.insert( INFO( id, &parts ) );
    UpdateChildPosition();
  }

  void IGUIContextMenu::IElement::Delete( int id )
  {
    for( CHILDLIST::iterator ite = m_Child.begin(); ite!=m_Child.end(); ++ite )
    {
      const INFO& info = *ite;
      if( info.ID==id )
      {
        m_Child.erase( ite );
        break;
      }
    }
    UpdateChildPosition();
  }

  void IGUIContextMenu::IElement::DeleteAll()
  {
    m_Child.clear();
  }

  SIZE2DI IGUIContextMenu::IElement::CalcChildSize() const
  {
    int w = 0;
    int h = 0;

    for( CHILDLIST::const_iterator ite = m_Child.begin(); ite!=m_Child.end(); ++ite )
    {
      const INFO& info = *ite;
      const SIZE2DI size = info.pChild->GetSize();
      w = std::max( w, size.w );
      h += size.h;
    }

    return SIZE2DI(w,h);
  }

  RECT2DI IGUIContextMenu::IElement::CalcChildRect() const
  {
    const SIZE2DI size( CalcChildSize() );
    POINT2DI pos(GetOffset().x,GetOffset().y);
    if( m_pParent!=NULL )
    {
      const SIZE2DI s = m_pParent->CalcChildSize();
      pos.x += s.w;
      //pos.y += s.h; //  右にずれるだけなので + s.h はいらない
    }

    return RECT2DI( pos, size );
  }


  const IGUIContextMenu::IElement* IGUIContextMenu::IElement::GetParent() const
  {
    return m_pParent;
  }

  IGUIContextMenu::IElement* IGUIContextMenu::IElement::FindChild( const POINT2DI& pos )
  {
    for( CHILDLIST::iterator ite = m_Child.begin(); ite!=m_Child.end(); ++ite )
    {
      const INFO&   info = *ite;
      if( info.pChild->IsCollision(pos) )
      {
        return info.pChild;
      }
    }

    return NULL;
  }

  const IGUIContextMenu::IElement::INFO* IGUIContextMenu::IElement::FindChildInfo( const POINT2DI& pos ) const
  {
    for( CHILDLIST::const_iterator ite = m_Child.begin(); ite!=m_Child.end(); ++ite )
    {
      const INFO&   info = *ite;
      if( info.pChild->IsCollision(pos) )
      {
        return &info;
      }
    }

    return NULL;
  }



  bool IGUIContextMenu::IElement::IsCollision( const POINT2DI& pos ) const
  {
    const SIZE2DI size = GetSize();
    const POINT2DI off(m_Offset.x,m_Offset.y);

    return Collision<int>::IsPointToRect( pos, RECT2DI(off,size) );
  }


  void IGUIContextMenu::IElement::UpdateFrame()
  {
    if( IsSelect() )
    {
      for( CHILDLIST::iterator ite = m_Child.begin(); ite!=m_Child.end(); ++ite )
      {
        const INFO& info = *ite;
        info.pChild->UpdateFrame();
      }
    }

    LocalUpdateFrame();
  }

  void IGUIContextMenu::IElement::UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
  {
    if( IsSelect() )
    {
      for( CHILDLIST::iterator ite = m_Child.begin(); ite!=m_Child.end(); ++ite )
      {
        const INFO& info = *ite;
        info.pChild->UpdateDraw( Target, Depth, pos );
      }
    }

    LocalUpdateDraw( Target, Depth, pos + m_Offset );
  }

  bool IGUIContextMenu::IElement::IsSelect() const
  {
    return m_IsSelect;
  }

  void IGUIContextMenu::IElement::SetSelectState( bool on )
  {
    m_IsSelect = on;
  }

  void IGUIContextMenu::IElement::SetOffset( const VECTOR2DI& off )
  {
    m_Offset = off;
    UpdateChildPosition();
  }

  const VECTOR2DI& IGUIContextMenu::IElement::GetOffset() const
  {
    return m_Offset;
  }


  void IGUIContextMenu::IElement::UpdateChildPosition()
  {
    VECTOR2DI now = m_Offset;

    if( m_pParent!=NULL )
    {
      const SIZE2DI s = m_pParent->CalcChildSize();
      now.x += s.w;
    }

    for( CHILDLIST::iterator ite = m_Child.begin(); ite!=m_Child.end(); ++ite )
    {
      const INFO& info = *ite;
      const SIZE2DI size = info.pChild->GetSize();
      info.pChild->SetOffset( now );

      now.y += size.h;
    }
  }

  int IGUIContextMenu::IElement::GetChildSize() const
  {
    return (int)m_Child.size();
  }
















  IGUIContextMenu::IGUIContextMenu()
  {
  }



  void IGUIContextMenu::Insert( int id, IElement& parts )
  {
    m_Top.Insert( id, parts );
  }

  void IGUIContextMenu::Delete( int id )
  {
    m_Top.Delete( id );
  }

  void IGUIContextMenu::DeleteAll()
  {
    m_Top.DeleteAll();
  }
 
  bool IGUIContextMenu::LocalIsCollision( const POINT2DI& pos ) const
  {
    for( SELECTLIST::const_iterator ite =m_SelectList.begin(); ite!=m_SelectList.end(); ++ite )
    {
      const IElement* p = *ite;

      const RECT2DI rc = p->CalcChildRect();

      if( Collision<int>::IsPointToRect( pos, rc ) )
      {
        return true;
      }
    }
    return false;
  }

  void IGUIContextMenu::OnUpdateFrame()
  {
    m_Top.UpdateFrame();
  }

  void IGUIContextMenu::OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
  {
    m_Top.UpdateDraw(Target,Depth,pos );
  }


  IGUIContextMenu::MESSAGERETURN IGUIContextMenu::MessageExecuting( SPGUIPARAM& pParam )
  {
    switch( pParam->Message )
    {
    case IGUIParam::MESSAGE_INITIALIZE:
      {
        m_Top.SetOffset( VECTOR2DI(0,0) );
        m_Top.SetSelectState( true );
        m_SelectList.push_back( &m_Top );
      }break;

    case IGUIParam::MESSAGE_MOUSEMOVE:
      {
        const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(*pParam);
        const POINT2DI pos = CalcLocalPosition(p.Position);

        IElement* pTop = m_SelectList.back();
        IElement* pChild = pTop->FindChild(pos);

        if( pChild!=NULL )
        { //  カーソルが子の上に載っているならフォーカスを移す
          m_SelectList.push_back(pChild);
          pChild->SetSelectState(true);
        }
        else
        {
          //  乗っていない場合は、別の子の上にいることがあるので
          //  それを探し出して、設定する
          for( SELECTLIST::reverse_iterator ite = m_SelectList.rbegin(); ite!=m_SelectList.rend(); ++ite )
          {
            IElement* p = *ite;

            IElement* pChild = p->FindChild(pos);

            if( pChild!=NULL )
            {
              while(true)
              {
                IElement* pLast = m_SelectList.back();
                if( p==pLast ) { break; }
                pLast->SetSelectState(false);
                m_SelectList.pop_back();
              }

              pChild->SetSelectState(true);
              m_SelectList.push_back(pChild);
              break;
            }
          }
        }

      }break;

    case IGUIParam::MESSAGE_MOUSEUP:
      {
        const GUIMESSAGE_MOUSEUP& p = static_cast<const GUIMESSAGE_MOUSEUP&>(*pParam);
        const POINT2DI pos = CalcLocalPosition(p.Position);

        for( SELECTLIST::reverse_iterator ite = m_SelectList.rbegin(); ite!=m_SelectList.rend(); ++ite )
        {
          const IElement::INFO* pInfo = (*ite)->FindChildInfo(pos);
          if( pInfo==NULL ) { continue; }

          //  子を持っている＝＝選択項目ではない
          if( pInfo->pChild->GetChildSize()!=0 ) { continue; }

          GUIMESSAGE_CONTEXTMENU_CLICK m;
          m.SelectID = pInfo->ID;
          PostMessage( m );
          break;
        }

      }break;
    }

    return IGUIParts::MessageExecuting( pParam );
  }
}