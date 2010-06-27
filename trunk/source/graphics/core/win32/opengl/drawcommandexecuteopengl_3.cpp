#include"drawcommandexecuteopengl.h"
#include"deviceopengl.h"

#include"bufferopengl.h"
#include"texture2dopengl.h"

/*
#include"rendertargetd3d11.h"
#include"depthstencild3d11.h"
*/
#include"materialopengl.h"
#include"inputlayoutopengl.h"

/*
#include"rasterizerstated3d11.h"
#include"samplerstated3d11.h"
#include"blendstated3d11.h"
#include"vertexshaderd3d11.h"
#include"pixelshaderd3d11.h"
#include"debug.h"
*/

//  ここは各種Drawを書く

namespace Maid { namespace Graphics {

  enum DECLUSAGE
  {
    DECLUSAGE_NONE,

    DECLUSAGE_POSITION,
    DECLUSAGE_NORMAL,
    DECLUSAGE_COLOR,
    DECLUSAGE_TEXCOORD,
  };

  inline DECLUSAGE INPUT_ELEMENTSemanticNametoDECLUSAGE( const char* p )
  {
    DECLUSAGE ret = DECLUSAGE_NONE ;
    std::string str = p;

    std::transform(str.begin(), str.end(), str.begin(), tolower );

         if( str=="position" ) { ret = DECLUSAGE_POSITION; }
    else if( str=="normal"   ) { ret = DECLUSAGE_NORMAL; }
    else if( str=="color"    ) { ret = DECLUSAGE_COLOR; }
    else if( str=="texcoord" ) { ret = DECLUSAGE_TEXCOORD; }
/*
    else if( str=="blendweight" ) { ret = D3DDECLUSAGE_BLENDWEIGHT; }
    else if( str=="blendindices" ) { ret = D3DDECLUSAGE_BLENDINDICES; }
    else if( str=="psize" ) { ret = D3DDECLUSAGE_PSIZE; }
    else if( str=="tangent" ) { ret = D3DDECLUSAGE_TANGENT; }
    else if( str=="binormal" ) { ret = D3DDECLUSAGE_BINORMAL; }
    else if( str=="tessfactor" ) { ret = D3DDECLUSAGE_TESSFACTOR; }
    else if( str=="positiont" ) { ret = D3DDECLUSAGE_POSITIONT; }
    else if( str=="fog" ) { ret = D3DDECLUSAGE_FOG; }
    else if( str=="depth" ) { ret = D3DDECLUSAGE_DEPTH; }
    else if( str=="sample" ) { ret = D3DDECLUSAGE_SAMPLE; }
*/
    return ret;
  }


  inline GLint CoordinatePerVertex( INPUT_ELEMENT::TYPE t )
  {
    GLint ret = 4;
    switch( t )
    {
    case INPUT_ELEMENT::TYPE_FLOAT1: { ret=1; }break;
    case INPUT_ELEMENT::TYPE_FLOAT2: { ret=2; }break;
    case INPUT_ELEMENT::TYPE_FLOAT3: { ret=3; }break;
    case INPUT_ELEMENT::TYPE_FLOAT4: { ret=4; }break;
    case INPUT_ELEMENT::TYPE_COLOR:  { ret=1; }break;
    }

    return ret;
  }

