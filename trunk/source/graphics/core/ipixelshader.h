#ifndef maid2_graphics_core_ipixelshader_h
#define maid2_graphics_core_ipixelshader_h


#include"../../config/define.h"
#include"idevicechild.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IPixelShader : public IDeviceChild
  {
  public:
  };

  typedef	boost::shared_ptr<IPixelShader>	SPPIXELSHADER;
}}

#endif