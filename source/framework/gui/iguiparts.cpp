#include"iguiparts.h"
#include"guimessageque.h"

namespace Maid
{
/*!
 	  @class	IGUIParts iguiparts.h
 	  @brief	GUIパーツのベースクラス
            新しい挙動をするGUIパーツはこのクラスを継承する
 */ 

IGUIParts::IGUIParts()
  :m_ID(PARTSID_NONE)
  ,m_pParent(NULL)
  ,m_IsEnable(true)
  ,m_IsVisible(true)
  ,m_IsMouseIn(false)
  ,m_IsKeybordFocus(false)
  ,m_Position(0,0)
  ,m_pPostTarget(NULL)
  ,m_ZOrder(ZORDER_FRONT)
{

}


IGUIParts::~IGUIParts()
{
  DelChildAll();
  MAID_ASSERT( m_ID!=PARTSID_NONE, "IGUIParam::MESSAGE_FINALIZE が来ていないのに、開放されようとしてます " << m_ID );
}


//! 状態の更新
/*!
 */
void IGUIParts::UpdateFrame()
{
  {
    //  この処理は SendMessage() 内でやるべきなんだけど、パフォーマンスに影響でそうなのでここでやる
    if( !IsEnable() ) { return ; }
    OnUpdateFrame();
  }

  //  子も実行
  for( CHILDLIST::iterator ite=m_ChildList.begin(); ite!=m_ChildList.end(); ++ite )
  {
    (*ite)->UpdateFrame();
  }
}

//! 状態の描画
/*!
    @param  Target  [i ]  描画先
    @param  Depth   [i ]  描画先
 */
void IGUIParts::UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth )
{
  {
    //  この処理は SendMessage() 内でやるべきなんだけど、パフォーマンスに影響でそうなのでここでやる
    if( !IsVisible() ) { return ; }
    OnUpdateDraw( Target, Depth, m_Position );
  }

  //  子も実行
  for( CHILDLIST::reverse_iterator ite=m_ChildList.rbegin(); ite!=m_ChildList.rend(); ++ite )
  {
    (*ite)->UpdateDraw( Target, Depth );
  }
}

//! 設置座標の変更
/*!
    @param  pos  [i ]  新しい座標
 */
void IGUIParts::SetPosition( const POINT2DI& pos )
{
  GUIMESSAGE_SETPOSITION m;
  m.Position = pos;
  SendMessage( m );
}

//! 設置座標の移動
/*!
    @param  vec  [i ]  移動量
 */
void IGUIParts::MovePosition( const VECTOR2DI& vec )
{
  GUIMESSAGE_MOVEPOSITION m;
  m.Offset = vec;
  SendMessage( m );
}

//! 状態の更新をしていいか？
/*!
    @param  IsEnable  [i ]  新しい設定
 */
void IGUIParts::SetEnable ( bool IsEnable )
{
  GUIMESSAGE_ENABLE m;
  m.IsEnable = IsEnable;
  SendMessage( m );
}

//! 状態の描画をしていいか？
/*!
    @param  IsVisible  [i ]  新しい設定
 */
void IGUIParts::SetVisible( bool IsVisible )
{
  GUIMESSAGE_VISIBLE m;
  m.IsVisible = IsVisible;
  SendMessage( m );
}

//! 現在の配置座標の取得
/*!
    座標はグローバル座標です

    @return  配置座標
 */
const POINT2DI& IGUIParts::GetPosition() const
{
  return m_Position;
}

//! 状態の更新をしていいか？
/*!
    @return  いいなら true ダメなら false
 */
bool IGUIParts::IsEnable () const
{
  return m_IsEnable;
}

//! 状態の描画をしていいか？
/*!
    @return  いいなら true ダメなら false
 */
bool IGUIParts::IsVisible() const
{
  return m_IsVisible;
}

bool IGUIParts::IsKeybordFocus() const
{
  return m_IsKeybordFocus;
}


//! パーツＩＤの取得
/*!
    メッセージを受ける場合はこのＩＤで識別するとベンリ
    @return  パーツＩＤ
 */
IGUIParts::ID   IGUIParts::GetID() const
{
  return m_ID;
}

