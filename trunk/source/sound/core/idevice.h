#ifndef maid2_sound_core_idevice_h
#define maid2_sound_core_idevice_h

#include"../../config/define.h"

#include<boost/smart_ptr.hpp>
#include"ibuffer.h"

namespace Maid { namespace Sound {

  struct BUFFERFORMAT
  {
    int Channels; //  チャンネル数
    int SamplesPerSecond;  //  １秒あたりのサンプル数
    int BitPerSamples;    //  １サンプルあたりのビット数
  };

  class IDevice
	{
	public:
    virtual ~IDevice(){}

		virtual void Initialize()=0;
		virtual void Finalize()=0;

    virtual void SetFormat( const BUFFERFORMAT& format )=0;

    virtual SPBUFFER CreateBuffer( const CREATEBUFFERPARAM& param )=0;

  };

  typedef	boost::shared_ptr<IDevice>	SPDEVICE;

}}


#endif
