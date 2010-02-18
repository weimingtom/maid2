#ifndef maid2_graphics_core_idevicechild_h
#define maid2_graphics_core_idevicechild_h


#include"../../config/define.h"

#include"createparam.h"
#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IDeviceChild
  {
  public:
    virtual ~IDeviceChild(){}
  };

  typedef	boost::shared_ptr<IDeviceChild>	SPDEVICECHILD;
}}

#endif