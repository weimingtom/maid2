#include"packfilecreater.h"
#include"packfileinfo.h"

//#include<boost/filesystem/path.hpp>

namespace Maid
{

void PackFileCreater::BeginArchive( const String& ArchiveName, const std::vector<FILEINFO>& FileInfo )
{
  m_ArchiveName = ArchiveName;
  m_FileInfo = FileInfo;

  m_Thread.SetFunc( MakeThreadObject(&PackFileCreater::ThreadFunction,this) );
  m_Thread.Execute();

}

PackFileCreater::STATUS PackFileCreater::GetStatus()
{

  return m_Status;
}

void   PackFileCreater::OnCancel()
{

}


unt PackFileCreater::ThreadFunction(volatile ThreadController::BRIGEDATA& brige)
{
  //  作業フォルダの作成

  //  作業フォルダにコピー＆圧縮していく

  //  統合する

  //  ヘッダを作る

  //  ヘッダの圧縮

  //  分割数を求めて、分割する

  return 0;
}

/*
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
*/

}