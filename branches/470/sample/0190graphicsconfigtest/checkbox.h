#ifndef sample_0190graphicsconfigtest_checkbox_h
#define sample_0190graphicsconfigtest_checkbox_h

#include"../../source/config/define.h"
#include"../../source/framework/gui.h"
#include"../../source/auxiliary/collision.h"
#include"../../source/graphics/font.h"
#include"../../source/graphics/graphics2drender.h"



class CheckBox : public Maid::IGUICheckBox
{
public:
  void Initialize( Maid::Graphics2DRender* r, const Maid::String& Text, const Maid::SIZE2DI& size );

protected:
    virtual void OnInitialize( ID id, const IGUIParts& Parent );
    virtual void OnFinalize();
    virtual bool LocalIsCollision( const Maid::POINT2DI& pos )const;
    virtual void OnUpdateFrame();
    virtual void OnUpdateDraw( const Maid::RenderTargetBase& Target, const Maid::IDepthStencil& Depth, const Maid::POINT2DI& pos  );
    virtual void OnMouseMove( const Maid::POINT2DI& pos );
    virtual void OnMouseIn( const Maid::POINT2DI& pos );
    virtual void OnMouseOut( const Maid::POINT2DI& pos );
    virtual void OnStateChange( bool IsCheck );

private:
  Maid::Graphics2DRender*  m_pRender;
  Maid::SIZE2DI m_Size;
  Maid::Font m_hFont;
  Maid::String  m_Text;
};


#endif