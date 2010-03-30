#include"stdafx.h"
#include"app.h"

using namespace Maid;




struct DRAWLIST
{
  POINT3DF Pos;
  CppDrawObject* pObject;

  DRAWLIST()
  {}
  DRAWLIST( CppDrawObject* p )
    :Pos(p->CalcGlobalPos()), pObject(p)
  {}
};

class DRAWLISTLess {
public:
    bool operator()(const DRAWLIST& lhs, const DRAWLIST& rhs) const {
      return lhs.Pos.z > rhs.Pos.z;
    }
};

void MyApp::GameDraw()
{
  SquirrelWrapper::SCENEINFOLIST SceneInfoList;
  const FUNCTIONRESULT ret = m_Squirrel.GetSceneInfo( SceneInfoList );
  if( FUNCTIONRESULT_FAILE(ret) ) { return ; }

  for( int i=0; i<(int)SceneInfoList.size(); ++i )
  {
    const SquirrelWrapper::SCENEINFO& scene = SceneInfoList[i];
    const SquirrelWrapper::OBJECTLIST& ObjList = scene.ObjectList;

    //  取得したリストをカメラから遠いい順にソートする
    //  今現在カメラを作っていないので、Zの大きい順にソート

    std::vector<DRAWLIST> dat;
    for( int j=0; j<(int)ObjList.size(); ++j )
    {
      CppDrawObject* pObj = ObjList[j];
      dat.push_back( DRAWLIST(pObj) );
    }

    std::sort( dat.begin(), dat.end(), DRAWLISTLess() );

    VECTOR3DF OffsetEye(0,0,0);
    VECTOR3DF OffsetTarget(0,0,0);
    if( m_ParallaxInfo.Start )
    {
      const float r = m_ParallaxInfo.BltType? DEGtoRAD(90.0f) : DEGtoRAD(-90.0f);

      const VECTOR3DF& u = scene.CameraUp;
      VECTOR3DF v;
      v.x = u.x * Math<float>::cos(r) - u.y * Math<float>::sin(r);
      v.y = u.x * Math<float>::sin(r) + u.y * Math<float>::cos(r);
      v.z = 0.0f;

      OffsetEye = v * scene.CameraParallaxEye * m_ParallaxInfo.Eye;
      OffsetTarget = v * scene.CameraParallaxTarget * m_ParallaxInfo.Target;
    }

    Camera camera;
    camera.SetPerspective( scene.CameraFov, scene.CameraAspect, scene.CameraNear, scene.CameraFar );
    camera.SetPosition( scene.CameraEye + OffsetEye );
    camera.SetTarget  ( scene.CameraTarget + OffsetTarget );
    camera.SetUpVector( scene.CameraUp );
  
    GraphicsCommandControl& Command = m_Command;
    std::vector<LIGHT> LightList;
    {
      LIGHT l;
      l.Type = LIGHT::DIRECTIONAL;
      l.Direction = scene.DirectionalLightVec;
      l.Diffuse   = scene.DirectionalLightColor;

      LightList.push_back( l );
    }

    Command.ClearDepth(); //  シーンごとにZをクリア

    m_3DRender.SetCamera( camera );
    m_3DRender.SetLight( LightList );
    m_3DRender.SetAmbient( scene.AmbientLight );
    for( int j=0; j<(int)dat.size(); ++j )
    {
      DRAWLIST& obj = dat[j];

      const float time = obj.pObject->time;
      const POINT3DF& pos = obj.Pos;
      obj.pObject->GetType()->Draw( time, pos );
    }
  }

  if( m_ParallaxInfo.Start )
  {
    const COLOR_R32G32B32A32F black(0,0,0,1);
    const COLOR_R32G32B32A32F white(1,1,1,1);
    const COLOR_R32G32B32A32F lc = m_ParallaxInfo.BltType? black : white;
    const COLOR_R32G32B32A32F rc = m_ParallaxInfo.BltType? white : black;

    for( int i=0; i<(int)m_ParallaxInfo.Left.size(); ++i )
    {
      const RECT2DI& rect = m_ParallaxInfo.Left[i];
      m_2DRender.Fill( rect.GetPoint(), lc, rect.GetSize(), POINT2DI(0,0) );
    }
    for( int i=0; i<(int)m_ParallaxInfo.Right.size(); ++i )
    {
      const RECT2DI& rect = m_ParallaxInfo.Right[i];
      m_2DRender.Fill( rect.GetPoint(), rc, rect.GetSize(), POINT2DI(0,0) );
    }

    m_ParallaxInfo.BltType = !m_ParallaxInfo.BltType;
  }
}

void MyApp::BeginSceneFade()
{
  DepthStencil  dp;

  m_FadeAlpha.Set( 1.0f, 0.0f, 120 );
  m_ScreenShot.Create( SIZE2DI(800,600), PIXELFORMAT_R08G08B08I );

  dp.CreateCompatible( m_ScreenShot, PIXELFORMAT_D32I );

  GraphicsCommandControl& Command = m_Command;

  Command.SetRenderTarget( m_ScreenShot, dp );
  Command.ClearRenderTarget();
  Command.ClearDepth();
  Command.Begin();

  GameDraw();
  Command.End();

 }
