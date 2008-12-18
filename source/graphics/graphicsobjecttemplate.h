#ifndef maid2_graphics_graphicsobjecttemplate_h
#define maid2_graphics_graphicsobjecttemplate_h

/*!
    @file
    @brief  hObject を使うようなベースクラス
 */


#include"../config/define.h"

#include"core/hobject.h"

namespace Maid
{
  template<class TYPE>
  class GraphicsObject
  {
  public:
    const TYPE& Get() {	return m_Object; }
  protected:
    void Set( const TYPE& t ) { m_Object = t; }
    void Clear() { m_Object.ID = hObject(Graphics::hObject::ID_NONE); }

  private:
    TYPE	m_Object;
  };

  typedef GraphicsObject<Graphics::hRenderTarget> IRenderTarget;
  typedef GraphicsObject<Graphics::hVertex>       IVertex;
  typedef GraphicsObject<Graphics::hIndex>        IIndex;
  typedef GraphicsObject<Graphics::hTexture2D>    ITexture2D;
  typedef GraphicsObject<Graphics::hConstant>     IConstant;

  typedef GraphicsObject<Graphics::hVertexShader>    IVertexShader;
  typedef GraphicsObject<Graphics::hPixelShader>     IPixelShader;

}

#endif