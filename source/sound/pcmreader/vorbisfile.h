#ifndef maid2_sound_pcmreader_vorbisfile_h
#define maid2_sound_pcmreader_vorbisfile_h 

#include"../../config/define.h"
#include"../../config/typedef.h"
#include"ipcmreader.h"

#include"../../storage/fileio/filereadmemory.h"
#include<vorbis/vorbisfile.h>

namespace Maid
{
  class VorbisFile : public IPCMReader
  {
  public:
    VorbisFile( const void* pSrc, size_t size );
    ~VorbisFile();

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

		OggVorbis_File	m_VorbisFile;

	private:
		static size_t	fread_emulation(void*, size_t, size_t, void*);
		static int		fseek_emulation(void*, ogg_int64_t, int);
		static int		fclose_emulation(void*);
		static long		ftell_emulation(void*);
  };
}

#endif