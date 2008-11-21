/*!	
	@file
	@brief	返信メッセージ 定義
*/

#ifndef maid2_graphic_graphicreturnmessage1_h
#define maid2_graphic_graphicreturnmessage1_h

#include"../config/define.h"
#include"../auxiliary/string.h"
#include"graphicreturnmessagebase.h"

#include"core/driver/surfacebuffermemory.h"

namespace Maid
{
	namespace GraphicReturnMessage
	{
		class CopyRenderTarget : public Base
		{
		public:
			CopyRenderTarget() : Base(COPYRENDERTARGET){}

			OBJECTID	ID;
			SPSURFACEBUFFERMEMORY pSurface;
		};

		class DriverCaps : public Base
		{
		public:
			DriverCaps() : Base(DRIVERCAPS){}

			mstring	CapsText;
		};
	}
}

#endif