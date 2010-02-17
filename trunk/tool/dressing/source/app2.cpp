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
  const FUNCTIONRESULT ret = m_Squirrel.GetDrawObjectList( SceneInfoList );
  if( FUNCTIONRESULT_FAILE(ret) ) { return ; }

  for( int i=0; i<(int)SceneInfoList.size(); ++i )
  {
    const SquirrelWrapper::OBJECTLIST& ObjList = SceneInfoList[i].ObjectList;

    //  取得したリストをカメラから遠いい順にソートする
    //  今現在カメラを作っていないので、Zの大きい順にソート

    std::vector<DRAWLIST> dat;
    for( int j=0; j<(int)ObjList.size(); ++j )
    {
      CppDrawObject* pObj = ObjList[j];
      dat.push_back( DRAWLIST(pObj) );
    }

    std::sort( dat.begin(), dat.end(), DRAWLISTLess() );

    for( int j=0; j<(int)dat.size(); ++j )
    {
      DRAWLIST& obj = dat[j];

      const float time = obj.pObject->time;
      const POINT3DF& pos = obj.Pos;
      obj.pObject->GetType()->Draw( time, pos );
    }
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
  Command.ClearDepthStencil();
  Command.Begin();

  GameDraw();
  Command.End();

 }
