/*!	
	@file
	@brief	レンダリングドライバ Direct3D9 Ver.
*/

#ifndef maid2_graphics_core_win32_direct3d09_deviced3d09_h
#define maid2_graphics_core_win32_direct3d09_deviced3d09_h

#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include<vector>

#include"../../idevice.h"

#include"../../../../framework/win32/window.h"
#include"../../../../auxiliary/thread/win32/threadmutex.h"
#include"common.h"

namespace Maid { namespace Graphics {

	class DeviceD3D09 : public IDevice
	{
	public:
    DeviceD3D09( IDirect3DDevice9* pDev, Window& Windw );

    void Initialize();
    void Finalize();
    virtual void SerchDisplayMode( std::vector<DISPLAYMODE>& mode );
    virtual void SerchEnableFormat( ENABLEFORMAT& caps );
    virtual void SetScreenMode( const SCREENMODE& mode );
    virtual void Present();
    virtual SCREENMODE GetScreenMode();

    virtual IInputLayout* CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )=0;
    virtual IBuffer* CreateBuffer( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data )=0;
    virtual ITexture2D* CreateTexture2D( const CREATERETEXTURE2DPARAM& param, const SUBRESOURCE* data )=0;


    virtual IRenderTarget* CreateRenderTarget( const IResource* resource, const CREATERENDERTARGETPARAM& param )=0;
    virtual IDepthStencil* CreateDepthStencil( const IResource* resource, const CREATEDEPTHSTENCILPARAM& param )=0;
    virtual IMaterial*     CreateMaterial( const IResource* resource, const CREATESHADERMATERIALPARAM& param )=0;

    virtual IVertexShader* CreateVertexShader( const void* pData, size_t Length )=0;
    virtual IPixelShader* CreatePixelShader( const void* pData, size_t Length )=0;

    virtual ISamplerState*     CreateSamplerState( const SAMPLERSTATEPARAM& Option )=0;
    virtual IRasterizerState*  CreateRasterizerState( const RASTERIZERSTATEPARAM& Option )=0;
    virtual IBlendState*       CreateBlendState( const BLENDSTATEPARAM& Option )=0;

    virtual IDrawCommandExecute* CreateDrawCommandExecute()=0;
    virtual IDrawCommandCapture* CreateDrawCommandCapture()=0;

    virtual IRenderTarget* GetDefaultRenderTarget()=0;
    virtual IDepthStencil* GetDefaultDepthStencil()=0;

    virtual bool CompileShaderLanguage( const String& Source, std::vector<unt08>& Binary, String& ErrorMessage )=0;




    const SPD3D09DEVICE& GetDevice() const { return m_pDevice; }
    const D3DDEVICE_CREATION_PARAMETERS& GetD3DDEVICE_CREATION_PARAMETERS() const { return m_CreateParams; }
    const D3DCAPS9& GetD3DCAPS9() const { return m_DeviceCaps; }
    const D3DPRESENT_PARAMETERS& GetD3DPRESENT_PARAMETERS() const { return m_CurrentMode; }

	private:
    SPD3D09 GetD3D();


	private:
		Window&	      m_Window;
		SPD3D09DEVICE m_pDevice;		            //!<	レンダリングデバイス

    D3DDEVICE_CREATION_PARAMETERS m_CreateParams;
		D3DCAPS9                      m_DeviceCaps;
    D3DPRESENT_PARAMETERS         m_CurrentMode;

    SCREENMODE  m_ScreenMode;
		LONG      m_WindowModeStyle;
		POINT2DI  m_WindowModePos;

    SPD3D09SURFACE  m_pDefaultRenderTarget;
    SPD3D09SURFACE  m_pDefaultDepthStencil;


	private:
    static BOOL CALLBACK EnumWindowsProc(HWND hWnd , LPARAM lp);

    struct OTHERWINDOWINFO
    {
	    HWND    hWnd;
	    RECT2DI	Rect;
    };
    std::vector<OTHERWINDOWINFO>	s_WindowInfoList;
	};

}}


#endif
