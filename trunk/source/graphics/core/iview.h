#ifndef maid2_graphics_core_iview_h
#define maid2_graphics_core_iview_h


#include"../../config/define.h"
#include"idevicechild.h"
#include"iresource.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IView : public IDeviceChild
  {
  public:
    IView( const SPRESOURCE& pRes ){}

    const SPRESOURCE& GetResource() { return m_pResource; }

  private:
    SPRESOURCE m_pResource;
  };

  typedef	boost::shared_ptr<IView>	SPVIEW;
}}

#endif