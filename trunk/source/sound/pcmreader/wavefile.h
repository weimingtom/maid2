#ifndef maid2_sound_pcmreader_wavefile_h
#define maid2_sound_pcmreader_wavefile_h 

#include"../../config/define.h"
#include"../../config/typedef.h"
#include"ipcmreader.h"

#include"../../storage/fileio/filereadmemory.h"

namespace Maid
{
  class WaveFile : public IPCMReader
  {
  public:
    WaveFile( const void* pSrc, size_t size );
    ~WaveFile();

    INITIALIZERESULT Initialize();
    void Finalize();

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

      FMTCHUNCK()
        :FormatTag(0)
        ,Channels(0)
        ,SamplesPerSec(0)
        ,AvgBytesPerSec(0)
        ,BlockAlign(0)
        ,BitsPerSample(0)
        ,ExtraSize(0)
      {}
    };

    FMTCHUNCK	m_WaveFormat;   //!<	読み込んだファイルフォーマット
    size_t    m_Length;       //!<	PCMデータの長さ（バイト単位）
    size_t    m_BeginOffset;  //!<	PCMデータの先頭オフセット
  };
}

#endif