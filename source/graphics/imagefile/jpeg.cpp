#include"jpeg.h"

extern "C" {
#define XMD_H
#include"../../import/ijg/jpeglib.h"
}

#include"../../auxiliary/debug/warning.h"




// �g�����́@http://www.syuhitu.org/other/jpeg/jpeg.html�@���Q��

namespace Maid
{
  namespace Jpeg
  {

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �w�肵���f�[�^���r�b�g�}�b�v�����ׂ�
/*!
    @param	FileImage	[i ]  �f�[�^

    @return �r�b�g�}�b�v�Ȃ�true
            �Ⴄ�Ȃ�false
*/
bool  Check( const std::vector<unt08>& FileImage )
{
  if( FileImage.size()<2 ) { return false; }

	return FileImage[0]==0xFF &&  FileImage[1]==0xD8;
}




	//	libjpeg ������������g����悤�ɂ��郉�b�p


/* �������\�[�X�����JPEG�W�J�p�}�l�[�W�� */
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
//! �r�b�g�}�b�v��ϊ�����
/*!
    @param	FileImage	[i ]  �t�@�C����
    @param	dst       [i ]  �]����
 */
FUCTIONRESULT   Load( const std::vector<unt08>& FileImage, Surface& surface )
{

	my_error_mgr jerr;
	jpeg_decompress_struct cinfo;

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	jpeg_create_decompress(&cinfo);

	jpeg_memory_src(&cinfo, &(FileImage[0]), (int)FileImage.size() );

	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	const int row_stride = cinfo.output_width * cinfo.output_components;

	const SIZE2DI size(cinfo.output_width, cinfo.output_height);
	PIXELFORMAT fmt = PIXELFORMAT_B08G08R08I;

	if( cinfo.out_color_components==3 )
	{	//	�O���[�X�P�[���Ȃ� cinfo.out_color_components==1 
		//	���̂Ƃ��ɂ� cinfo.colormap ���ݒ肳���
		fmt = PIXELFORMAT_B08G08R08I;
	}else
	{
		MAID_ASSERT( true, "���Ή�" );
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

  return FUCTIONRESULT_OK;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! �ǂݍ���ł���T�[�t�F�X���r�b�g�}�b�v�Ƃ��ĕۑ�����
/*!
 */
FUCTIONRESULT Save( const Surface& src, std::vector<unt08>& FileImage )
{
  MAID_ASSERT( true, "������" );
  return FUCTIONRESULT_OK;
}

  }
}