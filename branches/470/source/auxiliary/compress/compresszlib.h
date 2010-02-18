/*!
 	@file 
 	@brief	Zlib による圧縮・展開アルゴリズムルーチン
\n			圧縮ルーチンとして Zlib を使用しているのでZlibが必要です。
 */


#ifndef maid2_auxiliary_compress_compresszlib_h
#define maid2_auxiliary_compress_compresszlib_h

#include"icompress.h"

namespace Maid
{
	class CompressZLIB : public ICompress
	{
	public:
		virtual size_t Encode( const void* pSrc, size_t SrcLenght, void* pDst, size_t DstLength ) const;
		virtual size_t Decode( const void* pSrc, size_t SrcLenght, void* pDst, size_t DstLength ) const;
	};
}
#endif
