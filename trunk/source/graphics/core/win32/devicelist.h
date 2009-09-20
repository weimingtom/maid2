/*!
	@file
	@brief Win32用 VideoCard列挙ヘッダ
 */

#ifndef graphic_core_win32_devicelist_h
#define graphic_core_win32_devicelist_h


#include"../../../config/define.h"
#include"../../../config/win32.h"

#include"../idevicelist.h"

#include"../../../framework/win32/dllwrapper.h"
#include"../../../framework/win32/com_ptr.h"
#include"../../../framework/win32/window.h"

#include"direct3d09/common.h"
#include"direct3d10/common.h"
#include"dxgitypedef.h"

namespace Maid { namespace Graphics {


  class DeviceList : public IDeviceList
  {
  public:
    DeviceList( Window& window );
    void Initialize();

    void GetList( std::vector<INFO>& info );
    SPDEVICE Create( int DeviceID );
    SPFONTDEVICE CreateFontDevice();

  private:
    void InitializeD3D09();
    void InitializeDXGI();
    void InitializeD3D10();
    void InitializeD3D11();

    void FindAdapterD3D09( std::vector<INFO>& info );
    void FindAdapterD3D10( std::vector<INFO>& info );
    void FindAdapterD3D11( std::vector<INFO>& info );

  private:
    Window&  m_Window;

    bool m_IsD3D09;
    bool m_IsDXGI;
    bool m_IsD3D10;
    bool m_IsD3D10_1;
    bool m_IsD3D10_WARP;
    bool m_IsD3D11;

  private:
    DllWrapper   m_D3D9DLL;
    SPD3D09      m_pDirect3D09;


    DllWrapper     m_DXGIDLL;
    SPDXGIFACTORY  m_pDXGIFactory;

    DllWrapper     m_D3D10DLL;
    DllWrapper     m_D3D10_1DLL;
    DllWrapper     m_D3D10_WARPDLL;

    DllWrapper     m_D3D11DLL;
  };
}}



#endif
