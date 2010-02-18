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
#include"texture2dbase.h"
#include"font.h"


#include<map>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class FontTexture 
    : public Texture2DBase
    , public GlobalPointer<GraphicsCore>
  {
  public:
    void Load( const Surface& surf );
    void Destroy();
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
  };


}
#endif
