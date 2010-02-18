#include"wavefile.h"

#include"../../auxiliary/macro.h"

#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/string.h"

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
bool WaveFile::Check( const void* pSrc, size_t size )
{
  //  2048なのは Open() がそのサイズあることを前提としているため
  if( size<2048 ) { return false; }

  const unt32* p = (unt32*)pSrc;

  //  かなり強引に判定する
  if( p[0]==StrToBinary32('R','I','F','F') && p[2]==StrToBinary32('W','A','V','E') ) { return true; }

  return false;
}


WaveFile::WaveFile( const void* pSrc, size_t size )
  :m_Length(0)
  ,m_BeginOffset(0)
  ,m_hFile( pSrc, size )
{
}

WaveFile::~WaveFile()
{
  Finalize();
}


WaveFile::INITIALIZERESULT WaveFile::Initialize()
{
  m_hFile.Open();

	std::vector<unt08> tmp(2048);	//	これで十分でしょう
	m_hFile.Read( &(tmp[0]), tmp.size() );

	if( (*(unt32*)(&tmp[0]))!=StrToBinary32('R','I','F','F') ||
		(*(unt32*)(&tmp[8]))!=StrToBinary32('W','A','V','E')  ) { MAID_WARNING( "waveファイルでありません"); return INITIALIZERESULT_ERROR; }

	unt32 sample=0;
	{
		unt08*	pNow = &(tmp[12]);
		unt32	dwOffset = 12;

		bool bfmt = false;
		bool bdata= false;
		while( true )
		{
			if( bfmt && bdata ) { break; }

			const unt32 type   = *((unt32*)pNow);
			const unt32 length = *((unt32*)(pNow+4));

			if( type==StrToBinary32('f','m','t',' ') )
			{
				unt08* pData  = pNow+8;
				memcpy( &m_WaveFormat, pData, sizeof(m_WaveFormat) );
				bfmt = true;
			}else if( type==StrToBinary32('d','a','t','a') )
			{
				sample   = length;
				m_BeginOffset = dwOffset+8;
				bdata = true;
			}

			pNow += length+8;
			dwOffset += length+8;
		}

	}

	m_Length = sample;
	m_hFile.Seek( IFileRead::POSITION_BEGIN, m_BeginOffset );

	return INITIALIZERESULT_OK;
}

void WaveFile::Finalize()
{
  m_hFile.Seek( IFileRead::POSITION_BEGIN, 0 );
  m_WaveFormat = FMTCHUNCK();
	m_Length      = 0;
	m_BeginOffset = 0;
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デコードする
/*!
 *	引数などは IPCMDecoder::Read を参照すること
 */
size_t WaveFile::Read( void* pDst, size_t dwSize )
{
	const size_t pos = GetPosition();
	const size_t len = GetLength();

	size_t Red;

	if( pos+dwSize < len )	{ Red = dwSize; }
	else	
	{
		Red = len - pos; 
	}

	return m_hFile.Read( pDst, Red );

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デコード開始位置を移動する
/*!
 *	引数などは IPCMDecoder::SetPosition を参照すること
 */
void WaveFile::SetPosition( size_t Offset )
{
	m_hFile.Seek( IFileRead::POSITION_BEGIN, m_BeginOffset+Offset );
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在の位置の取得
/*!
 *	引数などは IPCMDecoder::GetPosition を参照すること
 */
size_t	WaveFile::GetPosition()		const
{
	const unt32 FilePos = m_hFile.GetPosition();

	return (FilePos-m_BeginOffset);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンバート後のファイルの長さを取得する（バイト単位）
/*!
 *	引数などは IPCMDecoder::GetLength を参照すること
 */
size_t	WaveFile::GetLength()			const
{
	return m_Length;
}

PCMFORMAT WaveFile::GetFormat() const
{
  PCMFORMAT fmt;

  fmt.SamplesPerSecond = m_WaveFormat.SamplesPerSec;
  fmt.BitPerSamples = m_WaveFormat.BitsPerSample;
  fmt.Channels = m_WaveFormat.Channels;

  return fmt;
}

}
