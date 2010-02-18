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
    :public IRenderTarget
  {
  public:
    RenderTargetBase();
    const SIZE2DI& GetSize()  const{ return m_Size;  }
    const SIZE2DI& GetTextureSize() const{ return m_TextureSize; }
    PIXELFORMAT GetFormat() const{ return m_Format; }

    RECT2DI CalcDrawArea() const;

  protected:

    enum DRAWAREA
    {
      DRAWAREA_CENTER,      //  描画範囲は中央
      DRAWAREA_LEFTTOP,     //  左上
      DRAWAREA_RIGHTTOP,    //  右上
      DRAWAREA_LEFTBOTTOM,  //  左下
      DRAWAREA_RIGHTBOTTOM, //  右下
    };

    void Clear();
    bool IsSetupped() const;
    void Setup( const Graphics::SPRENDERTARGET& pTarget, const SIZE2DI& virtualsize, const SIZE2DI& realsize, DRAWAREA area );
    void Setup( const Graphics::SPRENDERTARGET& pTarget );

  private:
    SIZE2DI m_Size;       //  ユーザーが指定した大きさ
    SIZE2DI m_RealSize;   //  実際の大きさ
    SIZE2DI m_TextureSize;
    PIXELFORMAT m_Format;
    DRAWAREA    m_Area;
  };
	
}

#endif