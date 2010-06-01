/*!	
	@file
	@brief	レンダリングドライバ OpenGL Ver.
*/

#ifndef maid2_graphics_core_win32_opengl_deviceopengl_h
#define maid2_graphics_core_win32_opengl_deviceopengl_h

#include"../../../../config/define.h"
#include"../../../../config/typedef.h"


#include"../../idevice.h"

#include"../../../../framework/win32/window.h"
#include"../../../../framework/win32/dllwrapper.h"
#include"../../../../auxiliary/thread/win32/threadmutex.h"
#include"../../../../auxiliary/functionresult.h"
#include"common.h"

#include<vector>

namespace Maid { namespace Graphics {

	class DeviceOpenGL 
    : public IDevice
	{
	public:
    DeviceOpenGL( Window& Windw );

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


  private:
    void CreateDefaultVertexShader( int no, std::string& Binary );
    void CreateDefaultPixelShader( int no, std::string& Binary );

    void UpdateDisplayAspect();

	private:
    Window&	      m_Window;
    OpenGL32DLL   m_OpenGLDLL;
    OpenGLExtension m_OpenGLExtension;
    HDC           m_hDC;
    HGLRC         m_hGLContext;


    SIZE2DI         m_DisplayAspect;

    SPDRAWCOMMANDEXECUTE  m_pDrawCommandExecute;

    SWAPCHAINFORMAT   m_SwapChainFormat;

	};
}}


#endif
