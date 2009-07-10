/*!
 	@file
 	@brief	jpegファイル操作
 */

#ifndef maid2_graphics_imagefile_jpeg_h
#define maid2_graphics_imagefile_jpeg_h


#include"../../config/define.h"
#include"../../auxiliary/string.h"
#include"../../auxiliary/functionresult.h"
#include"../surface.h"


namespace Maid
{
  namespace Jpeg
  {
    bool  Check( const std::vector<unt08>& FileImage );
    FUNCTIONRESULT  Load( const std::vector<unt08>& FileImage, SurfaceInstance& dst );
    FUNCTIONRESULT  Save( const SurfaceInstance& src, std::vector<unt08>& FileImage );
  };
}

#endif
