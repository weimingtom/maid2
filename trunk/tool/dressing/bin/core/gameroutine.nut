
class IGameRoutine
{
  _SceneAdmin = null;

  constructor()
  {
  }


  function Initialize()
  {
  }

  function CreateNextScene( exitcode )
  {
  }

  function SetScene( scene )
  {
    _SceneAdmin.SetScene( scene );
  }

  function PushScene( scene )
  {
    _SceneAdmin.PushScene( scene );
  }
  
  function PopScene()
  {
    _SceneAdmin.PopScene( scene );
  }
  
  function OnAppMessage( mess )
  {
  
  }
  
  function BeginSceneFade( time )
  {
    suspend(["scenefade",time]);
  }
}

