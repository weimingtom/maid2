#include"soundfile.h"

#include"soundmessage1.h"
#include"../storage/filereadersync.h"
#include"../auxiliary/debug/warning.h"


namespace Maid {

  namespace KEEPOUT
  {
  void SoundFileFunction::Execute( const IJobInput& Input, IJobOutput& Output )
  {
    const SoundFileInput&  in  = static_cast<const SoundFileInput&>(Input);
    SoundFileOutput& out = static_cast<SoundFileOutput&>(Output);

    boost::shared_ptr<std::vector<unt08> >  pFileImage( new std::vector<unt08> );

    SPPCMDECODER pDecorder;
    {
      FileReaderSync hFile;

      if( hFile.Open(in.FileName) ) { MAID_WARNING( "ファイルが開けません " << in.FileName ); return; }

      pFileImage->resize( hFile.GetSize() );

      hFile.Read( &(pFileImage->front()), pFileImage->size() );

      //  先頭の数バイトを見て、デコーダを作るのがいいか
      pDecorder.reset( new RiffPCM );
      pDecorder->Open( &(pFileImage->front()), pFileImage->size() );

//      if( Decoder.Open( &(pFileImage->front()), pFileImage->size() )==IPCMDecoder::OPENRESULT_ERROR )
//      { MAID_WARNING( "デコーダがありません " << in.FileName ); return; }
    }
    SPSOUNDMESSAGE pMessage;
    
    switch( in.Type )
    {
    case SoundFileInput::LOADTYPE_AUTO:
      {
        const size_t len = pDecorder->GetLength();
        const float time = pDecorder->GetFormat().CalcTime(len);

        //  10秒未満ならstatic
        if( time < 10.0f ) {  pMessage = CreateStatic( pFileImage, pDecorder ); }
        else { pMessage = CreateStream( pFileImage, pDecorder ); }


      }break;
    case SoundFileInput::LOADTYPE_STATIC:
      {
         pMessage = CreateStatic( pFileImage, pDecorder );
      }break;
    case SoundFileInput::LOADTYPE_STREAM:
      {
         pMessage = CreateStream( pFileImage, pDecorder );
      }break;
    }
    
    out.pInfo = in.pCore->CreateObjectInfo();
    in.pCore->PostMessage( out.pInfo, pMessage );
  }

  SPSOUNDMESSAGE SoundFileFunction::CreateStatic( const boost::shared_ptr<std::vector<unt08> >& pFileImage, const SPPCMDECODER& pDecorder )
  {
    boost::shared_ptr<std::vector<unt08> >  pPCMData( new std::vector<unt08> );

    pPCMData->resize( pDecorder->GetLength() );
    pDecorder->Read( &((*pPCMData)[0]), pDecorder->GetLength() );

    boost::shared_ptr<SoundMessage::CreatePCMStatic> pMessage( new SoundMessage::CreatePCMStatic );
    pMessage->pData    = pPCMData;

    Sound::CREATEBUFFERPARAM& param = pMessage->Param;

    param.Format = pDecorder->GetFormat();
    param.Length = pDecorder->GetLength();

    return pMessage;
  }

  SPSOUNDMESSAGE SoundFileFunction::CreateStream( const boost::shared_ptr<std::vector<unt08> >& pFileImage, const SPPCMDECODER& pDecorder )
  {
    boost::shared_ptr<SoundMessage::CreatePCMStream> pMessage( new SoundMessage::CreatePCMStream );
    pMessage->pData    = pFileImage;
    pMessage->pDecoder = pDecorder;

    Sound::CREATEBUFFERPARAM& param = pMessage->Param;

    param.Format = pDecorder->GetFormat();
    param.Length = param.Format.CalcLength(2.0f);

    return pMessage;
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
  m_Cache.Start( KEEPOUT::SoundFileInput(filename, GlobalPointer<SoundCore>::Get(), KEEPOUT::SoundFileInput::LOADTYPE_AUTO ) );

  LoadCheck();
}

void SoundFile::LoadFileStream( const String& filename )
{
  Destroy();
  m_Cache.Start( KEEPOUT::SoundFileInput(filename, GlobalPointer<SoundCore>::Get(), KEEPOUT::SoundFileInput::LOADTYPE_STREAM ) );
  LoadCheck();
}

void SoundFile::LoadFileStatic( const String& filename )
{
  Destroy();
  m_Cache.Start( KEEPOUT::SoundFileInput(filename, GlobalPointer<SoundCore>::Get(), KEEPOUT::SoundFileInput::LOADTYPE_STATIC ) );
  LoadCheck();
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
  SoundFile* pThis = const_cast<SoundFile*>(this);
  pThis->LoadCheck();

  return m_pInfo.get()==NULL;
}

void SoundFile::LoadCheck()
{
  if( m_pInfo.get()!=NULL ) { return ; }

  //  処理が終わるまでは忙しい
  if( m_Cache.IsExecuting() ) { return ; }

  const KEEPOUT::SoundFileOutput& out = m_Cache.GetOutput();

  if( m_Cache.IsUnique() )
  { //  最初の一個目はそのまま
    m_pInfo = out.pInfo;
  }else
  { //  ２個目以降はコピーする
    SoundCore* pCore = GlobalPointer<SoundCore>::Get();

    m_pInfo = pCore->CreateObjectInfo();

    boost::shared_ptr<SoundMessage::CreateClone> pMess( new SoundMessage::CreateClone );
    pMess->pSrc = out.pInfo;
    pCore->PostMessage(m_pInfo, pMess );
  }
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


