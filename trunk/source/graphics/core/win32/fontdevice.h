/*!
	  @file
	  @brief windows用フォント管理クラス
 */

#ifndef framework_win32_fontdevice_h
#define framework_win32_fontdevice_h

#include"../../../config/define.h"
#include"../ifontdevice.h"

namespace Maid { namespace Graphics {

  class FontDevice : public IFontDevice
  {
  public:
    virtual void Initialize();
    virtual void GetFontList( std::vector<FONTINFO>& List );

    virtual SPFONT CreateFont( const String& name, const SIZE2DI& size );
    virtual String GetDefaultFontName()const;
    
    virtual void Rasterize( const SPFONT& pFont, unt32 FontCode, SurfaceInstance& surf );
  };
}}


#endif