#include"jpeg.h"


#ifndef USE_INTELIPP
  extern "C" {
  #define XMD_H
  #include"../../import/ijg_ipp/jpeglib.h"
  }
#pragma comment(lib,"ippcorel.lib")
#pragma comment(lib,"ippiemerged.lib")
#pragma comment(lib,"ippimerged.lib")
#pragma comment(lib,"ippjemerged.lib")
#pragma comment(lib,"ippjmerged.lib")
#pragma comment(lib,"ippsemerged.lib")
#pragma comment(lib,"ippsmerged.lib")

#else
  extern "C" {
  #define XMD_H
  #include"../../import/ijg/jpeglib.h"
  }
#endif


#include"../../auxiliary/debug/warning.h"




// 使い方は　http://www.syuhitu.org/other/jpeg/jpeg.html　を参照

namespace Maid
{
  namespace Jpeg
  {

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定したデータがビットマップか調べる
/*!
    @param	FileImage	[i ]  データ

    @return ビットマップならtrue
            違うならfalse
*/
bool  Check( const std::vector<unt08>& FileImage )
{
  if( FileImage.size()<2 ) { return false; }

	return FileImage[0]==0xFF &&  FileImage[1]==0xD8;
}




	//	libjpeg をメモリから使えるようにするラッパ


/* メモリソースからのJPEG展開用マネージャ */
typedef struct {
	struct jpeg_source_mgr pub;	/* public fields */

	JOCTET * buffer;
	unsigned long buffer_length;
} memory_source_mgr;
typedef memory_source_mgr *memory_src_ptr;


METHODDEF(void) memory_init_source (j_decompress_ptr cinfo)
{
}


METHODDEF(boolean) memory_fill_input_buffer (j_decompress_ptr cinfo)
{
	memory_src_ptr src = (memory_src_ptr) cinfo->src;

	if( src->pub.bytes_in_buffer<=0 )
	{
		src->buffer[0] = (JOCTET) 0xFF;
		src->buffer[1] = (JOCTET) JPEG_EOI;
		src->pub.next_input_byte = src->buffer;
		src->pub.bytes_in_buffer = 2;
	}

	return TRUE;
}


METHODDEF(void) memory_skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
	memory_src_ptr src = (memory_src_ptr) cinfo->src;

	if (num_bytes > 0) {
		src->pub.next_input_byte += (size_t) num_bytes;

		if( src->pub.bytes_in_buffer < (size_t) num_bytes )
		{
			src->pub.bytes_in_buffer = 0;
		}else
		{
			src->pub.bytes_in_buffer -= (size_t) num_bytes;
		}
	}
}

METHODDEF(void) memory_term_source (j_decompress_ptr cinfo)
{
}

GLOBAL(void)
jpeg_memory_src (j_decompress_ptr cinfo, const void* data, unsigned long len)
{
	memory_src_ptr src;

	if (cinfo->src == NULL) {	/* first time for this JPEG object? */
		cinfo->src = (struct jpeg_source_mgr *)
		(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
		  sizeof(memory_source_mgr));
		src = (memory_src_ptr) cinfo->src;
		src->buffer = (JOCTET *)
		(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
		  len * sizeof(JOCTET));
	}


	src = (memory_src_ptr) cinfo->src;

	src->pub.init_source = memory_init_source;
	src->pub.fill_input_buffer = memory_fill_input_buffer;
	src->pub.skip_input_data = memory_skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
	src->pub.term_source = memory_term_source;

	src->pub.bytes_in_buffer = len;
	src->pub.next_input_byte = (JOCTET*)data;
}


struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{

}















/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! jpegを変換する
/*!
    @param	FileImage	[i ]  ファイル名
    @param	dst       [i ]  転送先
 */
FUNCTIONRESULT   Load( const std::vector<unt08>& FileImage, SurfaceInstance& surface )
{
  return Load( &(FileImage[0]), FileImage.size(), surface );
}

FUNCTIONRESULT  Load( const void* pFileImage, size_t FileImageSize, SurfaceInstance& surface )
{
	my_error_mgr jerr;
	jpeg_decompress_struct cinfo;

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	jpeg_create_decompress(&cinfo);

	jpeg_memory_src(&cinfo, pFileImage, FileImageSize );

	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	const int row_stride = cinfo.output_width * cinfo.output_components;

	const SIZE2DI size(cinfo.output_width, cinfo.output_height);
	PIXELFORMAT fmt = PIXELFORMAT_B08G08R08I;

	if( cinfo.out_color_components==3 )
	{	//	グレースケールなら cinfo.out_color_components==1 
		//	そのときには cinfo.colormap も設定される
		fmt = PIXELFORMAT_B08G08R08I;
	}else
	{
		MAID_ASSERT( true, "未対応" );
	}
	
	surface.Create( size, fmt );

	unt08* pSurface = (unt08*)surface.GetPlanePTR();

	{
		std::vector<JSAMPLE>	buffer((row_stride+15)&(~15));

		JSAMPLE* pBuff = &buffer[0];

		unt08* pDst = pSurface;

		while (cinfo.output_scanline < cinfo.output_height) {
			jpeg_read_scanlines(&cinfo, &pBuff, 1);

			memcpy( pDst, pBuff, row_stride );
			pDst += row_stride;
		}
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

  return FUNCTIONRESULT_OK;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 読み込んであるサーフェスをビットマップとして保存する
/*!
 */
FUNCTIONRESULT Save( const SurfaceInstance& src, std::vector<unt08>& FileImage )
{
  MAID_ASSERT( true, "未実装" );
  return FUNCTIONRESULT_OK;
}

  }
}