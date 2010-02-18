#include"guimanager.h"
#include"../../auxiliary/debug/assert.h"

namespace Maid
{



/*!
 	@class	GUIManager guimanager.h
 	@brief	各種GUIPartsにメッセージを送るマネージャ
\n        だけど、各GUIパーツの寿命は管理していないため、気をつけてね

 	@code
  class TittleScene
  {
  public:

  private:
    GUIManager  m_Manager;  // m_Managerのデストラクタでm_Buttonを参照するのでガッ！されることもある(ハード依存)
    GUIButton   m_Button;   
  };

  class BGMScene
  {
  public:

  private:
    GUIButton   m_Button;   
    GUIManager  m_Manager;  こうすると正しくなる
  };
 	@endcode
 */


GUIManager::GUIManager()
:m_pMouseCapturingParts(NULL)
,m_pKeybordFocus(NULL)
,m_pInterruptParts(NULL)
{
  //  これをやっておくことで UpdateMouseMessage() 内で
  //  m_pMousePrevParts!=NULL チェックの必要がなくなる
  m_pMousePrevParts = &m_Root;
  //  m_pKeybordFocus!=NULL チェックもなくなる
  m_pKeybordFocus = &m_Root;

  m_Root.SetMessageQue( &m_MessageQue );
  m_Root.SetReviceFunction( MakeReciveMessageFunction(&GUIManager::ReciveMessage,this) );
}

GUIManager::~GUIManager()
{
  DeletePartsAll();
}


//! ＧＵＩパーツで起こったイベントを通知する関数を設定する
/*!
    @param  Func  [i ] 設定する関数。
 */
void GUIManager::SetReviceFunction( const RECIVEFUNCTION& Func )
{
  m_ReceiveFunction = Func;
}


//! パーツの更新
/*!
    コイツが各種メッセージを作って、送信します

    @param  pMouse  [i ] マウス（ＮＵＬＬにするとマウス関連のメッセージが飛びません）
    @param  pKey    [i ] キーボード（ＮＵＬＬにするとキーボード関連のメッセージが飛びません）
    @param  pText   [i ] ＩＭ関連（ＮＵＬＬにするとＩＭ関連のメッセージが飛びません）
 */
void GUIManager::UpdateFrame( const Mouse* pMouse, const Keybord* pKey, const TextInput* pText )
{
  //  とりあえず管理してるフレームを進める
  m_Root.UpdateFrame();

  {
    //  次に起こっているメッセージを探す
    if( pMouse!=NULL )
    {
      UpdateMouseMessage( *pMouse, m_MessageQue );
    }
  }

  {
    if( pKey!=NULL )
    {
      UpdateKeybordMessage( *pKey, m_MessageQue );
    }
    if( pText!=NULL )
    {
      UpdateTextMessage( *pText, m_MessageQue );
    }
  }

  {
    //  最後に実行してもらうー
    m_MessageQue.Execute();
  }
}


//! ＧＵＩパーツの描画
/*!
    @param  Target  [i ] 描画先
    @param  Depth   [i ] 描画先
 */
void GUIManager::UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth )
{
  m_Root.UpdateDraw( Target, Depth );
}


//! 指定したパーツを強制割り込みします
/*!
    フォーカスが外れる事で指定されたパーツも消えます

    @param  pos   [i ] 配置座標
    @param  id    [i ] menu のID
    @param  parts [i ] 使うパーツ
 */
void GUIManager::BeginInterruptParts( const POINT2DI& pos, IGUIParts::ID id, IGUIParts& parts )
{
  parts.SetPosition( pos );

  RegisterParts( id, parts );

  SetFocus( id );

  m_pInterruptParts = &parts;
}



//! ＧＵＩパーツの登録
/*!
    登録することで、このクラスがメッセージの送信先として認知します

    @param  id    [i ] イベントがあった際にSetReviceFunction（）に通知されるＩＤ
    @param  Parts [i ] 登録するＧＵＩパーツ
 */
void GUIManager::RegisterParts( IGUIParts::ID id, IGUIParts& Parts )
{
  m_Root.AddChildParts( id, Parts );
  m_Root.SetMessageQue( &m_MessageQue );
}


//! ＧＵＩパーツの削除
/*!
    すでに登録してあるＧＵＩパーツをこのクラスからはずします
    
    @param  Parts [i ] 登録するＧＵＩパーツ
 */
void GUIManager::DeleteParts( IGUIParts::ID id )
{
  m_Root.DelChildParts( id );
}

//! ＧＵＩパーツの全削除
/*!
    すでに登録してあるＧＵＩパーツを全部はずします
 */
void GUIManager::DeletePartsAll()
{
  m_Root.DelChildAll();

  m_pMousePrevParts = NULL;
  m_pInterruptParts = NULL;
  m_pMousePrevParts = &m_Root;
  m_pKeybordFocus = &m_Root;

}

