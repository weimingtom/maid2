/*!
 	  @file
 	  @brief	フォントテクスチャの管理クラス
 */

#ifndef maid2_graphics_fonttexturemanager_h
#define maid2_graphics_fonttexturemanager_h

#include"../config/define.h"

#include"../auxiliary/cachemanager.h"
#include"../auxiliary/globalpointer.h"
#include"../graphics/core/ifontdevice.h"
#include"surface.h"
#include"graphicsobjecttemplate.h"
#include"graphicscore.h"
#include"font.h"


#include<map>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class FontTexture 
    : public IMaterial
    , public GlobalPointer<GraphicsCore>
  {
  public:
    void Load( const Surface& surf );
    void Destroy();

    const SIZE2DI& GetFontSize()   const;
    const SIZE2DI& GetCreateSize() const;
    const SIZE2DI& GetTextureSize()const;

  private:
    SIZE2DI m_FontSize;
    SIZE2DI m_CreateSize;
    SIZE2DI m_TextureSize;
  };


  class FontTextureManager
  {
  public:
    FontTextureManager();
    ~FontTextureManager();

    void Initialize();
    void Finalize();
    const FontTexture& GetTexture( const Font& font, unt32 FontCode );

    void ClearCache();
    void SetCacheSize( int Size );
    void Garbage();

    void SetFilter( const FONTRASTERIZEFILTER& Filter );
  private:

    //!	検索に使うキャッシュキー
    struct CACHEKEY
    {
      CACHEKEY();
      CACHEKEY( const CACHEKEY& rha );
      CACHEKEY( const Font& f, unt32 code );
      const Font font;
      unt32	  Code;

      bool operator < ( const CACHEKEY& rhs ) const;
    };


    typedef CacheManager<CACHEKEY,FontTexture> CACHETABLE;

    CACHETABLE	m_CacheTable;

    FONTRASTERIZEFILTER    m_Filter;
  };


}
#endif
