/*
  テキスト表示サンプル
*/

#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics2drender.h"
#include"../../source/graphics/graphicscommandcontrol.h"

#include"../../source/graphics/font.h"
#include"../../source/graphics/transiter.h"

#include"startupdialog.h"


using namespace Maid;



//  ラスタライズしたフォントの縁取りを行うフィルタ

class Fuchi
  :public  IFontRasterizeFilter
{
public:
  virtual int Convert( SPSURFACEINSTANCE& pData )
  {
    SPSURFACEINSTANCE pRet( new SurfaceInstance );

    pRet->Create( pData->GetSize(), pData->GetPixelFormat() );

    Transiter::Copy( *pData, *pRet );

    const SIZE2DI size = pData->GetSize();

    const COLOR_R32G32B32A32F c(1,0,0,1);
    for( int y=0; y<size.h; ++y )
    {
      for( int x=0; x<size.w; ++x )
      {
        const COLOR_R32G32B32A32F ref = pData->GetPixel(POINT2DI(x,y));

        if( ref.GetA()<=0.0f ) { continue; }

        Set( pData, pRet, POINT2DI(x,y), c );
      }
    }

    pData = pRet;
    return 0;
  }

  void Set( const SPSURFACEINSTANCE& pSrc, SPSURFACEINSTANCE& pDst, const POINT2DI& pos, const COLOR_R32G32B32A32F& c )
  {
    const SIZE2DI size = pSrc->GetSize();

    if( 0<pos.y )
    {
      if( 0<pos.x )
      {
        const COLOR_R32G32B32A32F ref = pSrc->GetPixel(POINT2DI(pos.x-1,pos.y-1));
        if( ref.GetA()<=0.0f ) { pDst->SetPixel( POINT2DI(pos.x-1,pos.y+1), c ); }
      }
      if( true )
      {
        const COLOR_R32G32B32A32F ref = pSrc->GetPixel(POINT2DI(pos.x,pos.y-1));
        if( ref.GetA()<=0.0f ) { pDst->SetPixel( POINT2DI(pos.x,pos.y+1), c ); }
      }
      if( pos.x<size.w-1 )
      {
        const COLOR_R32G32B32A32F ref = pSrc->GetPixel(POINT2DI(pos.x+1,pos.y-1));
        if( ref.GetA()<=0.0f ) { pDst->SetPixel( POINT2DI(pos.x+1,pos.y+1), c ); }
      }
    }
    if( true )
    {
      if( 0<pos.x )
      {
        const COLOR_R32G32B32A32F ref = pSrc->GetPixel(POINT2DI(pos.x-1,pos.y));
        if( ref.GetA()<=0.0f ) { pDst->SetPixel( POINT2DI(pos.x-1,pos.y+1), c ); }
      }/*
      if( true )
      {
        const COLOR_R32G32B32A32F ref = pSrc->GetPixel(POINT2DI(pos.x,pos.y));
        if( ref.GetA()<=0.0f ) { pDst->SetPixel( POINT2DI(pos.x,pos.y+1), c ); }
      }*/
      if( pos.x<size.w-1 )
      {
        const COLOR_R32G32B32A32F ref = pSrc->GetPixel(POINT2DI(pos.x+1,pos.y));
        if( ref.GetA()<=0.0f ) { pDst->SetPixel( POINT2DI(pos.x+1,pos.y+1), c ); }
      }
    }
    if( pos.y<size.h-1 )
    {
      if( 0<pos.x )
      {
        const COLOR_R32G32B32A32F ref = pSrc->GetPixel(POINT2DI(pos.x-1,pos.y+1));
        if( ref.GetA()<=0.0f ) { pDst->SetPixel( POINT2DI(pos.x-1,pos.y+1), c ); }
      }
      if( true )
      {
        const COLOR_R32G32B32A32F ref = pSrc->GetPixel(POINT2DI(pos.x,pos.y+1));
        if( ref.GetA()<=0.0f ) { pDst->SetPixel( POINT2DI(pos.x,pos.y+1), c ); }
      }
      if( pos.x<size.w-1 )
      {
        const COLOR_R32G32B32A32F ref = pSrc->GetPixel(POINT2DI(pos.x+1,pos.y+1));
        if( ref.GetA()<=0.0f ) { pDst->SetPixel( POINT2DI(pos.x+1,pos.y+1), c ); }
      }
    }
  }
};



class MyApp : public IGameThread
{
public:
  MyApp()
    :m_Render(m_Command)
  {
  }

protected:
  bool SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
  {
    const RETURNCODE ret = BeginStartupInfo( DeviceList, conf );
    return ret==GAMESTART;
  }

  void Initialize()
  {
    m_Command.Initialize();
    m_Render.Initialize();

    {
      FONTRASTERIZEFILTERLIST List;

      List.push_back( SPFONTRASTERIZEFILTER( new Fuchi ) );

      Font::SetFilter( 0, List );
    }


    m_FontS.Create( SIZE2DI( 8,16), false );
    m_FontM.Create( SIZE2DI(16,32), true );
    m_FontM2.Create( SIZE2DI(16,32), true, COLOR_R32G32B32A32F(1,0,0,1) );

    m_FontL.Create( SIZE2DI(32,64), true, COLOR_R32G32B32A32F(0,1,0,1), 0  );



    m_FrameCount = 0;
    m_DrawCount  = 0;
  }

  void UpdateFrame()
  {
    //  フレーム毎に呼ばれるとこ
    //  終了する場合は OnExitGame() を呼ぶ
    if( m_Render.IsInitializing() ) { return ; }

    m_Render.UpdateFrame();
    ++m_FrameCount;
  }

  void UpdateDraw()
  {
    //  定期的に描画するとこ

    m_Command.ClearRenderTarget( COLOR_A32B32G32R32F(1,0,0,0) );

    if( m_Render.IsInitializing() ) { return ; }

    m_Command.Begin();
    {
      char buf[256];
      sprintf( buf, "frame:%04d,draw:%04d", m_FrameCount, m_DrawCount );

      m_Render.BltText( POINT2DI(0,  0), m_FontS, String::ConvertSJIStoMAID(buf), COLOR_R32G32B32A32F(1,1,1,1) );
      m_Render.BltText( POINT2DI(0,100), m_FontS, MAIDTEXT("テキスト表示"), COLOR_R32G32B32A32F(1,1,1,1) );
      m_Render.BltText( POINT2DI(0,200), m_FontM2, MAIDTEXT("色かえもＯＫ"), COLOR_R32G32B32A32F(1,1,1,1) );
      m_Render.BltText( POINT2DI(0,300), m_FontM2, MAIDTEXT("半透明もできる"), COLOR_R32G32B32A32F(1,1,1,0.5f) );
      m_Render.BltText( POINT2DI(0,400), m_FontL, MAIDTEXT("改行も\nサポート"), COLOR_R32G32B32A32F(1,1,1,1) );
    }
    m_Command.End();
    ++m_DrawCount;
  }

  void Finalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  GraphicsCommandControl  m_Command;
  Graphics2DRender  m_Render;
  Font          m_FontS;
  Font          m_FontM;
  Font          m_FontM2;
  Font          m_FontL;

  int   m_FrameCount;
  int   m_DrawCount;
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