void GUIManager::SetFocus( IGUIParts::ID id )
{
  IGUIParts* pTarget = FindChildParts( id, m_Root );

  MAID_ASSERT( pTarget==NULL, "そのidは存在してません " << id );

  MakeMessageSETKEYBORDFOCUS( pTarget, m_MessageQue );
}

IGUIParts::ID GUIManager::GetFocus() const
{
  if( m_pKeybordFocus==NULL ) { return IGUIParts::PARTSID_NONE; }

  return m_pKeybordFocus->GetID();
}




//! 指定した座標にあるＧＵＩパーツを探します
/*!
    @param  pos [i ]  調べたい座標

    @return 見つかったパーツ
 */
IGUIParts* GUIManager::FindParts( const POINT2DI& pos )
{
  IGUIParts* pCurrent = FindChildParts( pos, m_Root );

  if( pCurrent!=NULL ) { return pCurrent; }


  //  ここまできたってことは指定している座標に登録してあるオブジェクトがなかったってことなんだけど
  //  そういうときは何もしないクラスを渡す
  //  これをやっておくことで UpdateMouseMessage() 内で
  //  m_pMousePrevParts.get()!=NULL チェックの必要がなくなる
  return &m_Root;
}


IGUIParts* GUIManager::FindChildParts( const POINT2DI& pos, IGUIParts& parts )
{
  const IGUIParts::CHILDLIST& list = parts.GetChildList();

  for( IGUIParts::CHILDLIST::const_iterator ite=list.begin(); ite!=list.end(); ++ite )
  {
    IGUIParts* p = *ite;

    IGUIParts* pCurrent = FindChildParts( pos, *p );

    if( pCurrent!=NULL ) { return pCurrent; }

    if( p->IsCollision(pos) ) { return p; }
  }

  return NULL;
}

//! 指定した id のＧＵＩパーツを探します
/*!
    @param  id [i ]  調べたいid

    @return 見つかったパーツ　見つからなければNULL
 */
IGUIParts* GUIManager::FindChildParts( IGUIParts::ID id, IGUIParts& parts )
{
  const IGUIParts::CHILDLIST& list = parts.GetChildList();

  for( IGUIParts::CHILDLIST::const_iterator ite=list.begin(); ite!=list.end(); ++ite )
  {
    IGUIParts* p = *ite;

    IGUIParts* pCurrent = FindChildParts( id, *p );

    if( pCurrent!=NULL ) { return pCurrent; }

    if( p->GetID()==id ) { return p; }
  }

  return NULL;
}


//! マウス関連のメッセージを作成する
/*!
    @param  mouse [i ]  マウスデバイス
    @param  Que   [ o]  作成したメッセージを追加するキュー
 */
void GUIManager::UpdateMouseMessage( const Mouse& mouse, GUIMessageQue& Que )
{
  const POINT2DI pos  = POINT2DI( mouse.GetX(), mouse.GetY() );
  const bool IsMove = mouse.GetDeltaX()!=0 || mouse.GetDeltaY()!=0;

  IGUIParts* pCapture = m_pMouseCapturingParts;
  IGUIParts* pNext    = FindParts( pos );
  IGUIParts* pPrev    = m_pMousePrevParts;

  const bool IsCapturing = pCapture!=NULL;

  //  マウスキャプチャ中の場合は現在フォーカスがあたっているところだけメッセージのやり取りを行う
  if( IsCapturing )
  {
    if( IsMove )
    {
      if( pPrev!=pNext )
      {
        if( pPrev==m_pMouseCapturingParts )
        {
          GUIMESSAGE_MOUSEOUT out;
          out.Position = pos;
          Que.PostMessage( pCapture, out ); 
        }
        else if( pNext==m_pMouseCapturingParts )
        {
          GUIMESSAGE_MOUSEIN in;
          in.Position = pos;
          Que.PostMessage( pCapture, in ); 
        }
        m_pMousePrevParts = pNext;
      }


      //  マウスが動いたら動いたらメッセージ
      GUIMESSAGE_MOUSEMOVE m;
      m.Position = pos;
      Que.PostMessage( pCapture, m );
    }

    if( mouse.IsInL() ){ MakeMessageMOUSEDOWN( pCapture, pos, IGUIParam::BUTTON_LEFT,  Que ); }
    if( mouse.IsOutL()){ MakeMessageMOUSEUP  ( pCapture, pos, IGUIParam::BUTTON_LEFT,  Que ); }
//    右ボタンは使わない雰囲気あるのでコメントアウト
//    if( mouse.IsInR() ){ MakeMessageMOUSEDOWN( pCapture, pos, IGUIParam::BUTTON_RIGHT, Que ); }
//    if( mouse.IsOutR()){ MakeMessageMOUSEUP  ( pCapture, pos, IGUIParam::BUTTON_RIGHT, Que ); }
  }else
  {
    if( IsMove )
    {
      if( pPrev!=pNext )
      {
        //  現在と今のパーツが違っていたら、パーツのインアウトメッセージ
        GUIMESSAGE_MOUSEOUT out;
        out.Position = pos;
        Que.PostMessage( pPrev, out ); 

        GUIMESSAGE_MOUSEIN in;
        in.Position = pos;
        Que.PostMessage( pNext, in ); 

        m_pMousePrevParts = pNext;
      }

      //  マウスが動いたら動いたらメッセージ
      GUIMESSAGE_MOUSEMOVE m;
      m.Position = pos;

      Que.PostMessage( pNext, m );
    }

    if( mouse.IsInL()  ){ MakeMessageMOUSEDOWN( pNext, pos, IGUIParam::BUTTON_LEFT,  Que ); }
    if( mouse.IsOutL() ){ MakeMessageMOUSEUP  ( pNext, pos, IGUIParam::BUTTON_LEFT,  Que ); }
//    右ボタンは使わない雰囲気あるのでコメントアウト
//    if( mouse.IsInR()  ){ MakeMessageMOUSEDOWN( pNext, pos, IGUIParam::BUTTON_RIGHT, Que ); }
//    if( mouse.IsOutR() ){ MakeMessageMOUSEUP  ( pNext, pos, IGUIParam::BUTTON_RIGHT, Que ); }
  }

}








