#ifndef maid2_graphics_font_h
#define maid2_graphics_font_h

#include"../config/define.h"

#include"../auxiliary/globalpointer.h"
#include"core/ifontdevice.h"

#include"surface.h"
#include"graphicscore.h"


namespace Maid
{

  class IFontRasterizeFilter
  {
  public:
    virtual ~IFontRasterizeFilter(){}
    virtual int Convert( SPSURFACE& pData )=0;
  };

  typedef	boost::shared_ptr<IFontRasterizeFilter>	SPFONTRASTERIZEFILTER;
  typedef	std::vector<SPFONTRASTERIZEFILTER>	FONTRASTERIZEFILTER;

  class Font 
    :public GlobalPointer<GraphicsCore>
  {
  public:
    void Create( const SIZE2DI& size, bool IsAntiAlias );
    void Create( const String& Name, const SIZE2DI& size, bool IsAntiAlias );
    void Destroy();

    const String&  GetName()   const;
    const SIZE2DI& GetSize()   const;
    bool IsAntiAlias()const;

    void Rasterize( unt32 code, const FONTRASTERIZEFILTER& Filter, Surface& Dst )const;

  private:
    Graphics::SPFONT m_pFont;
    String  m_FontName;
    SIZE2DI m_FontSize;
    bool    m_IsAntiAlias;
  };


}
#endif
