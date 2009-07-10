/*!
 	@file
 	@brief	PNGファイル操作
 */

#ifndef maid2_graphics_imagefile_portablenetworkgraphics_h
#define maid2_graphics_imagefile_portablenetworkgraphics_h

#include"../../config/define.h"
#include"../../auxiliary/string.h"
#include"../../auxiliary/functionresult.h"
#include"../surface.h"

namespace Maid
{
	namespace PNG
	{
    bool  Check( const std::vector<unt08>& FileImage );
    FUNCTIONRESULT  Load( const std::vector<unt08>& FileImage, SurfaceInstance& dst );
    FUNCTIONRESULT  Save( const String& FileName, const SurfaceInstance& src );
	};
}

#endif
