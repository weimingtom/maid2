#ifndef maid2_graphics_core_idepthstencil_h
#define maid2_graphics_core_idepthstencil_h


#include"../../config/define.h"
#include"iview.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  struct CREATEDEPTHSTENCILPARAM
  {
    enum DIMENSION
    {
      DIMENSION_TEXTURE1D,
      DIMENSION_TEXTURE1DARRAY,
      DIMENSION_TEXTURE2D,
      DIMENSION_TEXTURE2DARRAY,
      DIMENSION_TEXTURE2DMS,
      DIMENSION_TEXTURE2DMSARRAY,
    };
    enum FLAG
    {
      FLAG_READ_ONLY_DEPTH   = 0x01,
      FLAG_READ_ONLY_STENCIL = 0x02,
    };

    PIXELFORMAT Format; //  想定させるフォーマット
    DIMENSION   Dimension;
    unt32       ReadOnlyUsage;
    unt32       Param[4];

    CREATEDEPTHSTENCILPARAM()
    {
      ReadOnlyUsage = FLAG_READ_ONLY_DEPTH | FLAG_READ_ONLY_STENCIL;
      ZERO( Param, sizeof(Param) );
    }
  };

  class IDepthStencil : public IView
  {
  public:
    IDepthStencil( const SPRESOURCE& pRes, const CREATEDEPTHSTENCILPARAM& param )
      :IView(pRes), m_Param(param){}

    const CREATEDEPTHSTENCILPARAM& GetParam() const { return m_Param; }
  private:
    CREATEDEPTHSTENCILPARAM m_Param;
  };

  typedef	boost::shared_ptr<IDepthStencil>	SPDEPTHSTENCIL;
}}

#endif