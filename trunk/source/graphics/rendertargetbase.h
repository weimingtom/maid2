#ifndef maid2_graphics_rendertargetbase_h
#define maid2_graphics_rendertargetbase_h

/*!
    @file
    @brief  RenderTargetのベース。このRenderTargetは必ずテクスチャである
 */


#include"../config/define.h"

#include"graphicsobjecttemplate.h"

namespace Maid
{
  class RenderTargetBase
    :private IRenderTarget
  {
  public:
    RenderTargetBase();
    const Graphics::SPRENDERTARGET& Get() const {	return IRenderTarget::Get(); }
    const SIZE2DI& GetSize()  const{ return m_Size;  }
    const SIZE2DI& GetTextureSize() const{ return m_TextureSize; }

  protected:
    void Clear();
    bool IsSetupped() const;
    void Setup( const Graphics::SPRENDERTARGET& pTarget, const SIZE2DI& create );
    void Setup( const Graphics::SPRENDERTARGET& pTarget );

  private:
    SIZE2DI m_Size;   //  ユーザーが指定した大きさ
    SIZE2DI m_TextureSize;
  };
	
}

#endif