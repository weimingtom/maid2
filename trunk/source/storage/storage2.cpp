#include"storage.h"
#include"storagelocal.h"

#include"storagemessage1.h"
#include"storagemessage2.h"

#include"fileio/filereadnormal.h"

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
    {	//	とりあえず溜まっているメッセージを拾う
      ThreadMutexLocker Lock(m_MessageMutex);
      Mess.swap(m_MessageQue);
    }


    //	メッセージを順番に処理
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
        pMess->pObject->MessageCount -= 1;
      }
    }

    //	一仕事終わったのでしばし休憩
    ThreadController::Sleep( 1 );
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
    }break;

  case Base::FILE_W_OPEN:
    {
      StorageObjectFileWriter* pWriter = static_cast<StorageObjectFileWriter*>(pMess->pObject.get());
      FileWOpen* p = static_cast<FileWOpen*>(pMess.get());

      pWriter->Writer.Open( p->FileName, FileWrite::OPENOPTION_OPEN );
    }break;

  case Base::FILE_W_ADD: 
    {
      StorageObjectFileWriter* pWriter = static_cast<StorageObjectFileWriter*>(pMess->pObject.get());
      FileWAdd* p = static_cast<FileWAdd*>(pMess.get());

      pWriter->Writer.Open( p->FileName, FileWrite::OPENOPTION_ADD );
    }break;

  case Base::FILE_W_WRITE: 
    {
      StorageObjectFileWriter* pWriter = static_cast<StorageObjectFileWriter*>(pMess->pObject.get());
      FileWWrite* p = static_cast<FileWWrite*>(pMess.get());

      pWriter->Writer.Write( &(p->Buffer[0]), p->Buffer.size() );
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
    }break;

  case Base::FILE_W_CLOSE:
    {
      StorageObjectFileWriter* pWriter = static_cast<StorageObjectFileWriter*>(pMess->pObject.get());
      FileWClose* p = static_cast<FileWClose*>(pMess.get());
      pWriter->Writer.Close();
    }break;



  /////////////////////////////////// 読み込み編
  case Base::FILE_R_OPEN:
    {
      StorageObjectFileReader* pReader = static_cast<StorageObjectFileReader*>(pMess->pObject.get());
      FileROpen* pArg = static_cast<FileROpen*>(pMess.get());

      //  ここでパックファイルの検索とかやる。
      //  当面はスルーするけど
      SPFILEREAD pRead;
      {
        boost::shared_ptr<FileReadNormal> pp( new FileReadNormal );
        pp->Open( pArg->FileName );
        pRead = pp;
      }

      pReader->pReader = pRead;

    }break;

  case Base::FILE_R_READ: 
    {
      StorageObjectFileReader* pReader = static_cast<StorageObjectFileReader*>(pMess->pObject.get());
      FileRRead* p = static_cast<FileRRead*>(pMess.get());

      pReader->pReader->Read( p->pDst, p->Size );
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
    }break;

  case Base::FILE_R_CLOSE:
    {
      StorageObjectFileReader* pReader = static_cast<StorageObjectFileReader*>(pMess->pObject.get());
      FileRClose* p = static_cast<FileRClose*>(pMess.get());
      pReader->pReader->Close();
    }break;

  }
}

}
