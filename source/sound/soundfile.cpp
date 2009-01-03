#include"soundfile.h"

#include"soundmessage1.h"
#include"../storage/filereadersync.h"
#include"../auxiliary/debug/warning.h"

#include"pcmdecoder.h"

namespace Maid {

  namespace KEEPOUT
  {
  void SoundFileFunction::Execute( const IJobInput& Input, IJobOutput& Output )
  {
    const SoundFileInput&  in  = static_cast<const SoundFileInput&>(Input);
    SoundFileOutput& out = static_cast<SoundFileOutput&>(Output);

    std::vector<unt08>  FileImage;
    RiffPCM Decoder;
    {

      FileReaderSync hFile;

      if( hFile.Open(in.FileName) ) { MAID_WARNING( "ファイルが開けません " << in.FileName ); return; }

      FileImage.resize( hFile.GetSize() );

      hFile.Read( &(FileImage[0]), FileImage.size() );

      if( Decoder.Open( &(FileImage[0]), FileImage.size() )==IPCMDecoder::OPENRESULT_ERROR )
      { MAID_WARNING( "デコーダがありません " << in.FileName ); return; }
    }

    boost::shared_ptr<SoundMessage::CreatePCMStatic> pMessage( new SoundMessage::CreatePCMStatic );
    pMessage->pData.reset( new std::vector<unt08> );

    std::vector<unt08>&  pcmdata = *(pMessage->pData);
    pcmdata.resize( Decoder.GetLength() );
    Decoder.Read( &(pcmdata[0]), pcmdata.size() );

    Sound::CREATEBUFFERPARAM& param = pMessage->Param;

    param.Length = Decoder.GetLength();
    param.Format.SamplesPerSecond = Decoder.GetSamplesPerSec();
    param.Format.BitPerSamples = Decoder.GetBitsPerSamples();
    param.Format.Channels = Decoder.GetChannels();

    out.pInfo = in.pCore->CreateObjectInfo();
    in.pCore->PostMessage( out.pInfo, pMessage );
  }
  }

SoundFile::CACHE::INFOMAP  SoundFile::CACHE::s_InfoMap;


SoundFile::SoundFile()
{

}

SoundFile::~SoundFile()
{
  Destroy();
}

void SoundFile::LoadFile( const String& filename )
{
  Destroy();
  m_Cache.Start( KEEPOUT::SoundFileInput(filename, GlobalPointer<SoundCore>::Get() ) );
}

void SoundFile::Destroy()
{
  if( m_pInfo.get()==NULL ) { return ; }

  boost::shared_ptr<SoundMessage::DeleteObject> pMess( new SoundMessage::DeleteObject );
  GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );

  m_Cache.Reset();
  m_pInfo.reset();
}

bool SoundFile::IsLoading()const
{
  //  処理が終わるまでは忙しい
  if( m_Cache.IsExecuting() ) { return true; }

  const KEEPOUT::SoundFileOutput& out = m_Cache.GetOutput();
  SoundFile* pThis = const_cast<SoundFile*>(this);

  if( m_Cache.IsUnique() )
  { //  最初の一個目はそのまま
    pThis->m_pInfo = out.pInfo;
  }else
  { //  ２個目以降はコピーする
    SoundCore* pCore = GlobalPointer<SoundCore>::Get();

    pThis->m_pInfo = pCore->CreateObjectInfo();

    boost::shared_ptr<SoundMessage::CreateClone> pMess( new SoundMessage::CreateClone );
    pMess->pSrc = m_pInfo;
    pCore->PostMessage(m_pInfo, pMess );
  }

  return false;
}

void SoundFile::Play()
{
  if( m_pInfo.get()==NULL ) { return ; }

  boost::shared_ptr<SoundMessage::Play> pMess( new SoundMessage::Play );
  GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );
}

void SoundFile::Stop()
{
  if( m_pInfo.get()==NULL ) { return ; }
  boost::shared_ptr<SoundMessage::Stop> pMess( new SoundMessage::Stop );
  GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );
}


}


