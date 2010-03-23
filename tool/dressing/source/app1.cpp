#include"stdafx.h"
/*
  ゲーム用アプリケーションの雛形サンプル
*/

#include"app.h"

#include"startupdialog.h"


using namespace Maid;

MyApp* GlobalPointer<MyApp>::s_pPointer;


MyApp* s_pAPP;  //  強引すぎるハック。そのうちなんとかする

static SQInteger Import( HSQUIRRELVM v )
{
  SQInteger nargs = sq_gettop(v);

  const SQObjectType type1 = sq_gettype(v,1);
  const SQObjectType type2 = sq_gettype(v,2);

  MAID_ASSERT( type2!=OT_STRING, "importは文字列を指定してください" );

  const SQChar* pText = NULL;
  sq_getstring( v, 2, &pText );

  const String FileName = String::ConvertUNICODEtoMAID(pText);
  
  s_pAPP->ImportSquirrelFile( FileName );

  sq_pushinteger(v, nargs);
  return 1;
}


MyApp::MyApp()
  :m_2DRender(m_Command)
  ,m_3DRender(m_Command)
{

}

Maid::Graphics2DRender&  MyApp::Get2DRender() { return m_2DRender; }


bool MyApp::SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
{
  {
    const RETURNCODE ret = BeginStartupInfo( DeviceList, conf );
    if( ret==APPEXIT ) { return false; }
  }

  return true;
}

void MyApp::Initialize()
{
  GlobalPointer<MyApp>::Set();
  s_pAPP = this;

  m_Command.Initialize();
  m_2DRender.Initialize();
  m_3DRender.Initialize();
  m_Sound.Initialize();

  m_SystemFont.Create( SIZE2DI(8,16), true );
  m_Squirrel.Initialize( 1024, true );
  m_Squirrel.BindFunction( &::Import, MAIDTEXT("Import") );

  m_CompileList.push_back( MAIDTEXT("core/define.nut") );
  m_CompileList.push_back( MAIDTEXT("core/collision.nut") );
  m_CompileList.push_back( MAIDTEXT("core/vector.nut") );
  m_CompileList.push_back( MAIDTEXT("core/math.nut") );
  m_CompileList.push_back( MAIDTEXT("core/input.nut") );
  m_CompileList.push_back( MAIDTEXT("core/task.nut") );
  m_CompileList.push_back( MAIDTEXT("core/drawobject.nut") );
  m_CompileList.push_back( MAIDTEXT("core/camera.nut") );
  m_CompileList.push_back( MAIDTEXT("core/scene.nut") );
  m_CompileList.push_back( MAIDTEXT("core/gameroutine.nut") );
  m_CompileList.push_back( MAIDTEXT("core/function.nut") );
  m_CompileList.push_back( MAIDTEXT("core/storage.nut") );

  m_FadeAlpha.Set(0,0,0);
  SetState(STATE_INITIALIZING);
}

bool MyApp::Initializing() const
{
  if( m_2DRender.IsInitializing() ) { return true; }
  if( m_3DRender.IsInitializing() ) { return true; }

  return false;
}

//  squirrel側の基本ディレクトリの取得
const Maid::String& MyApp::GetCurrentDirectry() const
{
  return m_CurrentDirectry;
}

MyApp::STATE MyApp::GetState()const
{
  return m_State;
}

void  MyApp::SetState( STATE s )
{
  m_State = s;
}


