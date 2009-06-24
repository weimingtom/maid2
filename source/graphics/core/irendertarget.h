#ifndef maid2_graphics_core_irendertarget_h
#define maid2_graphics_core_irendertarget_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"iview.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  struct CREATERENDERTARGETPARAM
  {
    enum DIMENSION
    {
      DIMENSION_BUFFER,
      DIMENSION_TEXTURE1D,
      DIMENSION_TEXTURE1DARRAY,
      DIMENSION_TEXTURE2D,
      DIMENSION_TEXTURE2DARRAY,
      DIMENSION_TEXTURE2DMS,
      DIMENSION_TEXTURE2DMSARRAY,
      DIMENSION_TEXTURE3D,
      DIMENSION_TEXTURECUBE,
      DIMENSION_TEXTURECUBEARRAY,
    };

    PIXELFORMAT Format; //  想定させるフォーマット
    DIMENSION   Dimension;
    unt32       Param[4];

    CREATERENDERTARGETPARAM()
    {
      ZERO( Param, sizeof(Param) );
    }
  };

  class IRenderTarget : public IView
  {
  public:
    IRenderTarget( const SPRESOURCE& pRes, const CREATERENDERTARGETPARAM& param )
      :IView(pRes), m_Param(param){}

    const CREATERENDERTARGETPARAM& GetParam() const { return m_Param; }
  private:
    CREATERENDERTARGETPARAM m_Param;
  };

  typedef	boost::shared_ptr<IRenderTarget>	SPRENDERTARGET;
}}

#endif