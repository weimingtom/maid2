#ifndef maid2_sound_decoder_riffpcm_h
#define maid2_sound_decoder_riffpcm_h 

#include"../../config/define.h"
#include"../../config/typedef.h"
#include"ipcmdecoder.h"

#include"../../storage/fileio/filereadmemory.h"

namespace Maid
{
  class RiffPCM : public IPCMDecoder
  {
  public:
    OPENRESULT Open( const void* pSrc, size_t size );
    void    Close();
    size_t  Read( void* pDst, size_t size );
    void    SetPosition( size_t Offset );
    size_t  GetPosition()		const;
    size_t  GetLength()			const;

    PCMFORMAT GetFormat() const;

    static bool Check( const void* pSrc, size_t size );

  private:
    FileReadMemory	m_hFile;

    struct FMTCHUNCK
    {
      unt16 FormatTag;
      unt16 Channels;
      unt32 SamplesPerSec;
      unt32 AvgBytesPerSec;
      unt16 BlockAlign;
      unt16 BitsPerSample;
      unt16 ExtraSize;
    };

    FMTCHUNCK	m_WaveFormat;   //!<	読み込んだファイルフォーマット
    size_t    m_Length;       //!<	PCMデータの長さ（バイト単位）
    size_t    m_BeginOffset;  //!<	PCMデータの先頭オフセット
  };
}

#endif