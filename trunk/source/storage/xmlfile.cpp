#include"xmlfile.h"
#include"filereadersync.h"
#include"../auxiliary/debug/warning.h"


namespace Maid
{

XMLFileWriter::XMLFileWriter()
  :m_IsExecuting(false)
{

}

void XMLFileWriter::Write( const String& FileName )
{
  std::string tmp;
  Save( tmp );

  m_hFile.Create( FileName );
  m_hFile.Write( tmp.c_str(), tmp.length() );

  m_IsExecuting = true;
}

bool XMLFileWriter::IsWritting() const
{
  if( m_IsExecuting )
  {
     if( m_hFile.IsExecuting() ) { return true; }
     const_cast<XMLFileWriter*>(this)->m_hFile.Close();
     m_IsExecuting = false;
  }

  return false;
}



  namespace KEEPOUT
  {
    void XMLFileReaderFunction::Execute( const IJobInput& Input, IJobOutput& Output )
    {
      const XMLFileReaderInput&  in  = static_cast<const XMLFileReaderInput&>(Input);
      XMLFileReaderOutput& out = static_cast<XMLFileReaderOutput&>(Output);


      FileReaderSync  hFile;

      {
        const bool ret = hFile.Open( in.FileName );
        if( ret ) { MAID_WARNING( MAIDTEXT("オープンに失敗") ); return; }
      }

      std::vector<char> image( hFile.GetSize() );
      
      {
        const size_t ret = hFile.Read( &(image[0]), image.size() );
        if( ret!=image.size() ) { MAID_WARNING( MAIDTEXT("読み込みに失敗") ); return; }
      }

      out.Document.Parse( std::string(&(image[0]), image.size()) );
    }
  }



void XMLFileReader::Load( const String& FileName )
{
  if( FileName.empty() ) { return ; }
  m_Cache.Start( KEEPOUT::XMLFileReaderInput(FileName) );
}


bool XMLFileReader::IsLoading()const
{
  //  処理が終わるまでは忙しい
  if( m_Cache.IsExecuting() ) { return true; }

  const_cast<XMLFileReader*>(this)->m_Reader.Reset( m_Cache.GetOutput().Document );
  return false;
}

bool XMLFileReader::IsEmpty() const
{
  return m_Cache.IsEmpty();
}

XMLReader& XMLFileReader::GetReader()
{
  return m_Reader;
}



}