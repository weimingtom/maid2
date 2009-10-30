/*!	
	@file
	@brief	レンダリングドライバ Direct3D10 Ver.
*/

#ifndef maid2_graphics_core_win32_direct3d10_deviced3d10_1_h
#define maid2_graphics_core_win32_direct3d10_deviced3d10_1_h

#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include"../../idevice.h"

#include"../../../../framework/win32/window.h"
#include"../../../../framework/win32/dllwrapper.h"
#include"common.h"
#include"deviced3d10_0.h"

namespace Maid { namespace Graphics {

	class DeviceD3D10_1 
    : public DeviceD3D10_0
	{
	public:
    DeviceD3D10_1( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter, Window& Windw );
    virtual SPMATERIAL     CreateMaterial( const SPRESOURCE& resource, const CREATEMATERIALPARAM* param );
    virtual SPBLENDSTATE   CreateBlendState( const BLENDSTATEPARAM& Option );

  protected:
    virtual FUNCTIONRESULT CreateDeviceAndSwapChain( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter, DXGI_SWAP_CHAIN_DESC& desc, SPD3D10DEVICE& pDevice, SPDXGISWAPCHAIN& pSwapChain );
	};

}}


#endif
