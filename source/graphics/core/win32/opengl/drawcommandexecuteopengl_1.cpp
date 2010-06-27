#include"drawcommandexecuteopengl.h"

/*
#include"texture2dd3d11.h"
#include"rendertargetd3d11.h"
#include"depthstencild3d11.h"
#include"materiald3d11.h"
#include"inputlayoutd3d11.h"
#include"rasterizerstated3d11.h"
#include"samplerstated3d11.h"
#include"blendstated3d11.h"
*/

/*
#include"debug.h"
*/


namespace Maid { namespace Graphics {

DrawCommandExecuteOpenGL::DrawCommandExecuteOpenGL( OpenGL32DLL& dll, OpenGLExtension& ext )
  :m_Dll(dll)
  ,m_Ext(ext)
{
}

DrawCommandExecuteOpenGL::~DrawCommandExecuteOpenGL()
{
  Finalize();
}



void DrawCommandExecuteOpenGL::Initialize()
{
  m_PrimitiveTopology = GL_TRIANGLE_STRIP;

  m_VertexBuffer.resize( 8 );

  m_ShaderProgramID = m_Ext.glCreateProgram();
}

void DrawCommandExecuteOpenGL::Finalize()
{
  if( m_VertexBuffer.empty() ) { return ; }

  m_VertexBuffer = std::vector<VERTEXBUFFER>();
  m_Ext.glDeleteProgram( m_ShaderProgramID );

  m_pInputLayout = SPINPUTLAYOUT();
  m_pVertexShader = SPVERTEXSHADEROPENGL();
  m_pPixelShader = SPPIXELSHADEROPENGL();
}


void DrawCommandExecuteOpenGL::Begin()
{
}

void DrawCommandExecuteOpenGL::End()
{
}

void DrawCommandExecuteOpenGL::ClearDepthStencil( CLEARFLAG flag, float Depth, unt08 Stencil )
{
  if( flag==CLEARFLAG_DEPTH || flag==CLEARFLAG_DEPTHSTENCIL )
  {
    m_Dll.glClearDepth(Depth);
    m_Dll.glClear(GL_DEPTH_BUFFER_BIT);
  }

  if( flag==CLEARFLAG_STENCIL || flag==CLEARFLAG_DEPTHSTENCIL )
  {
    m_Dll.glClearStencil(Stencil);
    m_Dll.glClear(GL_STENCIL_BUFFER_BIT);
  }
}

void DrawCommandExecuteOpenGL::ClearRenderTarget( const COLOR_R32G32B32A32F& Col )
{
  m_Dll.glClearColor(Col.GetR(), Col.GetG(), Col.GetB(), Col.GetA());
  m_Dll.glClear(GL_COLOR_BUFFER_BIT);
}


void DrawCommandExecuteOpenGL::ResourceMap( const SPRESOURCE& pResource, int SubLevel, MAPTYPE Type, unt32 Flag, MAPPEDRESOURCE& Mapped )
{
  Mapped.pData = NULL;
  Mapped.Pitch = 0;
  Mapped.Slice = 0;

  switch( pResource->GetType() )
  {
  case IResource::TYPE_BUFFER:
    {
      BufferOpenGL* pBuffer = static_cast<BufferOpenGL*>( pResource.get() );

      Mapped.pData = pBuffer->Lock();
      Mapped.Pitch = pBuffer->GetParam().Length;
/*
      BufferD3D11* pBuffer = static_cast<BufferD3D11*>( pResource.get() );

      const D3D11_MAP t = MAPTYPEtoD3D11_MAP(Type);
      const UINT f = Flag;
      D3D11_MAPPED_SUBRESOURCE  Data;

      {
        const HRESULT ret = m_pDevice->Map( pBuffer->pBuffer.get(), SubLevel, t, f, &Data );
        if( FAILED(ret) ) 
        {
          MAID_WARNING( "ID3D11Buffer::Map() " << ret 
            << " D3D11_MAP:" << DebugStringD3D11(t)
            << " Flag:" << f
            );
          return; 
        }
      }
      Mapped.pData = Data.pData;
      Mapped.Pitch = pBuffer->GetParam().Length;
*/
    }break;
/*

  case IResource::TYPE_TEXTURE2D:
    {
      Texture2DD3D11* pBuffer = static_cast<Texture2DD3D11*>( pResource.get() );

      const UINT s = SubLevel;
      const D3D11_MAP t = MAPTYPEtoD3D11_MAP(Type);
      const UINT f = Flag;
      D3D11_MAPPED_SUBRESOURCE  Data;

      {
        const HRESULT ret = m_pDevice->Map( pBuffer->pTexture.get(), SubLevel, t, f, &Data );
        if( FAILED(ret) )
        {
          MAID_WARNING( "ID3D11Texture2D::Map() " << ret 
            << " D3D11_MAP:" << DebugStringD3D11(t)
            << " SubLevel:" << s
            << " Flag:" << f
            ); 
          return; 
        }
      }

      Mapped.pData = Data.pData;
      Mapped.Pitch = Data.RowPitch;
    }break;
*/
  }
}

void DrawCommandExecuteOpenGL::ResourceUnmap( const SPRESOURCE& pResource, int SubLevel )
{
  switch( pResource->GetType() )
  {
  case IResource::TYPE_BUFFER:
    {
      BufferOpenGL* pBuffer = static_cast<BufferOpenGL*>( pResource.get() );

      pBuffer->UnLock();

/*
      BufferD3D11* pBuffer = static_cast<BufferD3D11*>( pResource.get() );
      m_pDevice->Unmap( pBuffer->pBuffer.get(), SubLevel );
*/
    }break;
/*

  case IResource::TYPE_TEXTURE2D:
    {
      Texture2DD3D11* pBuffer = static_cast<Texture2DD3D11*>( pResource.get() );
      m_pDevice->Unmap( pBuffer->pTexture.get(), SubLevel );
    }break;
*/
  }
}

}}