void MyApp::UpdateFrame()
{
  if( !m_ScreenShot.Empty() )
  {
    if( m_FadeAlpha.IsEnd() ) { m_ScreenShot.Destroy(); }
    else                      { ++m_FadeAlpha; }
  }

  m_Sound.UpdateFrame();

  switch( GetState() )
  {
  case STATE_INITIALIZING:
    {
      //  squirrel以外の初期化をしてます。
      if( Initializing() ) { break ; }
      SetState(STATE_SCRIPTSETUP);

      m_CompileSource.Open( m_CompileList.front() );
      m_CompileList.pop_front();
    }break;

  case STATE_SCRIPTSETUP:
    {
      //  squirrelの基本ライブラリの作成
      FileReaderAll& hFile = m_CompileSource;
      if( hFile.IsExecuting() ) { break; }

      const String FileName = hFile.GetFileName();
      const std::string tmp = std::string( (char*)hFile.GetBuffer().GetPointer(0), hFile.GetBuffer().GetSize() );
      String SourceCode = String::ConvertSJIStoMAID(tmp);

      const FUNCTIONRESULT ret = m_Squirrel.Import( SourceCode, FileName );
      if( FUNCTIONRESULT_FAILE(ret) ) { SetState(STATE_ERROR); break; }

      //  インポートが終わった
      if( m_CompileList.empty() )
      {
        String FilePath = MAIDTEXT("boot/entrypoint.nut");
        if( !GetStorage().IsFileExist(FilePath) )
        {
          //  もし boot/entrypoint.nut が存在していないなら、選択ダイアログを表示する
          const String Tittle = MAIDTEXT("エントリポイントファイルの指定");
          const String DefExt = MAIDTEXT(".nut");
          const wchar_t* Filter = L".nut\0\0";
          const String DefaultFolder = Shell::GetCurrentDirectory();
          const String DefaultName = MAIDTEXT("");

          const String ret = Shell::ShowOpenDialog( NULL, Tittle, DefExt, Filter, DefaultFolder, DefaultName );
          TimerReset();

          if( ret.empty() ) { MAID_WARNING("ファイルが指定されませんでした"); return ; }
          FilePath = ret;
        }

        m_CompileSource.Open( FilePath );
        SetState(STATE_CREATEENTRYPOINT);
      }else
      {
        m_CompileSource.Open( m_CompileList.front() );
        m_CompileList.pop_front();
      }

    }break;

  case STATE_CREATEENTRYPOINT:
    {
      //  エントリポイントの作成
      FileReaderAll& hFile = m_CompileSource;
      if( hFile.IsExecuting() ) { break; }

      const String FileName = hFile.GetFileName();
      const std::string tmp = std::string( (char*)hFile.GetBuffer().GetPointer(0), hFile.GetBuffer().GetSize() );
      String SourceCode = String::ConvertSJIStoMAID(tmp);

      m_CurrentDirectry = String::GetDirectory(FileName);

      const FUNCTIONRESULT ret = m_Squirrel.Execute( SourceCode, FileName, m_SuspendState );
      if( FUNCTIONRESULT_FAILE(ret) ) { SetState(STATE_ERROR); break; }

      SetState(STATE_GAMEPLAY);
      Wakeup();
    }break;

  case STATE_GAMEPLAY:
    {
      Wakeup();
    }break;

  case STATE_STORAGELOAD:
    {
      if( m_pStorageFile->IsLoading() ) { break; }

      SetState( STATE_GAMEPLAY );
      m_Squirrel.WakeupStorageLoad( m_pStorageFile->GetReader(), m_SuspendState );
      Wakeup();
    }break;

  case STATE_STORAGESAVE:
    {
      if( m_pStorageSave->IsExecuting() ) { break; }
      m_pStorageSave.reset();
      SetState( STATE_GAMEPLAY );
      m_Squirrel.WakeupStorageSave( m_SuspendState );
      Wakeup();
    }break;

  case STATE_EXIT:
    {
    }break;

  case STATE_ERROR:
    {

    }break;
  }
}



