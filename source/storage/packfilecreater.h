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
#include<list>

namespace Maid
{
  class PackFileCreater
  {
  public:
    struct FILEINFO
    {
      String SourceFile;
      bool IsCompress;

      FILEINFO()
        :IsCompress(true)
      {}

      FILEINFO( const String& str, bool comp )
        :SourceFile(str),IsCompress(comp)
      {}
    };

    void BeginArchive( const String& ArchiveName, unt64 DivSize, const String& RootDirectory, const std::vector<FILEINFO>& FileInfo );


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

      STATUS()
        :Progress(0)
      {}
    };

    void PopStatus( std::list<STATUS>& st );
    void OnCancel();

  private:
    unt ThreadFunction(volatile ThreadController::BRIGEDATA& brige);

    void AddExecutingText( const String& text );
    void AddErrorText( const String& text );
    FILE* CreateArchiveFile( int count ); 

    std::string CalcMD5( const std::vector<unt08>& src ) const;

  private:
    std::list<STATUS>  m_StatusList;

    String  m_ArchiveName;
    String  m_RootDirectory;
    std::vector<FILEINFO> m_FileInfo;

    unt64   m_DivSize;

    ThreadMutex       m_StatusMutex;
    ThreadController  m_Thread;
  };
}
#endif
