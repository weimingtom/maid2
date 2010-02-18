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
#include"../../../../auxiliary/functionresult.h"
#include"common.h"

namespace Maid { namespace Graphics {

	class DeviceD3D10_0 
    : public IDevice
	{
	public:
    DeviceD3D10_0( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter, Window& Windw );

    void Initialize();
    void Finalize();
    virtual void SetFullScreenState( bool IsFullScreen );
    virtual bool IsFullScreen() const;
    virtual void SerchSwapChainSurfaceFormat( std::vector<PIXELFORMAT>& mode )  const;
    virtual void SetSwapChainFormat( const SWAPCHAINFORMAT& mode );

    virtual void SerchEnableFormat( ENABLEFORMAT& caps ) const;
    virtual float GetTexelMapValue() const;
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

    const SPD3D10DEVICE& GetDevice() const { return m_pDevice; }
    const SPDXGIFACTORY& GetFactory() const { return m_pFactory; }

	protected:
    virtual FUNCTIONRESULT CreateDeviceAndSwapChain( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter, DXGI_SWAP_CHAIN_DESC& desc, SPD3D10DEVICE& pDevice, SPDXGISWAPCHAIN& pSwapChain );

  private:
    void CreateDefaultVertexShader( int no, std::vector<unt08>& Binary );
    void CreateDefaultPixelShader( int no, std::vector<unt08>& Binary );

    void UpdateDisplayAspect();

	private:
    const DllWrapper&   m_DLL;
    const SPDXGIADAPTER m_pAdapter;
    SPDXGIFACTORY m_pFactory;
    Window&	      m_Window;

    DllWrapper    m_D3DX10_LastDLL;
		typedef HRESULT (WINAPI *SHADERCOMPILE)(LPCSTR, SIZE_T, LPCSTR, 
      CONST D3D10_SHADER_MACRO*, LPD3D10INCLUDE, LPCSTR, LPCSTR, UINT, UINT, 
      ID3DX10ThreadPump*, ID3D10Blob**, ID3D10Blob**, HRESULT* );
    SHADERCOMPILE m_ShaderCompilerDefault;
    SHADERCOMPILE m_ShaderCompilerLast;

    SIZE2DI         m_DisplayAspect;
		SPD3D10DEVICE   m_pDevice;		            //!<	レンダリングデバイス
    SPDXGISWAPCHAIN m_pSwapChain;

    SPDRAWCOMMANDEXECUTE m_pDrawCommandExecute;

    int m_SyncInterval;
	};

}}


#endif
