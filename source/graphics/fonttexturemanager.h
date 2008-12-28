/*!
 	  @file
 	  @brief	フォントテクスチャの管理クラス
 */

#ifndef maid2_graphics_fonttexturemanager_h
#define maid2_graphics_fonttexturemanager_h

#include"../config/define.h"

#include"../auxiliary/cachemanager.h"
#include"../auxiliary/globalpointer.h"
#include"../framework/ifontdevice.h"
#include"surface.h"
#include"graphicsobjecttemplate.h"
#include"graphicscore.h"


#include<map>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class FontTexture 
    : public IMaterial
    ,GlobalPointer<GraphicsCore>
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

  class IFontFilter
  {
  public:
    virtual ~IFontFilter(){}
    virtual int Convert( SPSURFACE& pData )=0;
  };

  typedef	boost::shared_ptr<IFontFilter>	SPFONTFILTER;
  typedef	std::vector<SPFONTFILTER>	FONTFILTER;

  class FontTextureManager
  {
  public:
    FontTextureManager();
    ~FontTextureManager();

    void Initialize( const SPFONTDEVICE& pFont );
    void Finalize();
    const FontTexture& GetTexture( const SPFONT& pFont, unt32 FontCode );

    void ClearCache();
    void SetCacheSize( int Size );
    void Garbage();

    void SetFilter( const FONTFILTER& Filter );
  private:

    //!	検索に使うキャッシュキー
    struct CACHEKEY
    {
      CACHEKEY();
      CACHEKEY( const CACHEKEY& rha );
      CACHEKEY( const SPFONT& p, unt32 code );
      const SPFONT pFont;
      unt32	  Code;

      bool operator < ( const CACHEKEY& rhs ) const;
    };


    typedef CacheManager<CACHEKEY,FontTexture> CACHETABLE;

    CACHETABLE	m_CacheTable;

    FONTFILTER    m_Filter;
    SPFONTDEVICE	m_pFontDevice;
  };


}
#endif
