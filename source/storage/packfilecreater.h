/*!	
	@file
	@brief	指定した複数のファイルを統合して１つにするクラス
*/

#ifndef maid2_storage_packfilecreater_h
#define maid2_storage_packfilecreater_h


#include"../config/define.h"
#include"../config/typedef.h"
#include"../auxiliary/thread.h"

#include<vector>

namespace Maid
{
  class PackFileCreater
  {
  public:
    struct FILEINFO
    {
      String SourceFile;
      bool IsCompress;
    };

    void BeginArchive( const String& ArchiveName, const std::vector<FILEINFO>& FileInfo );


    struct STATUS
    {
      enum STATE
      {
        STATE_EXECUTING,//!<	実行中
        STATE_ERROR,    //!<	エラーが起きた
        STATE_SUCCESS,  //!<	成功して終わった
        STATE_CANCEL,   //!<	キャンセル命令がでて終了した
      };

      STATE   State;    //!<	現在の処理状態
      String  Text;     //!<	現在の状態を表すテキスト
      int     Progress; //!<	進行状況(0-100)
    };

    STATUS GetStatus();
    void   OnCancel();

  private:
    unt ThreadFunction(volatile ThreadController::BRIGEDATA& brige);

  private:
    STATUS  m_Status;

    String  m_ArchiveName;
    std::vector<FILEINFO> m_FileInfo;

    ThreadMutex       m_StatusMutex;
    ThreadController  m_Thread;
  };
}
#endif
