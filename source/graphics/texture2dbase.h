#ifndef maid2_graphics_texture2dbase_h
#define maid2_graphics_texture2dbase_h

/*!
    @file
    @brief  テクスチャのベースクラス
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"graphicscore.h"

namespace Maid
{
  class Texture2DBase
    :public ITexture2D
    ,public IMaterial
  {
  public:
    Texture2DBase();
    virtual ~Texture2DBase(){}

    const SIZE2DI& GetSize() const{ return m_VirtualSize; }
    const SIZE2DI& GetRealSize()  const{ return m_RealSize;  }
    const SIZE2DI& GetTextureSize() const{ return m_TextureSize; }

    PIXELFORMAT GetVirtualFormat() const { return m_VirtualFormat; }
    PIXELFORMAT GetRealFormat() const { return m_RealFormat; }

    RECT2DF CalcUV( const RECT2DI& rc ) const;

  protected:
    void Clear();
    bool IsSetupped() const;
    void Setup( const Graphics::SPTEXTURE2D& pTex, const Graphics::SPMATERIAL& pMat, const SIZE2DI& Virtual, const SIZE2DI& real, PIXELFORMAT fmt );

  private:
    SIZE2DI m_VirtualSize;   //  ユーザーが指定したサイズ
    SIZE2DI m_RealSize;
    SIZE2DI m_TextureSize;
    PIXELFORMAT m_VirtualFormat;
    PIXELFORMAT m_RealFormat;
  };	
	
}

#endif