//! 指定した座標がこのＧＵＩパーツ内に入っているか調べる
/*!
    @param  pos [i ]  調べたい座標（グローバル座標）

    @return 入っているなら true
            はいってないなら false
 */
bool IGUIParts::IsCollision( const POINT2DI& pos ) const
{
  if( !IsEnable() ) { return false; }

  //  継承先に任せる！
  const POINT2DI p = CalcLocalPosition(pos);
  return LocalIsCollision( p );
}





bool IGUIParts::IsExistChildParts( IGUIParts::ID id ) const
{
  const CHILDLIST& list = GetChildList();

  for( CHILDLIST::const_iterator ite = list.begin();
                              ite!=list.end(); ++ite )
  {
    if( (*ite)->GetID()==id ) { return true; }
  }

  return false;
}

const IGUIParts& IGUIParts::GetChildParts( IGUIParts::ID id ) const
{
  return const_cast<IGUIParts*>(this)->_GetChildParts(id);
}

IGUIParts& IGUIParts::_GetChildParts( IGUIParts::ID id )
{
  MAID_ASSERT( !IsExistChildParts(id), "存在していません " << id );

  CHILDLIST& list = m_ChildList;
  IGUIParts* p = NULL;

  for( CHILDLIST::iterator ite = list.begin();
                              ite!=list.end(); ++ite )
  {
    if( (*ite)->GetID()==id ) { p = *ite; break; }
  }

  return *p;
}


const IGUIParts::CHILDLIST& IGUIParts::GetChildList() const
{
  return m_ChildList; 
}


//! 子パーツを加える
/*!
    継承先で専用のパーツを使うと、外部とＩＤがバッティングしてしまうことがあるので
    PARTSID_LOCAL以上のＩＤは継承先専用としています

    @param  id    [i ]  加えたいパーツID(PARTSID_LOCAL 以上は使えない）
    @param  Parts [i ]  加えたいパーツ
 */
void IGUIParts::AddChildParts( IGUIParts::ID id, IGUIParts& Parts )
{
  MAID_ASSERT( !(id<PARTSID_LOCAL), "この値は使えません " << id );
  LocalAddChildParts( id, Parts );
}

//! 子パーツを加える
/*!
    @param  id    [i ]  加えたいパーツID
    @param  Parts [i ]  加えたいパーツ
 */
void IGUIParts::LocalAddChildParts( IGUIParts::ID id, IGUIParts& Parts )
{
  {
    GUIMESSAGE_INITIALIZE m;
    m.pParent = this;
    m.ID = id;

    Parts.SendMessage( m );
  }

  InsertChildList( Parts );
}

//! 子パーツをはずす
/*!
    @param  Parts [i ]  はずしたいパーツ
 */
void IGUIParts::DelChildParts( IGUIParts::ID id )
{
  IGUIParts& parts = _GetChildParts(id);

  EraseChildList( parts );
  parts.SendMessage( GUIMESSAGE_FINALIZE() );
}

//! 子パーツを全部はずす
/*!
 */
void IGUIParts::DelChildAll()
{
  const GUIMESSAGE_FINALIZE m;

  for( CHILDLIST::const_iterator ite = m_ChildList.begin();
                                  ite != m_ChildList.end(); 
                                  ++ite )
  {
    (*ite)->SendMessage( m );
  }

  m_ChildList.clear();
}

//! 子のＺオーダーを変更する
/*!
    @param  Parts [i ]  変更したいパーツ
    @param  type  [i ]  新しいＺオーダー
 */
void IGUIParts::ChangeChildZOrder( IGUIParts::ID id, ZORDER type )
{
  IGUIParts& parts = _GetChildParts(id);

  EraseChildList( parts );
  parts.m_ZOrder = type;
  InsertChildList( parts );
}

//! 自分のＺオーダーを変更する
/*!
    @param  type  [i ]  新しいＺオーダー
 */
void IGUIParts::SetZOrder( ZORDER type )
{
  if( m_pParent==NULL )
  {
    m_ZOrder = type;
  }else
  {
    m_pParent->ChangeChildZOrder( GetID(), type );
  }
}

