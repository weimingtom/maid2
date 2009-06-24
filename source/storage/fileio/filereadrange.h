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
    FileReadRange( const String& FileName, size_t BeginPosition, size_t Size );
    virtual ~FileReadRange();

    virtual OPENRESULT  Open();
    virtual void        Close();
    virtual bool        IsOpen() const;

    virtual size_t Read( void* pData, size_t Size );
    virtual size_t GetSize()    const;
    virtual size_t GetPosition()const;

    virtual void Seek( IFileRead::POSITION Pos, int Size);

  private:
    const size_t		m_BeginPosition;
    const size_t		m_Size;
    FileReadNormal  m_hFile;
  };

}


#endif
