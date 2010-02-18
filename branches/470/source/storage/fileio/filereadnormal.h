/*!
 	@file
 	@brief	通常のファイル読み込みクラス
\n			ライブラリはＣランタイムを使っています。
\n			このクラスを使うよりも CFileRead を使ったほうが便利です
 *
 */

#ifndef maid2_storage_fileio_filereadnormal_h
#define maid2_storage_fileio_filereadnormal_h



#include"../../config/define.h"
#include"../../auxiliary/string.h"
#include"ifileread.h"
#include<stdio.h>

namespace Maid
{
  class FileReadNormal : public IFileRead
  {
  public:
    FileReadNormal( const String& FileName );
    virtual ~FileReadNormal();

    virtual OPENRESULT  Open();
    virtual void        Close();
    virtual bool        IsOpen() const;

    virtual size_t Read( void* pData, size_t Size );
    virtual size_t GetSize()    const;
    virtual size_t GetPosition()const;

    virtual void Seek( IFileRead::POSITION Pos, int Size);

    const String& GetFileName() const;

  private:
    const String  m_FileName;


    FILE*   m_Handle;   //!<	ファイルハンドル
    size_t  m_Size;     //!<	オープンしたファイルのファイルサイズ（バイト単位）
    size_t  m_Position; //!<	現在のファイルポジション
  };

}

#endif
