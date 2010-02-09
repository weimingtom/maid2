/*!
	@file
	@brief ストレージからデータを読み出してくるクラス
 */

#ifndef maid2_framework_movie_istoragereader_h
#define maid2_framework_movie_istoragereader_h


#include"../../config/define.h"

#include"istoragesample.h"

#include<boost/smart_ptr.hpp>


namespace Maid { namespace Movie { 

  class IStorageReader
  {
  public:
    virtual ~IStorageReader(){}

    virtual void Initialize()=0;
    virtual void Finalize()=0;

    virtual void Read( SPSTORAGESAMPLE& pSample )=0;
    virtual bool IsEnd() const =0;
  };

  typedef boost::shared_ptr<IStorageReader> SPSTORAGEREADER;

}}


#endif