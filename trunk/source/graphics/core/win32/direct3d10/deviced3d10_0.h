/*!	
	@file
	@brief	レンダリングドライバ Direct3D10 Ver.
*/

#ifndef maid2_graphics_core_win32_direct3d10_deviced3d10_0_h
#define maid2_graphics_core_win32_direct3d10_deviced3d10_0_h

#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include<vector>

#include"../../idevice.h"

#include"../../../../framework/win32/window.h"
#include"../../../../framework/win32/dllwrapper.h"
#include"../../../../auxiliary/thread/win32/threadmutex.h"
#include"common.h"

namespace Maid { namespace Graphics {

	class DeviceD3D10_0 
    : public IDevice
	{
	public:
    DeviceD3D10_0( const DllWrapper& dll, const SPDXGIFACTORY& pFactory, const SPDXGIADAPTER& pAdapter, Window& Windw );

    void Initialize();
    void Finalize();
    virtual void SerchDisplayMode( std::vector<DISPLAYMODE>& mode ) const;
    virtual void SerchDisplayInfo( std::vector<DISPLAYINFO>& info ) const;
    virtual void SerchEnableFormat( ENABLEFORMAT& caps ) const;
    virtual void SetScreenMode( const SCREENMODE& mode );
    virtual void Present();
    virtual SCREENMODE GetScreenMode() const;
    virtual SIZE2DI GetAspectRatio() const;

 
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

    virtual bool CompileShaderLanguage( const String& Source, std::vector<unt08>& Binary, String& ErrorMessage );

    const SPD3D10DEVICE& GetDevice() const { return m_pDevice; }
    const SPDXGIFACTORY& GetFactory() const { return m_pFactory; }

	protected:
    virtual SPD3D10DEVICE CreateDevice( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter );
    void CreateDefaultVertexShader( int no, std::vector<unt08>& Binary );
    void CreateDefaultPixelShader( int no, std::vector<unt08>& Binary );


	private:
    const DllWrapper&   m_DLL;
    const SPDXGIFACTORY m_pFactory;
    const SPDXGIADAPTER m_pAdapter;
    Window&	      m_Window;

    DllWrapper    m_D3DX10_LastDLL;
		typedef HRESULT (WINAPI *SHADERCOMPILE)(LPCSTR, SIZE_T, LPCSTR, 
      CONST D3D10_SHADER_MACRO*, LPD3D10INCLUDE, LPCSTR, LPCSTR, UINT, UINT, 
      ID3DX10ThreadPump*, ID3D10Blob**, ID3D10Blob**, HRESULT* );
    SHADERCOMPILE m_ShaderCompilerDefault;
    SHADERCOMPILE m_ShaderCompilerLast;

		SPD3D10DEVICE   m_pDevice;		            //!<	レンダリングデバイス
    SPDXGISWAPCHAIN m_pSwapChain;

    SPDRAWCOMMANDEXECUTE m_pDrawCommandExecute;

    SIZE2DI m_AspectRatio;

    SCREENMODE  m_ScreenMode;
		LONG        m_WindowModeStyle;

	private:
    static BOOL CALLBACK EnumWindowsProc(HWND hWnd , LPARAM lp);

    struct OTHERWINDOWINFO
    {
	    HWND    hWnd;
	    RECT2DI	Rect;
    };
    std::vector<OTHERWINDOWINFO>	m_WindowInfoList;
	};

}}


#endif