//! 自分のＺオーダーを取得する
/*!
    @return  Ｚオーダー
 */
IGUIParts::ZORDER IGUIParts::GetZOrder()const
{
  return m_ZOrder;
}

//! 指定した座標から自分の座標との距離を調べる
/*!
    @return  離れている量
 */
POINT2DI IGUIParts::CalcLocalPosition( const POINT2DI& pos ) const
{
  #pragma COMPILERMSG("TODO:これVECTOR2DI で返すべきかもなー。要検討")	
  return POINT2DI( pos.x - m_Position.x, pos.y - m_Position.y );
}


//! PostMessage用のキューの設定
/*!
    @param  pQue  [i ]  新しいキュー
 */
void IGUIParts::SetMessageQue( GUIMessageQue* pQue )
{
  m_pPostTarget = pQue;

  for( CHILDLIST::iterator ite =m_ChildList.begin();
                           ite!=m_ChildList.end();
                           ++ite )
  {
    (*ite)->SetMessageQue( pQue );
  }
}

//! 後で実行してもらうメッセージを送っておく
/*!
    @param  Param  [i ]  新しいメッセージ
 */
void IGUIParts::PostMessage( const IGUIParam& Param )
{
  MAID_ASSERT( m_pPostTarget==NULL, "投稿先がありません" );
  m_pPostTarget->PostMessage( this, Param );
}

//! 即実行するメッセージを送る
/*!
    @param  Param  [i ]  新しいメッセージ
 */
void IGUIParts::SendMessage( const IGUIParam& Param )
{
  SPGUIPARAM p( Param.Clone() );

  if( MessageExecuting( p )==MESSAGERETURN_SENDPARENT )
  {
    if( m_pParent==NULL ) { return; }
    m_pParent->ReciveMessage( m_ID, *p );
  }
}

//! 子が処理したメッセージを受け取る
/*!
    @param  src   [i ]  メッセージを処理した子のＩＤ
    @param  Param [i ]  メッセージ内容
 */
void IGUIParts::ReciveMessage( ID src, const IGUIParam& Param )
{
  if( m_pParent==NULL ) { return; }
  m_pParent->ReciveMessage( src, Param );
}

//! ＧＵＩパーツをリスト加える
/*!
    @param  pParts [i ]  加えたいパーツ
 */
void  IGUIParts::InsertChildList( IGUIParts& Parts )
{
  IGUIParts* pParts = &Parts;

  if( m_ChildList.empty() )
  {
    m_ChildList.push_back(pParts);
    return;
  }

  const ZORDER z = pParts->GetZOrder();

  switch( z )
  {
  case ZORDER_FRONTLOCK: { m_ChildList.push_front(pParts); }break;

  case ZORDER_FRONT:
    {
      bool IsFind = false;
      for( CHILDLIST::const_iterator ite = m_ChildList.begin();
                                      ite != m_ChildList.end(); 
                                      ++ite )
      {
        if( (*ite)->GetZOrder() <= z ) 
        {
          m_ChildList.insert( ite, pParts );
          IsFind = true; 
          break;
        }
      }

      //  自分より前がないなら先頭に入れる
      if( !IsFind ) { m_ChildList.push_front(pParts); }break;
    }break;

  case ZORDER_BACK:
    {
      bool IsFind = false;
      for( CHILDLIST::const_reverse_iterator ite = m_ChildList.rbegin();
                                      ite != m_ChildList.rend(); 
                                      ++ite )
      {
        if( z <= (*ite)->GetZOrder() ) 
        {
          m_ChildList.insert( ite.base(), pParts );
          IsFind = true; 
          break;
        }
      }
      //  自分より後ろがないなら後ろに入れる
      if( !IsFind ) { m_ChildList.push_back(pParts); }break;
    }break;

  case ZORDER_BACKLOCK: { m_ChildList.push_back(pParts); }break;
  }


}

//! リストから子パーツをはずす
/*!
    @param  pParts [i ]  はずしたいパーツ
 */
