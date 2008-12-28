/*!
 	@file
 	@brief	ビットマップファイル操作クラス
 */

#ifndef maid2_graphics_imagefile_bitmap_h
#define maid2_graphics_imagefile_bitmap_h


#include"../../config/define.h"
#include"../../auxiliary/string.h"
#include"../surface.h"


namespace Maid
{
  namespace Bitmap
  {
    bool  Check( const std::vector<unt08>& FileImage );
    void  Load( const std::vector<unt08>& FileImage, Surface& dst );
    void  Save( const String& FileName, const Surface& src );
  };
}

#endif
