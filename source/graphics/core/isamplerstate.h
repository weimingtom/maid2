#ifndef maid2_graphics_core_isamplerstate_h
#define maid2_graphics_core_isamplerstate_h


#include"../../config/define.h"
#include"idevicechild.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

/*
  SamplerState ってのは、Material にあるデータをどうやって取得するかの設定
*/

  struct SAMPLERSTATEPARAM
  {
    enum FILTER
    {
      FILTER_POINT,
      FILTER_LINEAR,
      FILTER_ANISOTROPIC,
    };

    enum ADDRESS
    {
      ADDRESS_WRAP,
      ADDRESS_MIRROR,
      ADDRESS_CLAMP,
      ADDRESS_BORDER,
      ADDRESS_MIRROR_ONCE,
    };



    FILTER  MinFilter;
    FILTER  MagFilter;
    FILTER  MipFilter;

    int   MaxAnisotropy;

    ADDRESS AddressU;
    ADDRESS AddressV;
    ADDRESS AddressW;

    float MipLODBias;
    COMPARISON   ComparisonFunc;
    COLOR_R32G32B32A32F BorderColor;
    float MinLOD;
    float MaxLOD;

    SAMPLERSTATEPARAM()
      :MinFilter(FILTER_POINT)
      ,MagFilter(FILTER_POINT)
      ,MipFilter(FILTER_POINT)
      ,MaxAnisotropy(16)
      ,AddressU(ADDRESS_CLAMP)
      ,AddressV(ADDRESS_CLAMP)
      ,AddressW(ADDRESS_CLAMP)
      ,MipLODBias(0.0f)
      ,ComparisonFunc(COMPARISON_NEVER)
      ,BorderColor(0,0,0,0)
      ,MinLOD(0.0f)
      ,MaxLOD(3.402823466e+38F)
    {
    }
  };


  class ISamplerState : public IDeviceChild
  {
  public:
    ISamplerState( const SAMPLERSTATEPARAM& param ):m_Param(param){}
    const SAMPLERSTATEPARAM& GetParam() const { return m_Param; }

  private:
    SAMPLERSTATEPARAM m_Param;
  };

  typedef	boost::shared_ptr<ISamplerState>	SPSAMPLERSTATE;
}}

#endif