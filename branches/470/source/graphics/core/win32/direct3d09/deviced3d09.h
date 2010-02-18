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
#include"../../../../framework/win32/dllwrapper.h"
#include"../../../../auxiliary/thread/win32/threadmutex.h"
#include"common.h"

namespace Maid { namespace Graphics {

	class DeviceD3D09 
    : public IDevice
	{
	public:
    DeviceD3D09( const SPD3D09& pD3D, UINT adapter, bool IsSoftwareVertexProcessing, Window& Windw );

    virtual void Initialize();
    virtual void Finalize();
    virtual void SetFullScreenState( bool IsFullScreen );
    virtual bool IsFullScreen() const;
    virtual float GetTexelMapValue() const;
    virtual void SerchSwapChainSurfaceFormat( std::vector<PIXELFORMAT>& mode )  const;
    virtual void SetSwapChainFormat( const SWAPCHAINFORMAT& mode );

    virtual void SerchEnableFormat( ENABLEFORMAT& caps ) const;
    virtual void Present();
    virtual SWAPCHAINFORMAT GetSwapChainFormat() const;
    virtual SIZE2DI GetDisplayAspect() const;

 
    virtual SPINPUTLAYOUT CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength );



    virtual SPBUFFER CreateBuffer( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data );
    virtual SPTEXTURE2D CreateTexture2D( const CREATERETEXTURE2DPARAM& param, const SUBRESOURCE* data );

    virtual SPRENDERTARGET CreateRenderTarget( const SPRESOURCE& resource, const CREATERENDERTARGETPARAM* param );
    virtual SPDEPTHSTENCIL CreateDepthStencil( const SPRESOURCE& resource, const CREATEDEPTHSTENCILPARAM* param );
    virtual SPMATERIAL     CreateMaterial( const SPRESOURCE& resource, const CREATEMATERIALPARAM* param );

    virtual SPVERTEXSHADER CreateVertexShader( const void* pData, size_t Length );
    virtual SPPIXELSHADER CreatePixelShader( const void* pData, size_t Length );

    virtual SPSAMPLERSTATE     CreateSamplerState( const SAMPLERSTATEPARAM& Option );
    virtual SPRASTERIZERSTATE  CreateRasterizerState( const RASTERIZERSTATEPARAM& Option );
    virtual SPDEPTHSTENCILSTATE CreateDepthStencilState( const DEPTHSTENCILSTATEPARAM& Option );
    virtual SPBLENDSTATE       CreateBlendState( const BLENDSTATEPARAM& Option );

    virtual SPDRAWCOMMANDEXECUTE GetDrawCommandExecute()const;
    virtual SPDRAWCOMMANDCAPTURE CreateDrawCommandCapture();

    virtual SPRENDERTARGET GetCurrentRenderTarget()const;

    virtual bool CompileShaderLanguage( const String& Source, CODETYPE type, std::vector<unt08>& Binary, String& ErrorMessage );

    const SPD3D09DEVICE& GetDevice() const { return m_pDevice; }
    const D3DDEVICE_CREATION_PARAMETERS& GetD3DDEVICE_CREATION_PARAMETERS() const { return m_CreateParams; }
    const D3DCAPS9& GetD3DCAPS9() const { return m_DeviceCaps; }

	private:
    const SPD3D09& GetD3D()const;

    void ScreenReset( const D3DPRESENT_PARAMETERS& param );

    IBuffer* CreateVertex( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data );
    IBuffer* CreateIndex( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data );
    IBuffer* CreateConstant( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data );

    SPTEXTURE2D CreateRenderTargetTexture( const CREATERETEXTURE2DPARAM& param );
    SPTEXTURE2D CreateRenderTarget( const CREATERETEXTURE2DPARAM& param );
    SPTEXTURE2D CreateDepthStencil( const CREATERETEXTURE2DPARAM& param );
    SPTEXTURE2D CreateTexture( const CREATERETEXTURE2DPARAM& param, const SUBRESOURCE* data );

    void CreateDefaultVertexShader( int no, std::vector<unt08>& Binary );
    void CreateDefaultPixelShader( int no, std::vector<unt08>& Binary );

    D3DPRESENT_PARAMETERS GetD3DPRESENT_PARAMETERS() const;
    D3DDISPLAYMODE FindScreenMode( const D3DDISPLAYMODE& base );

    void SetSwapChainFormat( bool IsFullScreen, const SWAPCHAINFORMAT& mode );

	private:
    const UINT    m_Adapter;
    const SPD3D09 m_pDirect3D09;
    const bool    m_IsSoftwareVertexProcessing;
		Window&	      m_Window;
		SPD3D09DEVICE   m_pDevice;		            //!<	レンダリングデバイス

    RECT2DI m_DefaultDisplayRect;
    D3DDEVICE_CREATION_PARAMETERS m_CreateParams;
		D3DCAPS9                      m_DeviceCaps;

    SWAPCHAINFORMAT  m_SwapChainFormat;
		LONG             m_WindowModeStyle;

    SPDRAWCOMMANDEXECUTE  m_pDrawCommandExecute;


	private:
    DllWrapper    m_D3DX9_LastDLL;
		typedef HRESULT (WINAPI *SHADERCOMPILE)(LPCSTR, UINT, CONST D3DXMACRO*, LPD3DXINCLUDE, LPCSTR, LPCSTR, DWORD, LPD3DXBUFFER*, LPD3DXBUFFER*, LPD3DXCONSTANTTABLE* );
    SHADERCOMPILE m_ShaderCompilerDefault;
    SHADERCOMPILE m_ShaderCompilerLast;

		typedef DWORD (WINAPI *GETSHADERVERSION)( CONST DWORD* );
    GETSHADERVERSION  m_GetShaderVersion;
	};

}}


#endif
