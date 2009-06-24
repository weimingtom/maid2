/*!
  @file 
  @brief	メモリ上においてあるデータをファイルとして扱うクラス
 */

#ifndef maid2_storage_fileio_filereadmemory_h
#define maid2_storage_fileio_filereadmemory_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"ifileread.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  class FileReadMemory : public IFileRead
  {
  public:
    FileReadMemory( const boost::shared_array<unt08>& pBegin, size_t Size );
    FileReadMemory( const void* pBegin, size_t Size );
    virtual ~FileReadMemory();

    virtual OPENRESULT  Open();
    virtual void        Close();
    virtual bool        IsOpen() const;

    virtual size_t  Read ( void* pData, size_t Size );
    virtual size_t  GetSize()    const;
    virtual size_t  GetPosition()const;

    virtual void Seek( IFileRead::POSITION Pos, int Size);

  private:
    boost::shared_array<unt08> m_pShared;
    const size_t  m_Size;
    const unt08*  m_pBegin;

    size_t  m_Position;
    bool    m_IsOpen;
  };

}


#endif
