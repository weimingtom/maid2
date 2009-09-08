#include"storage.h"
#include"storagelocal.h"

#include"storagemessage1.h"
#include"storagemessage2.h"

#include"fileio/filereadnormal.h"
#include"../auxiliary/debug/warning.h"

#include"fileio/fileoperation.h"


namespace Maid
{

using namespace StorageMessage;

unt Storage::ThreadFunction( volatile ThreadController::BRIGEDATA& brige )
{
  while( true )
  {
    if( brige.IsExit ) 
    {
       ThreadMutexLocker Lock(m_MessageMutex);
       if( m_MessageQue.empty() ) { break; }
    }

    STORAGEMESSAGEQUE Mess;
    { //	とりあえず溜まっているメッセージを拾う
      ThreadMutexLocker Lock(m_MessageMutex);
      Mess.swap(m_MessageQue);
    }

    if( Mess.empty() )
    {
      //	仕事なかったら休憩
      ThreadController::Sleep( 1 );
      ++m_SleepCount;
      continue;
    }

    m_SleepCount = 0;


    //  メッセージを順番に処理
    for( int i=0; i<(int)Mess.size(); ++i )
    {
      const SPSTORAGEMESSAGE& pMess = Mess[i];

      ExecuteMessage( pMess );
    }
    {
      //  処理したメッセージを減らす
      ThreadMutexLocker lock(m_MessageMutex);
      for( int i=0; i<(int)Mess.size(); ++i )
      {
        const SPSTORAGEMESSAGE& pMess = Mess[i];
        pMess->pObject->m_MessageCount -= 1;
      }
    }

  }

  return 0;
}


void Storage::ExecuteMessage( const SPSTORAGEMESSAGE& pMess )
{
  switch( pMess->Message )
  {
  case Base::NONE: { }break;


  /////////////////////////////////// 書き込み編
  case Base::FILE_W_CREATE:
    {
      FileWCreate* p = static_cast<FileWCreate*>(pMess.get());
      StorageObjectFileWriter* pWriter = static_cast<StorageObjectFileWriter*>(pMess->pObject.get());

      pWriter->Writer.Open( p->FileName, FileWrite::OPENOPTION_NEW );
      pWriter->m_Position = 0;
      pWriter->m_Size     = 0;
    }break;

  case Base::FILE_W_OPEN:
    {
      StorageObjectFileWriter* pWriter = static_cast<StorageObjectFileWriter*>(pMess->pObject.get());
      FileWOpen* p = static_cast<FileWOpen*>(pMess.get());

      pWriter->Writer.Open( p->FileName, FileWrite::OPENOPTION_OPEN );
      pWriter->m_Position = 0;
      pWriter->m_Size     = 0;
    }break;

  case Base::FILE_W_ADD: 
    {
      StorageObjectFileWriter* pWriter = static_cast<StorageObjectFileWriter*>(pMess->pObject.get());
      FileWAdd* p = static_cast<FileWAdd*>(pMess.get());

      pWriter->Writer.Open( p->FileName, FileWrite::OPENOPTION_ADD );
      pWriter->m_Position = 0;
      pWriter->m_Size     = 0;
    }break;

  case Base::FILE_W_WRITE: 
    {
      StorageObjectFileWriter* pWriter = static_cast<StorageObjectFileWriter*>(pMess->pObject.get());
      FileWWrite* p = static_cast<FileWWrite*>(pMess.get());

      pWriter->Writer.Write( &(p->Buffer[0]), p->Buffer.size() );
      pWriter->m_Position = pWriter->Writer.GetPosition();
      pWriter->m_Size     = pWriter->Writer.GetSize();
    }break;

  case Base::FILE_W_SEEK: 
    {
      StorageObjectFileWriter* pWriter = static_cast<StorageObjectFileWriter*>(pMess->pObject.get());
      FileWSeek* p = static_cast<FileWSeek*>(pMess.get());

      FileWrite::POSITION pos;
      switch( p->Pos )
      {
      case FileWSeek::POSITION_BEGIN:   { pos = FileWrite::POSITION_BEGIN;    }break;
      case FileWSeek::POSITION_END:     { pos = FileWrite::POSITION_END;      }break;
      case FileWSeek::POSITION_CURRENT: { pos = FileWrite::POSITION_CURRENT;  }break;
      }
      pWriter->Writer.Seek( pos, p->Size );
      pWriter->m_Position = pWriter->Writer.GetPosition();
    }break;

  case Base::FILE_W_CLOSE:
    {
      StorageObjectFileWriter* pWriter = static_cast<StorageObjectFileWriter*>(pMess->pObject.get());
      FileWClose* p = static_cast<FileWClose*>(pMess.get());
      pWriter->Writer.Close();
      pWriter->m_Position = 0;
      pWriter->m_Size     = 0;
    }break;



  /////////////////////////////////// 読み込み編
  case Base::FILE_R_OPEN:
    {
      StorageObjectFileReader* pReader = static_cast<StorageObjectFileReader*>(pMess->pObject.get());
      FileROpen* pArg = static_cast<FileROpen*>(pMess.get());

      SPFILEREAD pRead = OpenFileRead( pArg->FileName );

      pReader->m_Position = 0;
      pReader->m_Size     = pRead->GetSize();

      pReader->pReader = pRead;
    }break;

  case Base::FILE_R_READ: 
    {
      StorageObjectFileReader* pReader = static_cast<StorageObjectFileReader*>(pMess->pObject.get());
      FileRRead* p = static_cast<FileRRead*>(pMess.get());

      pReader->pReader->Read( p->pDst, p->Size );
      pReader->m_Position = pReader->pReader->GetPosition();
      pReader->m_Size     = pReader->pReader->GetSize();
    }break;

  case Base::FILE_R_SEEK: 
    {
      StorageObjectFileReader* pReader = static_cast<StorageObjectFileReader*>(pMess->pObject.get());
      FileRSeek* p = static_cast<FileRSeek*>(pMess.get());

      IFileRead::POSITION pos;
      switch( p->Pos )
      {
      case FileRSeek::POSITION_BEGIN:   { pos = IFileRead::POSITION_BEGIN;    }break;
      case FileRSeek::POSITION_END:     { pos = IFileRead::POSITION_END;      }break;
      case FileRSeek::POSITION_CURRENT: { pos = IFileRead::POSITION_CURRENT;  }break;
      }
      pReader->pReader->Seek( pos, p->Size );
      pReader->m_Position = pReader->pReader->GetPosition();
    }break;

  case Base::FILE_R_CLOSE:
    {
      StorageObjectFileReader* pReader = static_cast<StorageObjectFileReader*>(pMess->pObject.get());
      FileRClose* p = static_cast<FileRClose*>(pMess.get());
      pReader->pReader->Close();
      pReader->pReader.reset();
      pReader->m_Position = 0;
      pReader->m_Size     = 0;
    }break;


  case Base::FILE_R_OPENREAD:
    {
      StorageObjectFileReader* pReader = static_cast<StorageObjectFileReader*>(pMess->pObject.get());
      FileROpenRead* pArg = static_cast<FileROpenRead*>(pMess.get());

      SPFILEREAD pFile = OpenFileRead( pArg->FileName );

      if( pFile.get()!=NULL )
      {
        const size_t size = pFile->GetSize();

        pArg->pBuffer->Resize( size );
        pFile->Read( pArg->pBuffer->GetPointer(0), size );
      }
    }break;


  case Base::FILE_MOUNT:
    {
      FileMount* p = static_cast<FileMount*>(pMess.get());
      m_PackFileReader.Mount( p->FileName );
    }break;

  case Base::FILE_UNMOUNT:
    {
      FileUnMount* p = static_cast<FileUnMount*>(pMess.get());
      m_PackFileReader.UnMount( p->FileName );
    }break;
  }
}


SPFILEREAD Storage::OpenFileRead( const String& FileName )
{
  //  アーカイブより普通のファイルがあるならそっちを優先
  boost::shared_ptr<IFileRead> pRet;
  if ( FileOperation::IsFileExist(FileName) )
  {
    pRet.reset( new FileReadNormal(FileName) );
  }else
  {
    pRet = m_PackFileReader.FindFile(FileName);
  }

  MAID_ASSERT( pRet.get()==NULL, "ファイルが見つかりません" << FileName );


  const IFileRead::OPENRESULT ret = pRet->Open();

  if( ret==IFileRead::OPENRESULT_ERROR ) { MAID_WARNING( "ファイルオープンに失敗 " << FileName ); }

  return pRet;
}


}
