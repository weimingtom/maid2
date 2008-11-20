/*!
 	@file
 	@brief	頂点定義ベースクラス
 */

#ifndef maid2_graphic_core_ivertexdeclarationbuffer_h
#define maid2_graphic_core_ivertexdeclarationbuffer_h

#include"../../../config/define.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
	/*!
      @class	IVertexDeclarationBuffer ivertexdeclarationbuffer.h
      @brief	頂点定義の基本クラス
	 */
	class IVertexDeclarationBuffer
	{
	public:
		virtual ~IVertexDeclarationBuffer(){}	//!<	デストラクタ
	};

	typedef boost::shared_ptr<IVertexDeclarationBuffer> SPVERTEXDECLARATIONBUFFER;
}

#endif

