/*!	
	@file
	@brief	XMLファイル読み書きクラス
*/

#ifndef maid2_storage_xmlfile_h
#define maid2_storage_xmlfile_h

#include"../config/define.h"
#include"../auxiliary/xml.h"
#include"../auxiliary/memorybuffer.h"
#include"../auxiliary/jobcachetemplate.h"
#include"filewriter.h"
#include"filereader.h"



namespace Maid
{
  class XMLFileWriter 
    : public XMLWriter
  {
  public:
    XMLFileWriter();
    void Write( const String& FileName );
    bool IsWritting() const;

  private:
    FileWriter  m_hFile;
 
    mutable bool m_IsExecuting;
  };



  namespace KEEPOUT
  {
    class XMLFileReaderInput : public IJobInput
    {
    public:
      XMLFileReaderInput(){}
      XMLFileReaderInput( const String& name )
        :FileName(name){}
      String FileName;
    };

    inline bool operator < ( const XMLFileReaderInput& lha, const XMLFileReaderInput& rha ) 
    {
      return lha.FileName < rha.FileName; 
    }

    class XMLFileReaderOutput : public IJobOutput
    {
    public:
      XMLFileReaderOutput(){}

      XMLDocument Document;
    };

    class XMLFileReaderFunction : public IJobFunction
    {
    public:
      void Execute( const IJobInput& Input, IJobOutput& Output );

    };
  }

  class XMLFileReader
  {
  public:
    void Load( const String& FileName );
    bool IsLoading() const;

    bool IsEmpty() const;


    XMLReader& GetReader();

  private:

    XMLReader m_Reader;

    typedef JobCacheTemplate<KEEPOUT::XMLFileReaderInput,KEEPOUT::XMLFileReaderFunction, KEEPOUT::XMLFileReaderOutput> CACHE;
    CACHE m_Cache;
  };

}



#endif