#ifndef maid2_graphics_core_idepthstencilstate_h
#define maid2_graphics_core_idepthstencilstate_h


#include"../../config/define.h"
#include"idevicechild.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IDepthStencilState : public IDeviceChild
  {
  public:
  };

  typedef	boost::shared_ptr<IDepthStencilState>	SPDEPTHSTENCILSTATE;
}}

#endif