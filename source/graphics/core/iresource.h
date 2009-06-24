#ifndef maid2_graphics_core_iresource_h
#define maid2_graphics_core_iresource_h


#include"../../config/define.h"
#include"idevicechild.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class IResource : public IDeviceChild
  {
  public:
    enum TYPE
    {
      TYPE_BUFFER,
      TYPE_TEXTURE2D,
    };
    IResource( TYPE t ):m_Type(t){}

    TYPE GetType() const { return m_Type; }
  private:
    TYPE  m_Type;
  };

  typedef	boost::shared_ptr<IResource>	SPRESOURCE;

}}

#endif