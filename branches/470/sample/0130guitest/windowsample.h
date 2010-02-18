#ifndef sample_0130guitest_windowsample_h
#define sample_0130guitest_windowsample_h

#include"../../source/config/define.h"
#include"../../source/auxiliary/collision.h"
#include"../../source/graphics/font.h"
#include"../../source/graphics/graphics2drender.h"

#include"../../source/framework/gui.h"

class WindowSample : public Maid::IGUIWindow
{
public:
  void Initialize( Maid::Graphics2DRender* r );

protected:
    virtual void OnInitialize( ID id, const Maid::IGUIParts& Parent );
    virtual void OnFinalize();

    virtual void OnUpdateFrame();
    virtual void OnUpdateDraw( const Maid::RenderTargetBase& Target, const Maid::IDepthStencil& Depth, const Maid::POINT2DI& pos  );


    virtual bool IsBarCollision( const Maid::POINT2DI& pos ) const;
    virtual bool IsClientCollision( const Maid::POINT2DI& pos ) const;
    virtual bool IsMinimumButtonCollision( const Maid::POINT2DI& pos ) const;
    virtual bool IsHideButtonCollision( const Maid::POINT2DI& pos ) const;
 
    virtual void OnMouseIn( const Maid::POINT2DI& pos );
    virtual void OnMouseOut( const Maid::POINT2DI& pos );

    virtual void OnMoveBegin( const Maid::POINT2DI& pos );
    virtual void OnMoveEnd( const Maid::POINT2DI& pos );
    virtual void OnMinimum( bool IsMin );
    virtual void OnChangeState( const Maid::POINT2DI& pos, STATE state );

private:
  Maid::Graphics2DRender*  m_pRender;
  Maid::Font m_hFont;
};


#endif