#ifndef maid2_graphics_core_iconstant_h
#define maid2_graphics_core_iconstant_h


#include"../../config/define.h"

#include"ibuffer.h"
#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IConstant : public IBuffer
  {
  public:
    IConstant( const CREATEBUFFERPARAM& param ):IBuffer(param){}
  };

  typedef	boost::shared_ptr<IConstant>	SPCONSTANT;
}}

#endif