void IGUIParts::EraseChildList( IGUIParts& Parts )
{
  for( CHILDLIST::const_iterator ite = m_ChildList.begin();
                                  ite != m_ChildList.end(); 
                                  ++ite )
  {
    if( (*ite)->GetID()==Parts.GetID() ) 
    {
      m_ChildList.erase( ite );
      return;
    }
  }

  MAID_ASSERT( true, "見つかりませんでした" );
}



//! マウスがパーツ内に入っているか？
/*!
    @return  入っているなら true
 */
bool IGUIParts::IsMouseIn() const
{
  return m_IsMouseIn; 
}






/// 以下、継承先クラスで実装したほうがいいもの














//! メッセージ処理の実態
/*!
    ここで実装してあるメッセージはデフォルトの操作とします。
    なので継承したクラスでも極力呼ぶようにしてください

    @param  Param [i ]  飛んできたメッセージ

    @return 飛んできたメッセージを親に通知するなら MESSAGERETURN_SENDPARENT
\n          通知しないなら MESSAGERETURN_NONE
 */
IGUIParts::MESSAGERETURN IGUIParts::MessageExecuting( SPGUIPARAM& Param )
{
  //  ここでメッセージの処理を行います
  switch( Param->Message )
  {
  case IGUIParam::MESSAGE_INITIALIZE:
    {
      const GUIMESSAGE_INITIALIZE& p = static_cast<const GUIMESSAGE_INITIALIZE&>(*Param);
      MAID_ASSERT( p.ID==PARTSID_NONE, "このIDは使用できません " << p.ID );

      if( p.pParent!=NULL )
      {
        m_pPostTarget = p.pParent->m_pPostTarget;
        m_pParent = p.pParent;
      }
      m_ID = p.ID;
      OnInitialize( p.ID, *m_pParent );
    }break;

  case IGUIParam::MESSAGE_FINALIZE:
    {
      const GUIMESSAGE_FINALIZE& p = static_cast<const GUIMESSAGE_FINALIZE&>(*Param);

      m_pParent = NULL;
      m_ID = PARTSID_NONE;

      OnFinalize();
    }break;

  case IGUIParam::MESSAGE_SETPOSITION:
    {
      const GUIMESSAGE_SETPOSITION& p = static_cast<const GUIMESSAGE_SETPOSITION&>(*Param);

      const VECTOR2DI mov(m_Position, p.Position);

      m_Position = p.Position;
      OnSetPosition( p.Position );

      for( CHILDLIST::iterator ite=m_ChildList.begin(); ite!=m_ChildList.end(); ++ite )
      {
        (*ite)->MovePosition( mov );
      }

    }break;

  case IGUIParam::MESSAGE_MOVEPOSITION:
    {
      const GUIMESSAGE_MOVEPOSITION& p = static_cast<const GUIMESSAGE_MOVEPOSITION&>(*Param);
      m_Position += p.Offset;
      OnMovePosition( p.Offset );
      for( CHILDLIST::iterator ite=m_ChildList.begin(); ite!=m_ChildList.end(); ++ite )
      {
        (*ite)->MovePosition( p.Offset );
      }
    }break;


  case IGUIParam::MESSAGE_ENABLE:
    {
      const GUIMESSAGE_ENABLE& p = static_cast<const GUIMESSAGE_ENABLE&>(*Param);
      m_IsEnable = p.IsEnable;
      OnEnable( p.IsEnable );

      for( CHILDLIST::iterator ite=m_ChildList.begin(); ite!=m_ChildList.end(); ++ite )
      {
        (*ite)->SetEnable( p.IsEnable );
      }

      //  非機能化のときに、マウスが入っていたら、出て行く
      if( !p.IsEnable && IsMouseIn() )
      {
        GUIMESSAGE_MOUSEOUT out;
        out.Position = POINT2DI(0,0);
        PostMessage( out ); 
      }

    }break;

  case IGUIParam::MESSAGE_VISIBLE:
    {
      const GUIMESSAGE_VISIBLE& p = static_cast<const GUIMESSAGE_VISIBLE&>(*Param);
      m_IsVisible = p.IsVisible;
      OnVisible( p.IsVisible );

      for( CHILDLIST::iterator ite=m_ChildList.begin(); ite!=m_ChildList.end(); ++ite )
      {
        (*ite)->SetVisible( p.IsVisible );
      }
    }break;

  case IGUIParam::MESSAGE_MOUSEIN:
    {
      const GUIMESSAGE_MOUSEIN& p = static_cast<const GUIMESSAGE_MOUSEIN&>(*Param);
      const POINT2DI pos = CalcLocalPosition(p.Position);
      m_IsMouseIn = true;
      OnMouseIn( pos );
    }break;

  case IGUIParam::MESSAGE_MOUSEOUT:
    {
      const GUIMESSAGE_MOUSEOUT& p = static_cast<const GUIMESSAGE_MOUSEOUT&>(*Param);
      const POINT2DI pos = CalcLocalPosition(p.Position);
      m_IsMouseIn = false;
      OnMouseOut( pos );
    }break;

  case IGUIParam::MESSAGE_MOUSEMOVE:
    {
      const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(*Param);
      const POINT2DI pos = CalcLocalPosition(p.Position);
      OnMouseMove( pos );
    }break;

  case IGUIParam::MESSAGE_SETKEYBORDFOCUS:
    {
      const GUIMESSAGE_SETKEYBORDFOCUS& p = static_cast<const GUIMESSAGE_SETKEYBORDFOCUS&>(*Param);
      m_IsKeybordFocus = p.IsFocus;
      OnKeybordFocus( p.IsFocus );
    }break;
  }

  return MESSAGERETURN_SENDPARENT;
}




