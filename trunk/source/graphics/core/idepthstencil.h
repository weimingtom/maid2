#ifndef maid2_graphics_core_idepthstencil_h
#define maid2_graphics_core_idepthstencil_h


#include"../../config/define.h"
#include"iview.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IDepthStencil : public IView
  {
  public:
  };

  typedef	boost::shared_ptr<IDepthStencil>	SPDEPTHSTENCIL;
}}

#endif