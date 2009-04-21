/*!	
	@file
	@brief	XMLファイル読み書きクラス
*/

#ifndef maid2_storage_xmlfile_h
#define maid2_storage_xmlfile_h

#include"../config/define.h"
#include"../auxiliary/xml.h"
#include"../auxiliary/buffer.h"
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



  class XMLFileReader
    : public XMLReader
  {
  public:
    XMLFileReader();
    void Load( const String& FileName );
    bool IsLoading() const;

  private:
    FileReader  m_hFile;

    mutable Buffer m_Buffer;

    enum STATE
    {
      STATE_EMPTY,
      STATE_LOADREAD,
      STATE_LOADDATA,
      STATE_EXECUTING,
    };

    mutable STATE m_State;
  };

}



#endif