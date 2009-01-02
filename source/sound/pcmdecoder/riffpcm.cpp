#include"riffpcm.h"

#include"../../auxiliary/macro.h"

#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/string.h"

#include<vector>

namespace Maid
{

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルのロード
/*!
 *	引数などは IPCMDecoder::Open を参照すること
 */
IPCMDecoder::OPENRESULT RiffPCM::Open( const void* pSrc, size_t size )
{
	Close();

  m_hFile.OpenNoOwner( pSrc, size );

	std::vector<unt08> tmp(2048);	//	これで十分でしょう
	m_hFile.Read( &(tmp[0]), tmp.size() );

	if( (*(unt32*)(&tmp[0]))!=StrToBinary32('R','I','F','F') ||
		(*(unt32*)(&tmp[8]))!=StrToBinary32('W','A','V','E')  ) { MAID_WARNING( "waveファイルでありません"); return OPENRESULT_ERROR; }

	unt32 sample=0;
	{
		unt08*	pNow = &(tmp[12]);
		unt32	dwOffset = 12;

		bool bfmt = false;
		bool bdata= false;
		while( true )
		{
			if( bfmt && bdata ) { break; }

			unt32 type   = *((unt32*)pNow);
			unt32 length = *((unt32*)(pNow+4));

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

	return OPENRESULT_OK;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 読み込んだファイルの解放
/*!
 *	引数などは IPCMDecoder::Close を参照すること
 */
void	RiffPCM::Close()
{
	m_hFile.Close();
	ZERO( &m_WaveFormat, sizeof(m_WaveFormat) );
	m_Length      = 0;
	m_BeginOffset = 0;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デコードする
/*!
 *	引数などは IPCMDecoder::Read を参照すること
 */
size_t RiffPCM::Read( void* pDst, size_t dwSize )
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
void RiffPCM::SetPosition( size_t Offset )
{
	m_hFile.Seek( IFileRead::POSITION_BEGIN, m_BeginOffset+Offset );
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在の位置の取得
/*!
 *	引数などは IPCMDecoder::GetPosition を参照すること
 */
size_t	RiffPCM::GetPosition()		const
{
	const unt32 FilePos = m_hFile.GetPosition();

	return (FilePos-m_BeginOffset);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンバート後のファイルの長さを取得する（サンプル単位）
/*!
 *	引数などは IPCMDecoder::GetLength を参照すること
 */
size_t	RiffPCM::GetLength()			const
{
	return m_Length;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! １秒あたり何サンプルあるかの取得
/*!
 *	引数などは IPCMDecoder::GetSamplesPerSec を参照すること
 */
size_t	RiffPCM::GetSamplesPerSec()	const
{
	return m_WaveFormat.SamplesPerSec;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! チャンネル数(mono,stereo 等)の取得
/*!
 *	引数などは IPCMDecoder::GetChannels を参照すること
 */
size_t	RiffPCM::GetChannels()		const
{
	return m_WaveFormat.Channels;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! １サンプルあたりに使用しているビット量の取得
/*!
 *	引数などは IPCMDecoder::GetBitsPerBlock を参照すること
 */
size_t	RiffPCM::GetBitsPerBlock()	const
{
	return m_WaveFormat.BitsPerSample;
}

}