void GUIManager::MakeMessageSETKEYBORDFOCUS( IGUIParts* pParts, GUIMessageQue& Que )
{
  if( m_pKeybordFocus==pParts ) { return ; }

  if( m_pKeybordFocus!=NULL )
  {
    GUIMESSAGE_SETKEYBORDFOCUS m;
    m.IsFocus = false;
    Que.PostMessage( m_pKeybordFocus, m );
  }

  if( pParts!=NULL )
  {
    GUIMESSAGE_SETKEYBORDFOCUS m;
    m.IsFocus = true;
    Que.PostMessage( pParts, m );
    m_pKeybordFocus = pParts;  //  フォーカスを得る
  }
}

void GUIManager::MakeMessageMOUSEDOWN( IGUIParts* pTarget, const POINT2DI& pos, IGUIParam::BUTTONTYPE type, GUIMessageQue& Que )
{
  MakeMessageSETKEYBORDFOCUS( pTarget, Que );

  GUIMESSAGE_MOUSEDOWN m;
  m.Position = pos;
  m.Button = type;

  Que.PostMessage( pTarget, m );

  m_pMouseCapturingParts = pTarget;
}

void GUIManager::MakeMessageMOUSEUP( IGUIParts* pTarget, const POINT2DI& pos, IGUIParam::BUTTONTYPE type, GUIMessageQue& Que )
{
  GUIMESSAGE_MOUSEUP m;
  m.Position = pos;
  m.Button = type;
  Que.PostMessage( pTarget, m );
  m_pMousePrevParts = pTarget;
  m_pMouseCapturingParts = NULL;
}

//! キーボード関連のメッセージを作成する
/*!
    @param  key [i ]  キーボードデバイス
    @param  Que [ o]  作成したメッセージを追加するキュー
 */
void GUIManager::UpdateKeybordMessage( const Keybord& key, GUIMessageQue& Que )
{
  for( int i=0; i<=0xFF; ++i )
  {
    if( key.IsIn(i) )
    {
      GUIMESSAGE_KEYBORDDOWN m;
      m.Key = i;
      Que.PostMessage( m_pKeybordFocus, m );
    }

    if( key.IsOut(i) )
    {
      GUIMESSAGE_KEYBORDUP m;
      m.Key = i;
      Que.PostMessage( m_pKeybordFocus, m );
    }
  }
}

//! IM関連のメッセージを作成する
/*!
    @param  text  [i ]  IMデバイス
    @param  Que   [ o]  作成したメッセージを追加するキュー
 */
void GUIManager::UpdateTextMessage( const TextInput& text, GUIMessageQue& Que )
{
  if( !text.IsEvent() ) { return; }

  GUIMESSAGE_TEXTBOX_UPDATESTATE m;
  m.CharaCode = text.GetCharaCode();
  m.IMCompString = text.GetIMCompString();
  m.IMCompPosition = text.GetIMCompPosition();
  m.IsIMOpen = text.IsIMOpen();

  Que.PostMessage( m_pKeybordFocus, m );
}


void GUIManager::ReciveMessage( IGUIParts::ID src, const IGUIParam& Param )
{
  if( m_pInterruptParts!=NULL )
  {
    if( src==m_pInterruptParts->GetID() && Param.Message==IGUIParam::MESSAGE_SETKEYBORDFOCUS )
    {
      const GUIMESSAGE_SETKEYBORDFOCUS& m = static_cast<const GUIMESSAGE_SETKEYBORDFOCUS&>(Param);

      if( !m.IsFocus )
      {
        DeleteParts( src );
        m_pInterruptParts = NULL;
      }
    }
  }

  m_ReceiveFunction( src, Param );
}


}

