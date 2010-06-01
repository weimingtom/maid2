#ifndef maid2_graphics_core_win32_opengl_drawcommandexecuteopengl_h
#define maid2_graphics_core_win32_opengl_drawcommandexecuteopengl_h

#include"../../../../config/define.h"

#include"../../idrawcommandexecute.h"
#include"common.h"

#include"bufferopengl.h"

#include"blendstateopengl.h"
#include"vertexshaderopengl.h"
#include"pixelshaderopengl.h"

namespace Maid { namespace Graphics {

  /*!	
      @class	DrawCommandExecuteD3D11 drawcommandplayerd3d11.h
      @brief	描画コンテキスト 即時実行タイプ
  */

  class DrawCommandExecuteOpenGL
    : public IDrawCommandExecute
  {
  public:
    DrawCommandExecuteOpenGL( OpenGL32DLL& dll, OpenGLExtension& ext );
    ~DrawCommandExecuteOpenGL();

    void Initialize();
    void Finalize();


    virtual void Begin();
    virtual void End();

    virtual void ClearDepthStencil( CLEARFLAG flag, float Depth, unt08 Stencil );
    virtual void ClearRenderTarget( const COLOR_R32G32B32A32F& Col );


    virtual void ResourceMap( const SPRESOURCE& Resource, int SubLevel, MAPTYPE Type, unt32 Flag, MAPPEDRESOURCE& Mapped );
    virtual void ResourceUnmap( const SPRESOURCE& Resource, int SubLevel );
    virtual void SetIndex( const SPBUFFER& Buffer, size_t Offset );
    virtual void SetVertex( int Slot, const SPBUFFER& Buffer, int Offset, int Stride );
    virtual void SetInputLayout( const SPINPUTLAYOUT& Layout );
    virtual void SetPrimitiveTopology( PRIMITIVE_TOPOLOGY Topology );

    virtual void VSSetConstant( int slot, const SPBUFFER& pBuffer );
    virtual void VSSetMaterial( int stage, const SPMATERIAL& Material );
    virtual void VSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler );
    virtual void VSSetShader( const SPVERTEXSHADER& Shader );

    virtual void PSSetConstant( int slot, const SPBUFFER& pBuffer );
    virtual void PSSetMaterial( int stage, const SPMATERIAL& Material );
    virtual void PSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler );
    virtual void PSSetShader( const SPPIXELSHADER& Shader );

    virtual void SetViewPort( const VIEWPORT& port );
    virtual void SetRasterizerState( const SPRASTERIZERSTATE& State );
    virtual void SetDepthStencilState( const SPDEPTHSTENCILSTATE& State );
    virtual void SetBlendState( const SPBLENDSTATE& State );
    virtual void SetRenderTarget( int no, const SPRENDERTARGET& Color, const SPDEPTHSTENCIL& Depth );

    virtual VIEWPORT GetViewport();
    virtual void GetRenderTarget( int no, SPRENDERTARGET& Color, SPDEPTHSTENCIL& Depth );

    virtual void Draw( size_t UseVertexCount, size_t StartVertex );
    virtual void DrawIndexed( size_t UseIndexCount, size_t StartIndex, size_t OffsetVertex );

    virtual void CopyResource( const SPRESOURCE& pDst, const SPRESOURCE& pSrc );

    virtual void GenerateMips( const SPMATERIAL& pMaterial );


  private:
    OpenGL32DLL&     m_Dll;
    OpenGLExtension& m_Ext;

    struct VERTEXBUFFER
    {
      SPBUFFEROPENGL  pBuffer;
      int       Offset;
      int       Stride;

      VERTEXBUFFER()
        :Offset(0)
        ,Stride(0)
      {}
    };

    std::vector<VERTEXBUFFER> m_VertexBuffer;
    SPINPUTLAYOUT             m_pInputLayout;
    PRIMITIVE_TOPOLOGY        m_PrimitiveTopology;

    SPVERTEXSHADEROPENGL  m_pVertexShader;
    SPPIXELSHADEROPENGL   m_pPixelShader;

    GLuint  m_ShaderProgramID;
  };

}}

#endif