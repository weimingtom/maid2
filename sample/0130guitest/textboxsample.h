#ifndef sample_0130guitest_textboxsample_h
#define sample_0130guitest_textboxsample_h

#include"../../source/config/define.h"
#include"../../source/auxiliary/collision.h"
#include"../../source/auxiliary/mathematics.h"
#include"../../source/auxiliary/string.h"
#include"../../source/graphics/font.h"
#include"../../source/graphics/graphicsrender.h"

#include"../../source/framework/gui.h"




class TextBoxSample : public Maid::IGUITextBox
{
public:
  TextBoxSample();

  void Initialize( Maid::GraphicsRender* r );

protected:
  virtual void OnInitialize( ID id, const IGUIParts& Parent );
  virtual void OnFinalize();
  virtual bool LocalIsCollision( const Maid::POINT2DI& pos ) const;

  virtual void OnUpdateFrame();
  virtual void OnUpdateDraw( const Maid::RenderTargetBase& Target, const Maid::IDepthStencil& Depth, const Maid::POINT2DI& pos  );

private:
  Maid::GraphicsRender*  m_pRender;
  Maid::Font  m_hFont;  

};

#endif