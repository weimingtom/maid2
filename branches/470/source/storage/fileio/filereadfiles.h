/*!
 	@file 
 	@brief	複数ファイルを１ファイルとして扱うクラス
 */

#ifndef maid2_storage_fileio_filereadfiles_h
#define maid2_storage_fileio_filereadfiles_h



#include"../../config/define.h"
#include"../../auxiliary/string.h"
#include"ifileread.h"
#include"filereadnormal.h"
#include<vector>

namespace Maid
{
  class FileReadFiles : public IFileRead
  {
  public:

    struct ELEMENT
    {
      String  FileName;
      size_t  BeginPosition;
      size_t  Size;

      ELEMENT(){}
      ELEMENT( const String& str, size_t b, size_t s )
        :FileName(str), BeginPosition(b), Size(s)
      {}
    };


    FileReadFiles( const std::vector<ELEMENT>& dat );
    virtual ~FileReadFiles();

    virtual OPENRESULT  Open();
    virtual void        Close();
    virtual bool        IsOpen() const;

    virtual size_t Read( void* pData, size_t Size );
    virtual size_t GetSize()    const;
    virtual size_t GetPosition()const;

    virtual void Seek( IFileRead::POSITION Pos, int Size );

  private:
    OPENRESULT FileOpen( const ELEMENT& element );

  private:
    const std::vector<ELEMENT> m_Data;
    FILE*   m_Handle;
    size_t  m_Position; //  ファイルを連結したときの合計位置
    size_t  m_ReadSize; //  m_Handle 内で読み取れる残りのバイト数

    enum
    {
      OPENINDEX_NONE = ~0,  //  ファイルを開いていない
    };
    int m_OpenIndex;  //  現在何個目のファイルを開いているか？
  };

}


#endif
