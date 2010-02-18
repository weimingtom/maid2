#ifndef maid2_graphics_core_iinputlayout_h
#define maid2_graphics_core_iinputlayout_h


#include"../../config/define.h"
#include"idevicechild.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

/*
  InputLayoutってのは、SetVertex() で設定したそれぞれの要素をどう組み合わせて
  １つの頂点にするかって機能。

  たとえば
  SetVertex(0,頂点[] ...);
  SetVertex(1,UV[] ...);
  SetVertex(2,Normal[] ...);

  とかやって InputLayout の設定で１つの頂点をどうするかを決める　

*/
  struct INPUT_ELEMENT
  {
    enum TYPE
    {
      TYPE_FLOAT1,
      TYPE_FLOAT2,
      TYPE_FLOAT3,
      TYPE_FLOAT4,
      TYPE_COLOR,
    };
    enum METHOD {
        METHOD_DEFAULT = 0,
        METHOD_PARTIALU = 1,
        METHOD_PARTIALV = 2,
        METHOD_CROSSUV = 3,
        METHOD_UV = 4,
        METHOD_LOOKUP,
        METHOD_LOOKUPPRESAMPLED
    };

    std::string  SemanticName;
    int    SemanticIndex;
    TYPE   Type;
    int    SlotNo;
    int    Offset;
    METHOD Method;
  };


  class IInputLayout : public IDeviceChild
  {
  public:
  };

  typedef	boost::shared_ptr<IInputLayout>	SPINPUTLAYOUT;
}}

#endif