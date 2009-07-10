/*!
 	@file
 	@brief	ビットマップファイル操作
 */

#ifndef maid2_graphics_imagefile_bitmap_h
#define maid2_graphics_imagefile_bitmap_h


#include"../../config/define.h"
#include"../../auxiliary/string.h"
#include"../../auxiliary/functionresult.h"
#include"../surface.h"


namespace Maid
{
  namespace Bitmap
  {
    bool  Check( const std::vector<unt08>& FileImage );
    FUNCTIONRESULT  Load( const std::vector<unt08>& FileImage, SurfaceInstance& dst );
    FUNCTIONRESULT  CheckSave( const SurfaceInstance& src, std::vector<unt08>& FileImage );
    FUNCTIONRESULT  Save( const SurfaceInstance& src, std::vector<unt08>& FileImage );
  };
}

#endif
