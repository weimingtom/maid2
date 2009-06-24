#ifndef maid2_graphics_core_ibuffer_h
#define maid2_graphics_core_ibuffer_h


#include"../../config/define.h"

#include"iresource.h"
#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  struct CREATEBUFFERPARAM
  {
    size_t Length;      //  データの長さ(byte単位)
    RESOURCEUSAGE Usage;
    unt   BindFlags;
    unt   CPUAccessFlags;
    unt   MiscFlags;

    CREATEBUFFERPARAM()
      :Length(0)
      ,Usage(RESOURCEUSAGE_NONE)
      ,BindFlags(0)
      ,CPUAccessFlags(0)
      ,MiscFlags(0)
    {}
  };

  class IBuffer : public IResource
  {
  public:
    IBuffer( const CREATEBUFFERPARAM& param ):IResource(TYPE_BUFFER),m_Param(param){}

    const CREATEBUFFERPARAM& GetParam() const {return m_Param;}
  private:
    CREATEBUFFERPARAM m_Param;
  };

  typedef	boost::shared_ptr<IBuffer>	SPBUFFER;
}}

#endif