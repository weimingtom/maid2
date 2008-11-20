/*!
 	@file 
 	@brief	ファイルの特定区間を１ファイルとして扱うクラス
 */

#ifndef maid2_storage_fileio_filereadrange_h
#define maid2_storage_fileio_filereadrange_h



#include"../../config/define.h"
#include"../../auxiliary/string.h"
#include"ifileread.h"
#include"filereadnormal.h"
#include<stdio.h>

namespace Maid
{
	class FileReadRange : public IFileRead
	{
	public:
		FileReadRange();
		virtual ~FileReadRange();

    enum OPENRESULT
    {
      OPENRESULT_OK =0,
      OPENRESULT_ERROR,
    };
    OPENRESULT      Open( const String& FileName, size_t BeginPosition, size_t Size );
    virtual size_t Read( void* pData, size_t Size );
    virtual size_t GetSize()    const;
    virtual size_t GetPosition()const;

    virtual void Seek( IFileRead::POSITION Pos, int Size);
    virtual void Close();

	private:
		FileReadNormal m_hFile;
		size_t		m_BeginPosition;
		size_t		m_Size;
	};

}


#endif
