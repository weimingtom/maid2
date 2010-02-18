#ifndef sample_0130guitest_contextmenusample_h
#define sample_0130guitest_contextmenusample_h

#include"../../source/config/define.h"
#include"../../source/auxiliary/collision.h"
#include"../../source/graphics/font.h"
#include"../../source/graphics/graphics2drender.h"

#include"../../source/framework/gui/iguicontextmenu.h"


class ContextMenuChildSample 
  : public Maid::IGUIContextMenu::IElement
{
public:
    void Initialize( Maid::Graphics2DRender* r, const Maid::String& text, const Maid::SIZE2DI& size );

protected:
    virtual void LocalUpdateFrame();
    virtual void LocalUpdateDraw( const Maid::RenderTargetBase& Target, const Maid::IDepthStencil& Depth, const Maid::POINT2DI& pos );
    virtual Maid::SIZE2DI GetSize() const;

private:
  Maid::Graphics2DRender*  m_pRender;
  Maid::SIZE2DI m_Size;
  Maid::Font m_hFont;
  Maid::String m_Text;
};


#endif