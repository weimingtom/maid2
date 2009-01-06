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

    const SIZE2DI& GetSize() const{ return m_Size; }
    const SIZE2DI& GetCreateSize()  const{ return m_CreateSize;  }
    const SIZE2DI& GetTextureSize() const{ return m_TextureSize; }

  protected:
    void Clear();
    bool IsSetupped() const;
    void Setup( const Graphics::SPTEXTURE2D& pTex, const Graphics::SPMATERIAL& pMat, const SIZE2DI& def, const SIZE2DI& create );

  private:
    SIZE2DI m_Size;   //  ユーザーが指定したサイズ
    SIZE2DI m_CreateSize;
    SIZE2DI m_TextureSize;
  };	
	
}

#endif