#include"iguicontextmenu.h"
#include"../../auxiliary/collision.h"
#include"../../auxiliary/debug/warning.h"

namespace Maid
{

/*!
 	  @class	IGUIContextMenu iguicontextmenu.h
 	  @brief	コンテキストメニュー(windowsで言う、右クリックで出てくるアレ)
            IElement は１要素をあらわします
            縦並びです
*/ 

  IGUIContextMenu::IElement::IElement()
    :m_IsSelect(false)
    ,m_pParent(NULL)
    ,m_Offset(0,0)
  {
  }


  //! 要素を子として追加する
  /*!
      @param  id    [i ]  idの順に並びます。クリックされたときに GUIMESSAGE_CONTEXTMENU_CLICK.SelectID が飛んでくる
      @param  parts [i ]  追加するパーツ
   */
  void IGUIContextMenu::IElement::Insert( int id, IElement& parts )
  {
    parts.m_pParent = this;
    m_Child.insert( INFO( id, &parts ) );
    UpdateChildPosition();
  }

  //! すでに追加してある子を消す
  /*!
      @param  id    [i ] 消すid
   */
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

  //! すべての子要素を消す
  /*!
   */
  void IGUIContextMenu::IElement::DeleteAll()
  {
    m_Child.clear();
  }

  //! 子要素の横幅と縦幅を求める
  /*!
   */
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

  //! 子要素の表示範囲を求める
  /*!
   */
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

  //! この要素を管理している親の取得
  /*!
      @return  親
   */
  const IGUIContextMenu::IElement* IGUIContextMenu::IElement::GetParent() const
  {
    return m_pParent;
  }

  //! 指定した座標にある子要素を探す
  /*!
      @param    pos [i ] 探す座標（ルート(m_pParent==NULL)のとこからの相対座標です）

      @return   範囲内にあった要素
                見つからなかったら NULL
   */
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

  //! 指定した座標にある子要素を探す
  /*!
      @param    pos [i ] 探す座標（ルート(m_pParent==NULL)のとこからの相対座標です）

      @return   範囲内にあった要素
                見つからなかったら NULL
   */
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


  //! この要素が範囲内にあるか？
  /*!
      @param    pos [i ] 探す座標（ルート(m_pParent==NULL)のとこからの相対座標です）

      @return   範囲内にあるなら true
                ないなら false
   */
  bool IGUIContextMenu::IElement::IsCollision( const POINT2DI& pos ) const
  {
    const SIZE2DI size = GetSize();
    const POINT2DI off(m_Offset.x,m_Offset.y);

    return Collision<int>::IsPointToRect( pos, RECT2DI(off,size) );
  }

  //! １フレーム進める
  /*!
   */
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

  //! 描画する
  /*!
   */
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

  //! 現在この要素が選択されているか？
  /*!
      @param  選択されているなら true
   */
  bool IGUIContextMenu::IElement::IsSelect() const
  {
    return m_IsSelect;
  }


  //! 選択状態を変更する
  /*!
      @param  on  [i ]  選択するなら true
   */
  void IGUIContextMenu::IElement::SetSelectState( bool on )
  {
    m_IsSelect = on;
  }

  //! 座標の設定
  /*!
      @param    off [i ] 設定する座標（ルート(m_pParent==NULL)のとこからの相対座標です）
   */
  void IGUIContextMenu::IElement::SetOffset( const VECTOR2DI& off )
  {
    m_Offset = off;
    UpdateChildPosition();
  }

  //! 座標の取得
  /*!
      @return    off [i ] 座標（ルート(m_pParent==NULL)のとこからの相対座標です）
   */
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

  //! 子要素を何個もっているか？
  /*!
      @return    個数
   */
  int IGUIContextMenu::IElement::GetChildSize() const
  {
    return (int)m_Child.size();
  }
















  IGUIContextMenu::IGUIContextMenu()
  {
  }


  //! 要素を子として追加する
  /*!
      @param  id    [i ]  クリックされたときに GUIMESSAGE_CONTEXTMENU_CLICK.SelectID が飛んでくる
      @param  parts [i ]  追加するパーツ
   */
  void IGUIContextMenu::Insert( int id, IElement& parts )
  {
    m_Top.Insert( id, parts );
  }

  //! すでに追加してある子を消す
  /*!
      @param  id    [i ] 消すid
   */
  void IGUIContextMenu::Delete( int id )
  {
    m_Top.Delete( id );
  }

  //! すべての子要素を消す
  /*!
   */
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

    case IGUIParam::MESSAGE_FINALIZE:
      {
        for( SELECTLIST::reverse_iterator ite = m_SelectList.rbegin(); ite!=m_SelectList.rend(); ++ite )
        {
          (*ite)->SetSelectState(false);
        }

        m_SelectList.clear();
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