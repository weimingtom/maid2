#ifndef tool_dressing_app_h
#define tool_dressing_app_h





#include"squirrelwrapper.h"


class MyApp 
  : public Maid::IGameThread
  , public Maid::GlobalPointer<MyApp>
{
public:
  MyApp();
  Maid::Graphics2DRender&  Get2DRender();

  const Maid::String& GetCurrentDirectry() const;


  void ImportSquirrelFile( const Maid::String& FileName );

protected:
  bool SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf );
  void Initialize();
  void UpdateFrame();
  void UpdateDraw();
  void Finalize();

private:
  bool Initializing() const;
  void GameDraw();

  void Wakeup();
  void BeginSceneFade();

  enum STATE
  {
    STATE_INITIALIZING, //  初期化中
    STATE_SCRIPTSETUP,  //  スクリプトセットアップ中
    STATE_CREATEENTRYPOINT,  //  エントリポイント作成中
    STATE_GAMEPLAY,     //  ゲームプレイ中
    STATE_EXIT,         //  終了待機
    STATE_ERROR,        //  エラーが起こったので、ずっと待機
  };

  STATE GetState()const;
  void  SetState( STATE s );

private:
  Maid::GraphicsCommandControl  m_Command;
  Maid::Graphics2DRender  m_2DRender;
  Maid::GameSoundManager  m_Sound;

  STATE m_State;
  SquirrelWrapper m_Squirrel;

  SquirrelWrapper::RETURNCODE m_SuspendState;

  std::list<Maid::String>  m_CompileList;
  Maid::FileReaderAll      m_CompileSource;

  Maid::Font  m_SystemFont;

  Maid::String  m_CurrentDirectry;

  Maid::LinearCounter<float>  m_FadeAlpha;
  Maid::RenderTargetTexture   m_ScreenShot;

};




#endif
