#ifndef maid2_graphics_core_ibuffer_h
#define maid2_graphics_core_ibuffer_h


#include"../../config/define.h"

#include"iresource.h"
#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IBuffer : public IResource
  {
  public:
    IBuffer():IResource(TYPE_BUFFER){}
  };

  typedef	boost::shared_ptr<IBuffer>	SPBUFFER;
}}

#endif