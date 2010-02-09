#ifndef maid2_framework_core_istoragesample_h
#define maid2_framework_core_istoragesample_h


#include"../../../config/define.h"
#include"../../../config/typedef.h"

#include <boost/smart_ptr.hpp>



namespace Maid { namespace Movie { 

  class IStorageSample
  {
  public:
    virtual ~IStorageSample(){}
    virtual unt GetSize()=0;
  };

  typedef boost::shared_ptr<IStorageSample> SPSTORAGESAMPLE;

}}

#endif
