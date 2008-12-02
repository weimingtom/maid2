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
#include"d3d09include.h"

#include"object.h"

namespace Maid { namespace Graphics {

	class DeviceD3D09 : public IDevice
	{
	public:
    DeviceD3D09( IDirect3DDevice9* pDev, Window& Windw );


    void Initialize();
    void Finalize();
    virtual void SerchDisplayMode( std::vector<DISPLAYMODE>& mode );
    virtual void SerchEnableFormat( ENABLEFORMAT& caps );
    virtual void Reset( const SCREENMODE& mode );
    virtual DEVICESTATE TestCooperativeLevel();
    virtual void Flip();
    virtual SCREENMODE GetScreenMode();
    virtual void DeleteObject( const OBJECT& Object );
    virtual const OBJECTDESC& GetObjectDesc( const OBJECT& Object );
    virtual RESOURCE CreateResource( const CREATERESOURCEPARAM& param );
    virtual RENDERTARGET CreateRenderTarget( const RESOURCE& resource, const CREATERENDERTARGETPARAM& param );
    virtual DEPTHSTENCIL CreateDepthStencil( const RESOURCE& resource, const CREATEDEPTHSTENCILPARAM& param );
    virtual SHADERMATERIAL CreateShaderMaterial( const RESOURCE& resource, const CREATESHADERMATERIALPARAM& param );

    virtual VERTEXSHADER CreateVertexShader( const void* pData, size_t Length );
    virtual PIXELSHADER CreatePixelShader( const void* pData, size_t Length );

    virtual SAMPLERSTATE CreateSamplerState( const SAMPLERSTATEPARAM& Option );
    virtual RASTERIZERSTATE CreateRasterizerState( const RASTERIZERSTATEPARAM& Option );
    virtual BLENDSTATE& CreateBlendState( const BLENDSTATEPARAM& Option );
    virtual INPUTLAYOUT CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength );

    virtual IDrawCommandPlayer* CreateDrawCommandPlayer();
    virtual IDrawCommandRecorder* CreateDrawCommandRecorder();

	private:
    SPD3D09 GetD3D();

    OBJECT CreateObject( const SPOBJECTINFO& pObj );


	private:
		Window&	      m_Window;
		SPD3D09DEVICE m_pDevice;		            //!<	レンダリングデバイス

    D3DDEVICE_CREATION_PARAMETERS m_CreateParams;
		D3DCAPS9                      m_DeviceCaps;
    D3DPRESENT_PARAMETERS         m_CurrentMode;

		SPD3D09SURFACE	m_pDefaultRenderTarget;
		SPD3D09SURFACE  m_pDefaultDepthStencil;

    SCREENMODE  m_ScreenMode;
		LONG      m_WindowModeStyle;
		POINT2DI  m_WindowModePos;

    std::vector<SPOBJECTINFO> m_Object;
    std::vector<int>      m_ObjectEmptyList;
    int                   m_ObjectUseCount; //  m_Object が使われている個数
    ThreadMutex           m_ObjectMutex;    //  m_Object 更新用
    std::vector<OBJECT>   m_DeleteObjectList; //  次のフレームで削除されるオブジェクトリスト

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
