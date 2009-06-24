#ifndef maid2_graphics_core_itexture2d_h
#define maid2_graphics_core_itexture2d_h


#include"../../config/define.h"
#include"../../config/typedef.h"

#include"iresource.h"
#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  struct CREATERETEXTURE2DPARAM
  {
    SIZE2DI Size;
    int MipLevels;
    int ArraySize;      //  何枚あるか？
    PIXELFORMAT Format; //  ピクセルフォーマット
    SAMPLEDESC  Sample;
    RESOURCEUSAGE Usage;
    unt   BindFlags;
    unt   CPUAccessFlags;
    unt   MiscFlags;

    CREATERETEXTURE2DPARAM()
      :Size(0,0)
      ,MipLevels(0)
      ,ArraySize(0)
      ,Format(PIXELFORMAT_NONE)
      ,Usage(RESOURCEUSAGE_DEFAULT)
      ,BindFlags(0)
      ,CPUAccessFlags(0)
      ,MiscFlags(0)
    {}
  };

  class ITexture2D : public IResource
  {
  public:
    ITexture2D( const CREATERETEXTURE2DPARAM& param )
      :IResource(TYPE_TEXTURE2D),m_Param(param){}

    const CREATERETEXTURE2DPARAM& GetParam() const { return m_Param; }

  private:
    CREATERETEXTURE2DPARAM  m_Param;
  };

  typedef	boost::shared_ptr<ITexture2D>	SPTEXTURE2D;
}}

#endif