#ifndef tool_dressing_squirrelwrapper_h
#define tool_dressing_squirrelwrapper_h

#include"bindobject.h"


class SquirrelWrapper
{
public:
  SquirrelWrapper();
  virtual ~SquirrelWrapper();

  Maid::FUNCTIONRESULT Initialize( int StackSize, bool raiseerror );
	void Finalize();

  Maid::FUNCTIONRESULT BindFunction( SQFUNCTION func, const Maid::String& Name );
  Maid::FUNCTIONRESULT Import( const Maid::String& SourceCode, const Maid::String& FileName );

  struct RETURNCODE
  {
    enum CODE
    {
      EXIT,       // １ファイルの実行が終了した
      NEXTFRAME,  // １フレーム進んだ
      INPUTSTATE, // キー入力情報が欲しい
      SCENEFADE,  // 画面フェードアウトして欲しい
      STORAGELOAD,  // セーブデータ読み込んで欲しい
      STORAGESAVE,  // セーブデータを保存して欲しい
    };

    CODE Code;
    std::vector<Maid::String> Param;  //  渡されたパラメータ
  };

  Maid::FUNCTIONRESULT Execute( const Maid::String& SourceCode, const Maid::String& FileName, RETURNCODE& ret );
  Maid::FUNCTIONRESULT WakeupGameEnd( RETURNCODE& ret );
  Maid::FUNCTIONRESULT WakeupInputState( const Maid::Keybord& key, const Maid::Mouse& mou, RETURNCODE& ret );
  Maid::FUNCTIONRESULT WakeupNextFrame( RETURNCODE& ret );
  Maid::FUNCTIONRESULT WakeupSceneFade( RETURNCODE& ret );
  Maid::FUNCTIONRESULT WakeupStorageLoad( Maid::XMLReader& reader, RETURNCODE& ret );
  Maid::FUNCTIONRESULT WakeupStorageSave( RETURNCODE& ret );

  bool IsSuspended() const;

  typedef std::vector<CppDrawObject*> OBJECTLIST;
  struct SCENEINFO
  {
    Maid::POINT3DF CameraEye;
    Maid::POINT3DF CameraTarget;
    Maid::VECTOR3DF CameraUp;
    float CameraFov;
    float CameraAspect;
    float CameraNear;
    float CameraFar;
    float CameraParallaxEye;
    float CameraParallaxTarget;

    Maid::COLOR_R32G32B32A32F AmbientLight;

    Maid::COLOR_R32G32B32A32F DirectionalLightColor;
    Maid::VECTOR3DF  DirectionalLightVec;

    OBJECTLIST ObjectList;
  };

  typedef std::vector<SCENEINFO> SCENEINFOLIST;

  Maid::FUNCTIONRESULT GetSceneInfo( SCENEINFOLIST& list );
  Maid::FUNCTIONRESULT GetStorageData( Maid::XMLWriter& xml );

  void ExitGameLoop();

private:
  Maid::FUNCTIONRESULT Wakeup( RETURNCODE& ret );
  Maid::FUNCTIONRESULT UpdateReturnCode( RETURNCODE& ret );
  void SetupBindObject();
  void ReleaseBindObject();

  SQBool IsRaiseError() const;

  void WriteErrorLog();


  Maid::FUNCTIONRESULT  MakeStorageArray( HSQUIRRELVM v, Maid::XMLReader& reader );
  Maid::FUNCTIONRESULT  MakeStorageTable( HSQUIRRELVM v, Maid::XMLReader& reader );
  Maid::FUNCTIONRESULT  MakeStorageTag( HSQUIRRELVM v, Maid::XMLReader& reader );

  Maid::FUNCTIONRESULT  ReadStorageArray( HSQUIRRELVM v, Maid::XMLWriter& xml );
  Maid::FUNCTIONRESULT  ReadStorageTable( HSQUIRRELVM v, Maid::XMLWriter& xml );

  Maid::FUNCTIONRESULT  ReadCameraData( HSQUIRRELVM v, SCENEINFO& info );
  Maid::FUNCTIONRESULT  ReadDrawObject( HSQUIRRELVM v, SCENEINFO& info );
  Maid::FUNCTIONRESULT  ReadBaseLight( HSQUIRRELVM v, SCENEINFO& info );

  float GetFloat( HSQUIRRELVM v, const SQChar* p );
  int   GetInteger( HSQUIRRELVM v, const SQChar* p );


private:
  HSQUIRRELVM m_SquirrelVM;
  bool m_IsRaiseError;
};

#endif
