#include"vorbisfile.h"

#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"

#include<vector>

namespace Maid
{

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! このクラスで開けるか？
/*!
    @param  pSrc  [i ]  調べるデータ
    @param  size  [i ]  データの大きさ

    @return 開けるなら true
            開けないなら false
 */
bool VorbisFile::Check( const void* pSrc, size_t size )
{
	ov_callbacks callbacks = {
		fread_emulation,
		fseek_emulation,
		fclose_emulation,
		ftell_emulation
	};

  FileReadMemory  hFile(pSrc,size);
  OggVorbis_File vorbis = {0};

	hFile.Open();

  const int ret = ov_test_callbacks(&hFile, &vorbis, NULL, 0, callbacks);
	ov_clear(&vorbis);

  return ret==0;
}


VorbisFile::VorbisFile( const void* pSrc, size_t size )
  :m_hFile(pSrc,size)
{
  ZERO( &m_VorbisFile, sizeof(m_VorbisFile) );
}

VorbisFile::~VorbisFile()
{
  Finalize();
}



VorbisFile::INITIALIZERESULT VorbisFile::Initialize()
{
	Finalize();

	m_hFile.Open();


  {
	  ov_callbacks callbacks = {
		  fread_emulation,
		  fseek_emulation,
		  fclose_emulation,
		  ftell_emulation
	  };
	  // ファイルはOggVorbis形式かな？
	  const int ret = ov_open_callbacks(&m_hFile, &m_VorbisFile, NULL, 0, callbacks);
    if( ret!=0 )
    {
      MAID_WARNING( "ov_open_callbacks()" );
      return INITIALIZERESULT_ERROR;
    }
  }

  {
	  // シークできないと困る
	  const int ret = ov_seekable(&m_VorbisFile);
    if( ret==0 )
    {
      MAID_WARNING( "ov_seekable()" );
      return INITIALIZERESULT_ERROR;
    }
  }

	return INITIALIZERESULT_OK;
}


void	VorbisFile::Finalize()
{
	ov_clear(&m_VorbisFile);
  m_hFile.Close();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デコードする
/*!
 *	引数などは IPCMDecoder::Read を参照すること
 */
size_t VorbisFile::Read( void* pDst, size_t dwSize )
{
	size_t          read = 0;
	OggVorbis_File* pFile = &m_VorbisFile;

	const size_t len = dwSize;

	while(read<len)
	{
		const long ret = ov_read( pFile, (char*)pDst+read, len-read, 0, 2, 1, NULL);

    if( ret==OV_HOLE     ) { MAID_WARNING( "OV_HOLE" ); }
    else if( ret==OV_EBADLINK ) { MAID_WARNING( "OV_EBADLINK" ); }

		read += ret;
		if( ret==0 ) { break; }
	}

	return read;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デコード開始位置を移動する
/*!
 *	引数などは IPCMDecoder::SetPosition を参照すること
 */
void VorbisFile::SetPosition( size_t Offset )
{
  const PCMFORMAT fmt = GetFormat();
  const ogg_int64_t pos = Offset / fmt.CalcBlockSize();

	ov_pcm_seek( &m_VorbisFile, pos );
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在の位置の取得
/*!
 *	引数などは IPCMDecoder::GetPosition を参照すること
 */
size_t	VorbisFile::GetPosition()		const
{
	OggVorbis_File* pFile = const_cast<OggVorbis_File*>(&m_VorbisFile);

  const PCMFORMAT fmt = GetFormat();

	const size_t sample_size = fmt.CalcBlockSize();

	const ogg_int64_t sample_pos = ov_pcm_tell( pFile );
	return (size_t)sample_pos * sample_size;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンバート後のファイルの長さを取得する（バイト単位）
/*!
 *	引数などは IPCMDecoder::GetLength を参照すること
 */
size_t	VorbisFile::GetLength()			const
{
	OggVorbis_File* pFile = const_cast<OggVorbis_File*>(&m_VorbisFile);

	const size_t sample_pos = (size_t)ov_pcm_total( pFile, -1 );
  const PCMFORMAT fmt = GetFormat();

	return sample_pos * fmt.CalcBlockSize();
}

PCMFORMAT VorbisFile::GetFormat() const
{
	OggVorbis_File* pFile = const_cast<OggVorbis_File*>(&m_VorbisFile);

  PCMFORMAT fmt;

  fmt.SamplesPerSecond =  ov_info(pFile, -1)->rate;
  fmt.BitPerSamples = 16;	//	ogg は16固定
  fmt.Channels = ov_info(pFile, -1)->channels & 0xffff;

  return fmt;
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! <stdio.h> の fread() のエミュレーション
/*!
 *	fread は実際に読み出した全項目の数を返します。\n
 *	この数が count より小さい場合は、エラーが発生したか、\n
 *	count 数を読み出す前にファイルの終端に達しているはずです。\n
 *	size または count を 0 にすると、fread 関数は 0 を返します。バッファの内容は変わりません。
 *
 *	\param  buffer		[i ]	データの格納場所
 *	\param  size		[i ]	バイト単位の項目サイズ
 *	\param  count		[i ]	読み出す最大項目数
 *	\param  p			[i ]	VorbisFile* です。キャストして使う
 *
 *	\return 読み込んだサイズ（バイト単位）
 */
size_t VorbisFile::fread_emulation(void* buffer, size_t size, size_t count, void *p)
{
	FileReadMemory* pFile = (FileReadMemory*)p;
	if(p==NULL)return 0;

	const int ret = pFile->Read( buffer, unt32(size*count) );

	return ret;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! <stdio.h> の fseek() のエミュレーション
/*!
 *	指定された位置にファイル ポインタを移動します
 *
 *	\param  p			[i ]	VorbisFile* です。キャストして使う
 *	\param  offset		[i ]	origin からのバイト数
 *	\param  origin		[i ]	初期位置
 *
 *
 *	\return 関数の成功で０
 *			関数の失敗で０以外
 */
int VorbisFile::fseek_emulation(void* p, ogg_int64_t offset, int origin)
{
	if(p==NULL)return -1;

	FileReadMemory* pFile = (FileReadMemory*)p;
	switch(origin){
	case SEEK_CUR: { pFile->Seek( IFileRead::POSITION_CURRENT, int(offset)	); }break;
	case SEEK_END: { pFile->Seek( IFileRead::POSITION_END, int(offset)		); }break;
	case SEEK_SET: { pFile->Seek( IFileRead::POSITION_BEGIN, int(offset)	); }break;
	default:
		return -1;
	}

	return 0;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! <stdio.h> の fclose() のエミュレーション
/*!
 *	指定したストリームを閉じます
 *
 *	\param  p			[i ]	VorbisFile* です。キャストして使う
 *
 *	\return 関数の成功で０
 *			関数の失敗で０以外
 */
int VorbisFile::fclose_emulation(void* p)
{
  if(p==NULL) { return(EOF); }

	FileReadMemory* pFile = (FileReadMemory*)p;
	pFile->Close();

	return 0;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! <stdio.h> の ftell() のエミュレーション
/*!
 *	ファイル ポインタの現在位置を得ます
 *
 *	\param  p			[i ]	VorbisFile* です。キャストして使う
 *
 *	\return 現在のファイル位置
 */
long VorbisFile::ftell_emulation(void* p)
{
	if(p==NULL) return -1;

	FileReadMemory* pFile = (FileReadMemory*)p;

	const long pos = pFile->GetPosition();

	return pos;
}

}
