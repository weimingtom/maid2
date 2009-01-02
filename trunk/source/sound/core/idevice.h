#ifndef maid2_sound_core_idevice_h
#define maid2_sound_core_idevice_h

#include"../../config/define.h"

#include"common.h"
#include"ibuffer.h"
#include<boost/smart_ptr.hpp>

namespace Maid { namespace Sound {


  class IDevice
	{
	public:
    virtual ~IDevice(){}

		virtual void Initialize()=0;
		virtual void Finalize()=0;

    virtual void SetFormat( const PCMFORMAT& format )=0;

    virtual SPBUFFER CreateBuffer( const CREATEBUFFERPARAM& param )=0;
    virtual SPBUFFER DuplicateBuffer( const SPBUFFER& pSrc )=0;

  };

  typedef	boost::shared_ptr<IDevice>	SPDEVICE;

}}


#endif
