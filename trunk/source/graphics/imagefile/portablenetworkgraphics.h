/*!
 	@file
 	@brief	PNGÉtÉ@ÉCÉãëÄçÏ
 */

#ifndef maid2_graphics_imagefile_portablenetworkgraphics_h
#define maid2_graphics_imagefile_portablenetworkgraphics_h

#include"../../config/define.h"
#include"../../auxiliary/string.h"
#include"../surface.h"

namespace Maid
{
	namespace PNG
	{
    bool  Check( const std::vector<unt08>& FileImage );
    void  Load( const std::vector<unt08>& FileImage, Surface& dst );
    void  Save( const String& FileName, const Surface& src );
	};
}

#endif
