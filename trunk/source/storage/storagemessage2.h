#ifndef maid2_storage_storagemessage2_h
#define maid2_storage_storagemessage2_h


#include"storagemessage.h"



namespace Maid
{
  //	描画スレッドに投げる　メッセージクラス

  namespace StorageMessage
  {
		class FileROpen : public Base
		{
		public:
			FileROpen() : Base(FILE_R_OPEN){}

			String	FileName;	//!<	作成するファイル名
		};

		class FileRRead : public Base
		{
		public:
			FileRRead() : Base(FILE_R_READ){}

      void*   pDst;
      size_t  Size;
		};

		class FileRSeek : public Base
		{
		public:
			FileRSeek() : Base(FILE_R_SEEK){}

      enum POSITION
      {
	      POSITION_BEGIN,		//!<	ファイルの先頭から
	      POSITION_END,		  //!<	ファイルの終端から
	      POSITION_CURRENT,	//!<	現在の位置から
      };

      POSITION  Pos;
      int       Size;      
		};

		class FileRClose : public Base
		{
		public:
			FileRClose() : Base(FILE_R_CLOSE){}
		};
  }
}



#endif
