#ifndef maid2_graphics_core_ivertexshader_h
#define maid2_graphics_core_ivertexshader_h


#include"../../config/define.h"
#include"idevicechild.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IVertexShader : public IDeviceChild
  {
  public:
  };

  typedef	boost::shared_ptr<IVertexShader>	SPVERTEXSHADER;
}}

#endif