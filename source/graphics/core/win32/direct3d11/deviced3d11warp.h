/*!	
	@file
	@brief	レンダリングドライバ Direct3D11 Ver.
*/

#ifndef maid2_graphics_core_win32_direct3d11_deviced3d11warp_h
#define maid2_graphics_core_win32_direct3d11_deviced3d11warp_h

#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include"../../idevice.h"

#include"../../../../framework/win32/window.h"
#include"../../../../framework/win32/dllwrapper.h"
#include"common.h"
#include"deviced3d11.h"

namespace Maid { namespace Graphics {

	class DeviceD3D11WARP
    : public DeviceD3D11
	{
	public:
    DeviceD3D11WARP( const DllWrapper& dll, const SPDXGIFACTORY& pFactory, Window& Windw );

  protected:
    virtual SPD3D11DEVICE CreateDevice( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter );

	};

}}


#endif