  inline GLenum CoordinateType( INPUT_ELEMENT::TYPE t )
  {
    GLint ret = GL_FLOAT;
    switch( t )
    {
    case INPUT_ELEMENT::TYPE_FLOAT1: { ret=GL_FLOAT; }break;
    case INPUT_ELEMENT::TYPE_FLOAT2: { ret=GL_FLOAT; }break;
    case INPUT_ELEMENT::TYPE_FLOAT3: { ret=GL_FLOAT; }break;
    case INPUT_ELEMENT::TYPE_FLOAT4: { ret=GL_FLOAT; }break;
    case INPUT_ELEMENT::TYPE_COLOR:  { ret=GL_4_BYTES; }break;  //  通るかな？
    }
    return ret;
  }


void DrawCommandExecuteOpenGL::Draw( size_t UseVertexCount, size_t StartVertex )
{
  {
    const GLuint prog = m_ShaderProgramID;

    m_Ext.glLinkProgram( prog );
    m_Ext.glUseProgram( prog );
  }

  const std::vector<INPUT_ELEMENT>& InputElement = static_cast<InputLayoutOpenGL*>(m_pInputLayout.get())->Element;
  std::vector<GLenum> EnableList;

  for( int i=0; i<(int)InputElement.size(); ++i )
  {
    const INPUT_ELEMENT& ele = InputElement[i];
    const DECLUSAGE decl = INPUT_ELEMENTSemanticNametoDECLUSAGE( ele.SemanticName.c_str() );
    const VERTEXBUFFER& v = m_VertexBuffer[ele.SlotNo];
    const GLint cpv = CoordinatePerVertex( ele.Type );
    const GLenum type = CoordinateType( ele.Type );
    const GLsizei stride = v.Stride;
    const int offset = ele.Offset;

    m_Ext.glBindBuffer( GL_ARRAY_BUFFER, v.pBuffer->GetID() );
    switch( decl )
    {
    case DECLUSAGE_POSITION:
      {
        EnableList.push_back( GL_VERTEX_ARRAY );
        m_Ext.glVertexPointer( cpv, type, stride, (const GLvoid*)(offset) );
      }break;

    case DECLUSAGE_NORMAL: { }break;
    case DECLUSAGE_COLOR: 
      {
        EnableList.push_back( GL_COLOR_ARRAY );
        m_Ext.glColorPointer( cpv, type, stride, (const GLvoid*)(offset) );
      }break;
    case DECLUSAGE_TEXCOORD:
      {
        EnableList.push_back( GL_TEXTURE_COORD_ARRAY );
        m_Ext.glTexCoordPointer( cpv, type, stride, (const GLvoid*)(offset) );

        const int idx = ele.SemanticIndex;
        {
          char buf[256];
          sprintf( buf, "texture%0d", idx );
          const GLint tex_pos = m_Ext.glGetUniformLocation(m_ShaderProgramID, buf);
          m_Ext.glUniform1i(tex_pos,idx);
        }

      }break;
    }
  }


  for( int i=0; i<(int)EnableList.size(); ++i )
  {
    m_Dll.glEnableClientState( EnableList[i] );
  }

  m_Ext.glActiveTexture( GL_TEXTURE0 );
  m_Ext.glDrawArrays( m_PrimitiveTopology, 0, UseVertexCount );

  for( int i=0; i<(int)EnableList.size(); ++i )
  {
    m_Dll.glDisableClientState( EnableList[i] );
  }
}

void DrawCommandExecuteOpenGL::DrawIndexed( size_t UseIndexCount, size_t StartIndex, size_t OffsetVertex )
{
//  m_pDevice->DrawIndexed( UseIndexCount, StartIndex, OffsetVertex );
}
/*

ID3D11Resource* DrawCommandExecuteOpenGL::GetResource( const SPRESOURCE& pResource )
{
  ID3D11Resource* pRet = NULL;
  switch( pResource->GetType() )
  {
  case IResource::TYPE_BUFFER:
    {
      BufferD3D11* pBuffer = static_cast<BufferD3D11*>( pResource.get() );
      pRet = pBuffer->pBuffer.get();
    }break;

  case IResource::TYPE_TEXTURE2D:
    {
      Texture2DD3D11* pBuffer = static_cast<Texture2DD3D11*>( pResource.get() );
      pRet = pBuffer->pTexture.get();
    }break;
  }

  return pRet;
  return NULL;
}
*/


void DrawCommandExecuteOpenGL::CopyResource( const SPRESOURCE& pDstResource, const SPRESOURCE& pSrcResource )
{
//  ID3D11Resource* pDst = GetResource(pDstResource);
//  ID3D11Resource* pSrc = GetResource(pSrcResource);

//  m_pDevice->CopyResource( pDst, pSrc );
}


void DrawCommandExecuteOpenGL::GenerateMips( const SPMATERIAL& pMaterial )
{
//  ID3D11ShaderResourceView* pView = static_cast<MaterialD3D11*>(pMaterial.get())->pView.get();
//  m_pDevice->GenerateMips( pView );
}


}}

