/*!
  @file 
  @brief	メモリ上においてあるデータをファイルとして扱うクラス
 */

#ifndef maid2_storage_fileio_filereadmemory_h
#define maid2_storage_fileio_filereadmemory_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"filereadinterface.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
	class FileReadMemory : public FileRead
	{
	public:
		FileReadMemory();
		virtual ~FileReadMemory();

				void Open( const boost::shared_array<unt08>& pBegin, size_t Size );
				void OpenNoOwner( const void* pBegin, size_t Size );
		virtual size_t	 Read ( void* pData, size_t Size );
		virtual size_t	 GetSize()    const;
		virtual size_t	 GetPosition()const;

    virtual void Seek( FileRead::POSITION Pos, int Size);

		virtual void Close();

	private:
		boost::shared_array<unt08> m_pShared;
		size_t  m_Size;
		size_t  m_Position;
		unt08*  m_pBegin;
	};

}


#endif
