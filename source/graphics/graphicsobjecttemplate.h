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
}

#endif