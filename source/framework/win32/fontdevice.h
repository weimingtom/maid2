/*!
	  @file
	  @brief windows用フォント管理クラス
 */

#ifndef framework_win32_fontdevice_h
#define framework_win32_fontdevice_h

#include"../config/define.h"
#include"../ifontdevice.h"

namespace Maid
{
  class FontDevice : public IFontDevice
  {
  public:
    virtual void GetFontList( std::vector<FONTINFO>& List );

    virtual SPFONT CreateFont( const String& name, const SIZE2DI& size );
    virtual SPFONT CreateDefaultFont( const SIZE2DI& size );
    
    virtual void Rasterize( const SPFONT& pFont, unt32 FontCode, SurfaceSystemMemory& surf );
  };
}


#endif