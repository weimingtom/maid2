#include"xmlfile.h"


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



XMLFileReader::XMLFileReader()
  :m_State(STATE_EMPTY)
{

}

void XMLFileReader::Load( const String& FileName )
{
  m_hFile.Open( FileName );

  m_State = STATE_LOADREAD;
}

bool XMLFileReader::IsLoading() const
{
  XMLFileReader* pThis = const_cast<XMLFileReader*>(this);

  switch( m_State )
  {
  case STATE_EMPTY:     { return false; }break;

  case STATE_LOADREAD:
    {
      if( m_hFile.IsExecuting() ) { return true; }

      const int len = m_hFile.GetSize();

      m_Buffer.Resize( len );
      pThis->m_hFile.Read( m_Buffer.GetPointer(0), len );

      m_State = STATE_LOADDATA;

      return true;
    }break;

  case STATE_LOADDATA:
    {
      if( m_hFile.IsExecuting() ) { return true; }
      pThis->XMLReader::Load( std::string((char*)m_Buffer.GetPointer(0),0,m_Buffer.GetSize()) );

      pThis->m_hFile.Close();

      m_Buffer.Resize(0);
      m_State = STATE_EXECUTING;
    }break;

  case STATE_EXECUTING:
    {
      return false; 
    }break;
  }

  return false;
}



}