#ifndef maid2_graphics_core_imaterial_h
#define maid2_graphics_core_imaterial_h


#include"../../config/define.h"
#include"iview.h"
#include"createparam.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

/*
  Materialってのは、各シェーダーで使える素材。

*/
  struct CREATEMATERIALPARAM
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
      DIMENSION_BUFFEREX,
    };

    PIXELFORMAT Format;   //  想定させるフォーマット
    DIMENSION   Dimension;//  想定させる状態
    unt32       Param[4]; //  Dimensionに応じたパラメータ
  };

  class IMaterial : public IView
  {
  public:
    IMaterial( const SPRESOURCE& pRes, const CREATEMATERIALPARAM& param ):IView(pRes),m_Param(param){}
    const CREATEMATERIALPARAM& GetParam() const { return m_Param; }

  private:
    CREATEMATERIALPARAM m_Param;
  };

  typedef	boost::shared_ptr<IMaterial>	SPMATERIAL;
}}

#endif