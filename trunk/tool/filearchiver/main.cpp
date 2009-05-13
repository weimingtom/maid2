#include"../../source/framework/application.h"
#include"../../source/framework/win32/shell.h"
#include"../../source/storage/packfilecreater.h"
#include"../../source/storage/xmlfile.h"

using namespace Maid;


struct ARCHIVECONFIG
{
  String  ArchiveName;  //  出力する名前
  unt32   DivideSize;   //  分割サイズ
  std::vector<String> ArchiveFile;    //  アーカイブ化するファイル（ワイルドカード）
  std::vector<String> NoArchiveFile;  //  アーカイブに含めないファイル(正規表現)
  std::vector<String> NoComplessFile; //  圧縮しないファイル(正規表現)
};


  class PickUp
    : public Shell::IPickupFileFilter
  {
  public:
    PickUp( const std::vector<String>& str )
      : m_NoArchiveFile(str)
    {

    }

    virtual bool IsPassage( const Shell::FindObject& Data )
    {
      return true;
    }

  private:
    std::vector<String> m_NoArchiveFile;
  };


void main()
{
  {
    //  これはおまじないです。
    String::Initialize();
  }

  Storage storage;
  storage.Initialize(); //  これもおまじない

  JobPool jobpool;
  jobpool.Initialize(); //  これも(ry

  ARCHIVECONFIG ArchiveConfig;

  { //  設定ファイルの読み込み
    XMLFileReader ConfigFile;
    {
      ConfigFile.Load( MAIDTEXT("filearchiver_config.xml") );

      while( true )
      {
        if( ConfigFile.IsLoading() ) { ::Sleep(1); continue; }
        break;
      }

    }




    XMLReader& reader = ConfigFile.GetReader();

    {
      reader.AscendNode();
      while( true )
      {
        if( reader.IsEndNode() ) { break; }

        if( reader.GetNodeType()==XMLReader::NODETYPE_ELEMENT )
        {
          ARCHIVECONFIG& conf = ArchiveConfig;
          const String nodename = reader.GetNodeName();
          std::string str = String::ConvertMAIDtoSJIS(nodename);
          if( nodename==MAIDTEXT("archivename") )  { conf.ArchiveName = reader.GetString(); }
          ef( nodename==MAIDTEXT("dividesize" ) )  { conf.DivideSize = reader.GetInteger(); }
          ef( nodename==MAIDTEXT("archivefile") )  { conf.ArchiveFile.push_back(reader.GetString()); }
          ef( nodename==MAIDTEXT("noarchivefile")) { conf.NoArchiveFile.push_back(reader.GetString()); }
          ef( nodename==MAIDTEXT("nocomplessfile")){ conf.NoComplessFile.push_back(reader.GetString()); }
         
        }

        reader.NextNode();
      }

      reader.DescendNode();
    }
  }

  {
    //  アーカイブするファイルの選定
/*
    class CFindObject
    {
    public:
      CFindObject();
      CFindObject( WIN32_FIND_DATA& rData );

      enum TYPE
      {
	      TYPE_FILE,			//!<	普通のファイル
	      TYPE_DIRECTORY,		//!<	ディレクトリ
	      TYPE_UNKOWN,		//!<	わかりません
      };

      TYPE    GetFileType() const ;
      String  GetFileName() const ;

    private:
      WIN32_FIND_DATA	m_FindData;
    };

    class CPickupFileFilter
    {
    public:
      virtual ~CPickupFileFilter(){}
      virtual bool IsPassage( const CFindObject& Data ){return true;}
    };

    typedef boost::shared_ptr<CPickupFileFilter> SPPICKUPFILEFILTER;
    typedef std::list<CFindObject>			FINDOBJECTLIST;

    FINDOBJECTLIST PickupFile( const String& Path );
    FINDOBJECTLIST PickupFile( const String& Path, const SPPICKUPFILEFILTER& pFilter );
*/
  }




  PackFileCreater m_Packer;


  PackFileCreater::FILEINFO FileList[] =
  {
    PackFileCreater::FILEINFO(MAIDTEXT("nc1429.png"),true),
    PackFileCreater::FILEINFO(MAIDTEXT("nc1673.bmp"),false),
  };

  m_Packer.BeginArchive( MAIDTEXT("arc.dat"), 1024*1024*600, MAIDTEXT(""),
    std::vector<PackFileCreater::FILEINFO>(FileList,FileList+NUMELEMENTS(FileList)) );

  while( true )
  {
    const PackFileCreater::STATUS stat = m_Packer.GetStatus();

    if( stat.State!=PackFileCreater::STATUS::STATE_EXECUTING ) { break; }

    Sleep(100);

  }
}
