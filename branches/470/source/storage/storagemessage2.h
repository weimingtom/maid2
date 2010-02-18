#ifndef maid2_storage_storagemessage2_h
#define maid2_storage_storagemessage2_h


#include"storagemessagebase.h"
#include"../auxiliary/memorybuffer.h"



namespace Maid
{
  //  描画スレッドに投げる　メッセージクラス

  namespace StorageMessage
  {
    struct FileROpen : public Base
    {
      FileROpen() : Base(FILE_R_OPEN){}

      String  FileName; //!<  作成するファイル名
    };

    struct FileRRead : public Base
    {
      FileRRead() : Base(FILE_R_READ){}

      void*   pDst;
      size_t  Size;
    };

    struct FileROpenRead : public Base
    {
      FileROpenRead() : Base(FILE_R_OPENREAD){}

      String  FileName; //!<  作成するファイル名
      SPMEMORYBUFFER pBuffer;
    };

    struct FileRSeek : public Base
    {
      FileRSeek() : Base(FILE_R_SEEK){}

      enum POSITION
      {
        POSITION_BEGIN,   //!<  ファイルの先頭から
        POSITION_END,     //!<  ファイルの終端から
        POSITION_CURRENT, //!<  現在の位置から
      };

      POSITION  Pos;
      int       Size;
    };

    struct FileRClose : public Base
    {
      FileRClose() : Base(FILE_R_CLOSE){}
    };

    struct FileMount : public Base
    {
      FileMount() : Base(FILE_MOUNT){}
      String  FileName;
    };

    struct FileUnMount : public Base
    {
      FileUnMount() : Base(FILE_UNMOUNT){}
      String  FileName;
    };
  }
}



#endif