void MyApp::UpdateDraw()
{
  const RenderTargetBase& rt = GetGraphicsCore().GetBackBuffer();
  const IDepthStencil& dp = GetGraphicsCore().GetDepthStencil();

  m_Command.SetRenderTarget( rt, dp );
  m_Command.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(1,0,0,0) );

  m_Command.Begin();

  switch( GetState() )
  {
  case STATE_INITIALIZING:
    {

    }break;
  case STATE_SCRIPTSETUP:
    {
      m_2DRender.BltText( POINT2DI(0,0), m_SystemFont, MAIDTEXT("スクリプトの読み込み中" ) );
    }break;

  case STATE_CREATEENTRYPOINT:
    {
      m_2DRender.BltText( POINT2DI(0,0), m_SystemFont, MAIDTEXT("エントリポイント作成中" ) );
    }break;

  case STATE_GAMEPLAY:
    {
      GameDraw();
    }break;
  case STATE_STORAGELOAD:
    {
      GameDraw();
    }break;
  case STATE_STORAGESAVE:
    {
      GameDraw();
    }break;

  case STATE_EXIT:
    {

    }break;

  case STATE_ERROR:
    {

    }break;
  }

  if( !m_ScreenShot.Empty() ) 
  {
    const RECT2DI rect( POINT2DI(0,0), rt.GetSize() );
    const float alpha = m_FadeAlpha;
    m_2DRender.SetBlendState( Graphics2DRender::BLENDSTATE_ALPHA );
    m_2DRender.Blt( POINT2DI(0,0), m_ScreenShot, rect, POINT2DI(0,0), alpha );
  }

  m_Command.End();
}

void MyApp::Finalize()
{
  if( GetState()!=STATE_ERROR )
  {
    m_Squirrel.ExitGameLoop();

    while( GetState()!=STATE_EXIT )
    {
      //Wakeup();
      UpdateFrame();
    }
  }

  m_Squirrel.Finalize();
}

void MyApp::Wakeup()
{
  while( true )
  {
    SquirrelWrapper::RETURNCODE& ret = m_SuspendState;
    switch( ret.Code )
    {
    case SquirrelWrapper::RETURNCODE::EXIT:
      {
        SetState( STATE_EXIT );
        return ;
      }break;
    case SquirrelWrapper::RETURNCODE::NEXTFRAME:
      {
        m_Squirrel.WakeupNextFrame( ret );
        return ;
      }break;

    case SquirrelWrapper::RETURNCODE::SCENEFADE:
      {
        BeginSceneFade();
        m_Squirrel.WakeupSceneFade( ret );
      }break;

    case SquirrelWrapper::RETURNCODE::INPUTSTATE:
      {
        const Keybord& key = GetKeybord();
        const Mouse& mou = GetMouse();

        m_Squirrel.WakeupInputState( key, mou, ret );
      }break;

    case SquirrelWrapper::RETURNCODE::STORAGELOAD:
      {
        m_pStorageFile.reset( new XMLFileReader() );
        if( GetStorage().IsFileExist(ret.Param[0]) )
        {
          SetState( STATE_STORAGELOAD );
          m_pStorageFile->Load( ret.Param[0] );
          return ;
        }else
        {
          m_Squirrel.WakeupStorageLoad( m_pStorageFile->GetReader(), ret );
        }
      }break;
    case SquirrelWrapper::RETURNCODE::STORAGESAVE:
      {
        XMLWriter xml;
        m_Squirrel.GetStorageData( xml );
        std::string data;
        xml.Save( data );

        m_pStorageSave.reset( new FileWriter );
        m_pStorageSave->Create( m_SuspendState.Param[0] );
        m_pStorageSave->Write( data.c_str(), data.length() );

        SetState( STATE_STORAGESAVE );
          return ;
      }break;
    }
  }
}


void MyApp::ImportSquirrelFile( const String& FileName )
{
  const String path = m_CurrentDirectry + MAIDTEXT("\\") + FileName;

  FileReaderAll hFile;
  hFile.Open( path );
  hFile.Sync();

  const std::string tmp = std::string( (char*)hFile.GetBuffer().GetPointer(0), hFile.GetBuffer().GetSize() );
  String SourceCode = String::ConvertSJIStoMAID(tmp);

  const FUNCTIONRESULT ret = m_Squirrel.Import( SourceCode, FileName );
  if( FUNCTIONRESULT_FAILE(ret) ) { SetState(STATE_ERROR); return; }
}
