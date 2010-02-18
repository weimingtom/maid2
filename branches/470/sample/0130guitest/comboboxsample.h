#ifndef sample_0130guitest_comboboxsample_h
#define sample_0130guitest_comboboxsample_h


#include"../../source/config/define.h"
#include"../../source/auxiliary/collision.h"
#include"../../source/graphics/font.h"
#include"../../source/graphics/graphics2drender.h"

#include"../../source/framework/gui/iguicombobox.h"


class ComboBoxElementSample 
  : public Maid::IGUIComboBox::IElement
{
public:
  void Initialize( Maid::Graphics2DRender* r, const Maid::String& Text );

  virtual void UpdateFrame();

  virtual void DrawNormal( const Maid::RenderTargetBase& Target, const Maid::IDepthStencil& Depth, const Maid::POINT2DI& pos );
  virtual void DrawMouseIn( const Maid::RenderTargetBase& Target, const Maid::IDepthStencil& Depth, const Maid::POINT2DI& pos );
  virtual void DrawSelect( const Maid::RenderTargetBase& Target, const Maid::IDepthStencil& Depth, const Maid::POINT2DI& pos );

  virtual bool IsCollision( const Maid::POINT2DI& pos ) const;
  virtual Maid::SIZE2DI GetBoxSize() const;

private:
  Maid::Graphics2DRender*  m_pRender;
  Maid::Font    m_hFont;
  Maid::String  m_Text;
};




class ComboBoxSample 
  : public Maid::IGUIComboBox 
{
public:
   void Initialize( Maid::Graphics2DRender* r );


protected:
  virtual void OnUpdateFrame();
  virtual void OnUpdateDraw( const Maid::RenderTargetBase& Target, const Maid::IDepthStencil& Depth, const Maid::POINT2DI& pos );
  virtual void DrawSlider( const Maid::RenderTargetBase& Target, const Maid::IDepthStencil& Depth, const Maid::POINT2DI& pos );

  virtual bool IsBoxCollision( const Maid::POINT2DI& pos ) const;
  virtual bool IsSelectBoxCollision( const Maid::POINT2DI& pos ) const;
  virtual bool IsSliderCollision( const Maid::POINT2DI& pos ) const;
  virtual bool IsSliderButtonCollision( const Maid::POINT2DI& pos ) const;

private:
  Maid::Graphics2DRender*  m_pRender;
  Maid::Font m_hFont;
};


#endif
