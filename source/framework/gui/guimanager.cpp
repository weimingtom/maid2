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
{
  //  これをやっておくことで UpdateMouseMessage() 内で
  //  m_pMousePrevParts!=NULL チェックの必要がなくなる
  m_pMousePrevParts = &m_Root;
  //  m_pKeybordFocus!=NULL チェックもなくなる
  m_pKeybordFocus = &m_Root;

  m_Root.SetMessageQue( &m_MessageQue );

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
  m_Root.SetReviceFunction( Func );
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
void GUIManager::DeleteParts( IGUIParts& Parts )
{
  m_Root.DelChildParts( Parts );
}

//! ＧＵＩパーツの全削除
/*!
    すでに登録してあるＧＵＩパーツを全部はずします
 */
void GUIManager::DeletePartsAll()
{
  m_Root.DelChildAll();
}



//! 指定した座標にあるＧＵＩパーツを探します
/*!
    @param  pos [i ]  調べたい座標

    @return 見つかった座標
 */
IGUIParts* GUIManager::FindParts( const POINT2DI& pos )
{
  IGUIParts* pCurrent = m_Root.FindChildParts( pos );

  if( pCurrent!=NULL ) { return pCurrent; }


  //  ここまできたってことは指定している座標に登録してあるオブジェクトがなかったってことなんだけど
  //  そういうときは何もしないクラスを渡す
  //  これをやっておくことで UpdateMouseMessage() 内で
  //  m_pMousePrevParts.get()!=NULL チェックの必要がなくなる
  return &m_Root;
}


//! マウス関連のメッセージを作成する
/*!
    @param  mouse [i ]  マウスデバイス
    @param  Que   [ o]  作成したメッセージを追加するキュー
 */
void GUIManager::UpdateMouseMessage( const Mouse& mouse, GUIMessageQue& Que )
{
  const POINT2DI pos  = POINT2DI( mouse.GetX(), mouse.GetY() );

  IGUIParts* pCapture = m_pMouseCapturingParts;
  IGUIParts* pNext    = FindParts( pos );
  IGUIParts* pPrev    = m_pMousePrevParts;

  const bool IsCapturing = pCapture!=NULL;


  if( pPrev!=pNext )
  {
    //  現在と今のパーツが違っていたら、パーツのインアウトメッセージ
    if( IsCapturing ) 
    {
      {
        //  キャプチャと前回のが違う == キャプチャから出た
        if( pCapture==pPrev )
        { 
          GUIMESSAGE_MOUSEOUT out;
          out.Position = pos;
          Que.PostMessage( pCapture, out ); 
        }
      }
      {
        if( pCapture==pNext )
        {
          //  今回がキャプチャ == キャプチャに入る
          GUIMESSAGE_MOUSEIN in;
          in.Position = pos;
          Que.PostMessage( pCapture, in ); 
        }
      }
    }else
    {
      GUIMESSAGE_MOUSEOUT out;
      out.Position = pos;
      Que.PostMessage( pPrev, out ); 

      GUIMESSAGE_MOUSEIN in;
      in.Position = pos;
      Que.PostMessage( pNext, in ); 
    }
    m_pMousePrevParts = pNext;
  }


  if( mouse.GetDeltaX()!=0 || mouse.GetDeltaY()!=0 )
  {
    //  マウスが動いたら動いたらメッセージ
    GUIMESSAGE_MOUSEMOVE m;
    m.Position = pos;

    if( IsCapturing ) { Que.PostMessage( pCapture, m );  }
    else              { Que.PostMessage( pNext, m );     }
  }

  //  左ボタンを押したらキャプチャの開始
  if( mouse.IsInL() )
  {
    {
      GUIMESSAGE_SETFOCUS m;
      m.IsFocus = false;
      Que.PostMessage( m_pKeybordFocus, m );
    }
    {
      GUIMESSAGE_SETFOCUS m;
      m.IsFocus = true;
      Que.PostMessage( pNext, m );
      m_pKeybordFocus = pNext;  //  フォーカスを得る
    }
    GUIMESSAGE_MOUSEDOWN m;
    m.Position = pos;
    m.Button = IGUIParam::BUTTON_LEFT;

    Que.PostMessage( pNext, m );

    m_pMouseCapturingParts = pNext;
  }

  //  左ボタンを離したらキャプチャの終了
  if( mouse.IsOutL() )
  {
    GUIMESSAGE_MOUSEUP m;
    m.Position = pos;
    m.Button = IGUIParam::BUTTON_LEFT;
    Que.PostMessage( pCapture, m );
    m_pMousePrevParts = m_pMouseCapturingParts;
    m_pMouseCapturingParts = NULL;
  }
}


//! キーボード関連のメッセージを作成する
/*!
    @param  key [i ]  キーボードデバイス
    @param  Que [ o]  作成したメッセージを追加するキュー
 */
void GUIManager::UpdateKeybordMessage( const Keybord& key, GUIMessageQue& Que )
{

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


}

