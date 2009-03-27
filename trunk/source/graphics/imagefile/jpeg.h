/*!
 	@file
 	@brief	jpegÉtÉ@ÉCÉãëÄçÏ
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
    FUCTIONRESULT  Load( const std::vector<unt08>& FileImage, Surface& dst );
    FUCTIONRESULT  Save( const Surface& src, std::vector<unt08>& FileImage );
  };
}

#endif
