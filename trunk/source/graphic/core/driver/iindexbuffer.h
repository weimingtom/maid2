/*!
    @file
    @brief	インデックスバッファベースクラス
 */

#ifndef maid2_graphic_core_iindexbuffer_h
#define maid2_graphic_core_iindexbuffer_h

#include"../../../config/define.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
	/*!
      @class	IIndexBuffer iindexbuffer.h
      @brief	頂点バッファの基本クラス
	 */
	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer(){}	//!<	デストラクタ
	};

	typedef boost::shared_ptr<IIndexBuffer> SPINDEXBUFFER;
}

#endif