//! 指定した座標がこのＧＵＩパーツ内に入っているか調べる
/*!
    @param  pos [i ]  調べたい座標（自分の位置を(0,0)とした座標）

    @return 入っているなら true
            はいってないなら false
 */
bool IGUIParts::LocalIsCollision( const POINT2DI& pos ) const
{
  return false; 
}

//! フレーム毎の更新はここで実装する
/*!
 */
void IGUIParts::OnUpdateFrame(){}

//! フレーム毎の描画はここで実装する
/*!
 */
void IGUIParts::OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos ){}


//! このパーツが初期化されている最中に呼ばれる
/*!
    画像の読み込みとかはここでやるのがいいでしょう
 */
void IGUIParts::OnInitialize( ID id, const IGUIParts& Parent ){}

//! このパーツが破棄されている最中に呼ばれる
/*!
    ゲーム的にはなにもしないことが多いと思います
 */
void IGUIParts::OnFinalize(){}

//! ＧＵＩパーツが動いている最中に呼ばれる
/*!
    @param  pos [i ]  設定される座標（グローバル座標）
 */
void IGUIParts::OnSetPosition( const POINT2DI& pos ){}

//! ＧＵＩパーツが動いている最中に呼ばれる
/*!
    @param  vec [i ]  設定される座標（相対座標）
 */
void IGUIParts::OnMovePosition( const VECTOR2DI& vec ){}

//! 更新状態が変更されようとしている時に呼ばれる
/*!
    @param  IsEnable [i ]  新しい状態
 */
void IGUIParts::OnEnable( bool IsEnable ){}

//! 描画状態が変更されようとしている時に呼ばれる
/*!
    @param  IsVisible [i ]  新しい状態
 */
void IGUIParts::OnVisible( bool IsVisible ){}

//! マウスがこのパーツの上を通ったときに呼ばれる
/*!
    @param  pos [i ]  通った場所（自分中心座標）
 */
void IGUIParts::OnMouseMove( const POINT2DI& pos ){}

//! マウスがこのパーツの内に入ったときに呼ばれる
/*!
    @param  pos [i ]  通った場所（自分中心座標）
 */
void IGUIParts::OnMouseIn( const POINT2DI& pos ){}

//! マウスがこのパーツから出たときに呼ばれる
/*!
    @param  pos [i ]  通った場所（自分中心座標）
 */
void IGUIParts::OnMouseOut( const POINT2DI& pos ){}


//! キーボードフォーカスを得たとき、出たときに呼ばれる
/*!
    @param  in [i ]  得たときはtrue 出たときは false
 */
void IGUIParts::OnKeybordFocus( bool in ){}

}

