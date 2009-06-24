/*
  ゲーム用アプリケーションの雛形サンプル
*/


#include"../../source/framework/application.h"
#include"../../source/framework/gameframework.h"
#include"../../source/framework/igamethread.h"
#include"../../source/graphics/graphicscommandcontrol.h"

#include"startupdialog.h"

using namespace Maid;


class MyApp : public IGameThread
{
protected:
  bool SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
  {
    //  ここでは引数で渡されたデバイスリストをユーザーに選択させる処理を書く
    //  ユーザーの選択結果は conf に設定するんだけど、設定しなくても自動的になんとかしてくれる

    {
      //  サンプルとして選択ダイアログを出すようにしておきます。
      const RETURNCODE ret = BeginStartupInfo( DeviceList, conf );
      //  ここでアプリケーションを終了したいときは return false です。
      if( ret==APPEXIT ) { return false; }
    }

    return true;
  }

  void Initialize()
  {
    //  ここまでくると、SelectDevice() で指定した設定で、各種初期化されています
    //  自分のメンバを初期化する
    m_Command.Initialize();

  }

  void UpdateFrame()
  {
    //  フレーム毎に呼ばれるとこ
    //  終了する場合は OnExitGame() を呼ぶ

  }

  void UpdateDraw()
  {
    //  定期的に描画するとこ

    m_Command.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(1,1,1,0) );

  }

  void Finalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  GraphicsCommandControl  m_Command;
};



int WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, 
    int nCmdShow
)
{
  GameFrameWork app( SPGAMETHREAD(new MyApp) );
  
  return app.Run(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
}

