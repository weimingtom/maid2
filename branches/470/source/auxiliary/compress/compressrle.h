/*!
 	@file 
 	@brief	Run Length Encording による圧縮・展開アルゴリズムルーチン
 */


#ifndef maid2_auxiliary_compress_compressrle_h
#define maid2_auxiliary_compress_compressrle_h

#include"icompress.h"

namespace Maid
{
	class CompressRLE : public ICompress
	{
	public:
		virtual size_t Encode( const void* pSrc, size_t SrcLenght, void* pDst, size_t DstLength ) const;
		virtual size_t Decode( const void* pSrc, size_t SrcLenght, void* pDst, size_t DstLength ) const;
	};
}
#endif
