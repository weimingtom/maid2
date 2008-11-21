/*!	
	@file
	@brief	レンダリングドライバ Direct3D9 Ver.
*/

#ifndef graphic_core_driver_win32_direct3d09_videodeviced3d09_h
#define graphic_core_driver_win32_direct3d09_videodeviced3d09_h

#include"../../../../../config/define.h"
#include"../../../../../config/typedef.h"

#include<vector>

#include"../../ivideodevice.h"

#include"../../../../../framework/win32/window.h"
#include"d3d09include.h"


namespace Maid
{
	class VideoDeviceD3D09 : public IVideoDevice
	{
	public:
    VideoDeviceD3D09( IDirect3DDevice9* pDev, Window& Windw );

    virtual void Reset( const SCREENMODE& mode );
    virtual void Restore();
    virtual DEVICESTATE TestCooperativeLevel();
    virtual void Flip();
    virtual SCREENMODE GetScreenMode();


    virtual ISurfaceBuffer* CreateRenderTargetSurface( const SIZE2DI& size, PIXELFORMAT fmt, SAMPLETYPE type, int Quality );
    virtual ISurfaceBuffer* CreateDepthStencileSurface( const SIZE2DI& size, PIXELFORMAT fmt, SAMPLETYPE type, int Quality );
    virtual ITextureBuffer* CreateTextureBuffer( const TextureBufferMemory& buffer );
    virtual ITextureBuffer* CreateRenderTargetTextureBuffer( const SIZE2DI& size, PIXELFORMAT fmt );
    virtual IIndexBuffer* CreateIndexBuffer( const std::vector<unt08>& data, int Format );
    virtual IVertexBuffer* CreateVertexBuffer( const std::vector<unt08>& data, unt32 Format );
    virtual IVertexShaderBuffer* CreateVertexShader( const std::vector<unt08>& Code );
    virtual IPixelShaderBuffer* CreatePixelShader( const std::vector<unt08>& Code );
    virtual IVertexDeclarationBuffer* CreateVertexDeclarationBuffer( const VertexDeclarationBufferMemory& buffer );

    virtual void SetRenderTarget( const ISurfaceBuffer* pColor, const ISurfaceBuffer* pDepth );
    virtual void ResetRenderTarget();

    virtual void SetTextureBuffer( int stage, const ITextureBuffer* pBuffer );
    virtual void SetIndexBuffer( const IIndexBuffer* pBuffer );
    virtual void SetVertexBuffer( int pos, const IVertexBuffer* pBuffer );
    virtual void SetVertexShader( const IVertexShaderBuffer* pShader );
    virtual void SetPixelShader( const IPixelShaderBuffer* pShader );
    virtual void SetVertexDeclaration( const IVertexDeclarationBuffer* pDecl );
    virtual void SetVertexShaderConstF( int pos, const VECTOR4DF& vec );
    virtual void SetViewPort( const RECT2DI& screen, float MinZ, float MaxZ );
    virtual void SetRenderState( RENDERSTATE state, unt value );
    virtual void SetSamplerState( int Stage, SAMPLERSTATE state, unt value );
    virtual void CopySurface( const ISurfaceBuffer* pSrc, SurfaceBufferMemory& Dst );
    virtual void ClearRenderTarget( const COLOR_A32B32G32R32F& Color );
    virtual void ClearDepthStencil( float Depth, unt32 Stencil );

    virtual void BeginScene();
    virtual void EndScene();
    virtual void DrawPrimitive( PRIMITIVE prim, unt StartVertex, unt PrimitiveCount );
    virtual void DrawPrimitiveUP( PRIMITIVE prim, const void* pVertex, unt Stride, unt PrimitiveCount );
    virtual void DrawIndexedPrimitive( PRIMITIVE prim, int BaseVertexOffset, unt MinIndex, unt NumVertices, unt StartIndex, unt PrimitiveCount );
    virtual void DrawIndexPrimitiveUP( PRIMITIVE prim, int MinIndex, int NumVertices, const void* pVertex, int VertexSize, int PrimitiveCount, const void* pIndex, int IndexSize  );

  protected:
    virtual void SetupDevice();
    virtual void SerchDisplayMode( std::vector<DISPLAYMODE>& mode );
    virtual void SerchEnableFormat( ENABLEFORMAT& caps );

	private:
    SPD3D09 GetD3D();

	private:
		Window&	      m_Window;
		SPD3D09DEVICE m_pDevice;		            //!<	レンダリングデバイス

    D3DDEVICE_CREATION_PARAMETERS m_CreateParams;
		D3DCAPS9                      m_DeviceCaps;
    D3DPRESENT_PARAMETERS         m_CurrentMode;

		SPD3D09SURFACEBUFFER	m_pDefaultRenderTarget;
		SPD3D09SURFACEBUFFER  m_pDefaultDepthStencil;

    SCREENMODE  m_ScreenMode;
		LONG      m_WindowModeStyle;
		POINT2DI  m_WindowModePos;

	private:
    static BOOL CALLBACK EnumWindowsProc(HWND hWnd , LPARAM lp);

    struct OTHERWINDOWINFO
    {
	    HWND    hWnd;
	    RECT2DI	Rect;
    };
    std::vector<OTHERWINDOWINFO>	s_WindowInfoList;
	};
}


#endif
