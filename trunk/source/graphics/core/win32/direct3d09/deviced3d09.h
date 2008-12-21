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

 
    virtual SPINPUTLAYOUT CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength );
    virtual SPBUFFER CreateBuffer( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data );
    virtual SPTEXTURE2D CreateTexture2D( const CREATERETEXTURE2DPARAM& param, const SUBRESOURCE* data );

    virtual SPRENDERTARGET CreateRenderTarget( const SPRESOURCE& resource, const CREATERENDERTARGETPARAM& param );
    virtual SPDEPTHSTENCIL CreateDepthStencil( const SPRESOURCE& resource, const CREATEDEPTHSTENCILPARAM& param );
    virtual SPMATERIAL     CreateMaterial( const SPRESOURCE& resource, const CREATESHADERMATERIALPARAM& param );

    virtual SPVERTEXSHADER CreateVertexShader( const void* pData, size_t Length );
    virtual SPPIXELSHADER CreatePixelShader( const void* pData, size_t Length );

    virtual SPSAMPLERSTATE     CreateSamplerState( const SAMPLERSTATEPARAM& Option );
    virtual SPRASTERIZERSTATE  CreateRasterizerState( const RASTERIZERSTATEPARAM& Option );
    virtual SPDEPTHSTENCILSTATE CreateDepthStencilState( const DEPTHSTENCILSTATEPARAM& Option );
    virtual SPBLENDSTATE       CreateBlendState( const BLENDSTATEPARAM& Option );

    virtual SPDRAWCOMMANDEXECUTE GetDrawCommandExecute();
    virtual SPDRAWCOMMANDCAPTURE CreateDrawCommandCapture();

    virtual SPRENDERTARGET GetDefaultRenderTarget()const;
    virtual SPDEPTHSTENCIL GetDefaultDepthStencil()const;


    virtual bool CompileShaderLanguage( const String& Source, std::vector<unt08>& Binary, String& ErrorMessage );

    const SPD3D09DEVICE& GetDevice() const { return m_pDevice; }
    const D3DDEVICE_CREATION_PARAMETERS& GetD3DDEVICE_CREATION_PARAMETERS() const { return m_CreateParams; }
    const D3DCAPS9& GetD3DCAPS9() const { return m_DeviceCaps; }
    const D3DPRESENT_PARAMETERS& GetD3DPRESENT_PARAMETERS() const { return m_CurrentMode; }

	private:
    SPD3D09 GetD3D();

    IBuffer* CreateVertex( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data );
    IBuffer* CreateIndex( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data );
    IBuffer* CreateConstant( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data );


	private:
		Window&	      m_Window;
		SPD3D09DEVICE m_pDevice;		            //!<	レンダリングデバイス

    D3DDEVICE_CREATION_PARAMETERS m_CreateParams;
		D3DCAPS9                      m_DeviceCaps;
    D3DPRESENT_PARAMETERS         m_CurrentMode;

    SCREENMODE  m_ScreenMode;
		LONG      m_WindowModeStyle;
		POINT2DI  m_WindowModePos;

    SPD3D09SURFACE  m_pDefaultDepthStencil;

    SPDRAWCOMMANDEXECUTE  m_pDrawCommandExecute;


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
