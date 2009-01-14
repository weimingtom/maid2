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
{
  //  これをやっておくことで UpdateMouseMessage() 内で
  //  m_pMousePrevParts.get()!=NULL チェックの必要がなくなる
  m_pMousePrevParts = &m_Root;

  m_Root.SetMessageQue( &m_MessageQue );

}

GUIManager::~GUIManager()
{
  DeletePartsAll();
}

void GUIManager::SetReviceFunction( const RECIVEFUNCTION& Func )
{
  m_Root.SetReviceFunction( Func );
}

void GUIManager::UpdateFrame( const Mouse* pMouse, const Keybord* pKey )
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
    //  最後に実行してもらうー
    m_MessageQue.Execute();
  }
}



void GUIManager::UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth )
{
  m_Root.UpdateDraw( Target, Depth, POINT2DI(0,0) );
}



void GUIManager::RegisterParts( IGUIParts::ID id, IGUIParts& Parts )
{
  m_Root.AddChildParts( id, Parts );
}

void GUIManager::DeleteParts( IGUIParts& Parts )
{
  m_Root.DelChildParts( Parts );
}

void GUIManager::DeletePartsAll()
{
  m_Root.DelChildAll();
}




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

}




