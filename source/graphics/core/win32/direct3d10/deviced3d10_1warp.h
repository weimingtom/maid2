/*!	
	@file
	@brief	レンダリングドライバ Direct3D10 Ver.
*/

#ifndef maid2_graphics_core_win32_direct3d10_deviced3d10_1warp_h
#define maid2_graphics_core_win32_direct3d10_deviced3d10_1warp_h

#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include"../../idevice.h"

#include"../../../../framework/win32/window.h"
#include"../../../../framework/win32/dllwrapper.h"
#include"common.h"
#include"deviced3d10_1.h"

namespace Maid { namespace Graphics {

	class DeviceD3D10_1WARP
    : public DeviceD3D10_1
	{
	public:
    DeviceD3D10_1WARP( const DllWrapper& dll, Window& Windw );

  protected:
    virtual FUNCTIONRESULT CreateDeviceAndSwapChain( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter, DXGI_SWAP_CHAIN_DESC& desc, SPD3D10DEVICE& pDevice, SPDXGISWAPCHAIN& pSwapChain );

  private:

	};

}}


#endif
