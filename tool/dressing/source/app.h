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
  Maid::Graphics3DRender&  Get3DRender();

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
    STATE_STORAGELOAD,  //  ストレージロード
    STATE_STORAGESAVE,  //  ストレージセーブ
    STATE_ERROR,        //  エラーが起こったので、ずっと待機
  };

  STATE GetState()const;
  void  SetState( STATE s );

  Maid::FUNCTIONRESULT  ReadConfigData( Maid::XMLReader& reader );
  Maid::FUNCTIONRESULT  ReadConfigDataParallax( Maid::XMLReader& reader );
  Maid::FUNCTIONRESULT  ReadConfigDataRect( Maid::XMLReader& reader,  Maid::RECT2DI& rc );


private:
  Maid::GraphicsCommandControl  m_Command;
  Maid::Graphics2DRender  m_2DRender;
  Maid::Graphics3DRender  m_3DRender;
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

  boost::shared_ptr<Maid::XMLFileReader> m_pStorageFile;
  boost::shared_ptr<Maid::FileWriter> m_pStorageSave;


  //  立体視用設定
  struct PARALLAXINFO
  {
    bool  Start;
    float Eye;
    float Target;
    std::vector<Maid::RECT2DI>  Left;
    std::vector<Maid::RECT2DI>  Right;
    bool BltType; //  false で左 trueで右描画
  };
  PARALLAXINFO          m_ParallaxInfo;
  Maid::XMLFileReader   m_ConfigFile;

};




#endif
