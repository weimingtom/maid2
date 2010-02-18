#ifndef maid2_graphics_graphicsobjecttemplate_h
#define maid2_graphics_graphicsobjecttemplate_h

/*!
    @file
    @brief  hObject を使うようなベースクラス
 */


#include"../config/define.h"

#include"core/ibuffer.h"
#include"core/itexture2d.h"
#include"core/iconstant.h"
#include"core/irendertarget.h"
#include"core/idepthstencil.h"
#include"core/imaterial.h"
#include"core/iinputlayout.h"
#include"core/isamplerstate.h"
#include"core/ivertexshader.h"
#include"core/ipixelshader.h"
#include"core/irasterizerstate.h"
#include"core/idepthstencilstate.h"
#include"core/iblendstate.h"

namespace Maid
{
  template<class TYPE>
  class GraphicsObject
  {
  public:
    const TYPE& Get() const {	return m_Object; }
  protected:
    void Set( const TYPE& t ) { m_Object = t; }
    void Clear() { m_Object = TYPE(); }

  private:
    TYPE  m_Object;
  };

  typedef GraphicsObject<Graphics::SPBUFFER>        IVertex;
  typedef GraphicsObject<Graphics::SPBUFFER>        IIndex;
  typedef GraphicsObject<Graphics::SPBUFFER>        IConstant;
  typedef GraphicsObject<Graphics::SPTEXTURE2D>     ITexture2D;
  typedef GraphicsObject<Graphics::SPINPUTLAYOUT>   IInputLayout;
  typedef GraphicsObject<Graphics::SPMATERIAL>      IMaterial;
  typedef GraphicsObject<Graphics::SPRENDERTARGET>  IRenderTarget;
  typedef GraphicsObject<Graphics::SPDEPTHSTENCIL>  IDepthStencil;

  typedef GraphicsObject<Graphics::SPSAMPLERSTATE>    ISamplerState;
  typedef GraphicsObject<Graphics::SPVERTEXSHADER>    IVertexShader;
  typedef GraphicsObject<Graphics::SPPIXELSHADER>     IPixelShader;


  typedef GraphicsObject<Graphics::SPRASTERIZERSTATE>   IRasterizerState;
  typedef GraphicsObject<Graphics::SPDEPTHSTENCILSTATE> IDepthStencilState;
  typedef GraphicsObject<Graphics::SPBLENDSTATE>        IBlendState;


}

#endif