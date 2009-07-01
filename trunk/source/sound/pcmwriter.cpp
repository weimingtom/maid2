#include"pcmwriter.h"
#include"soundmessage1.h"


namespace Maid {


PCMWriter::PCMWriter()
  :m_IsPlay(false)
  ,m_Volume(1)
  ,m_Position(0)
{

}

PCMWriter::~PCMWriter()
{
  Finalize();
}

void PCMWriter::Initialize( const PCMFORMAT& fmt, float BufferSize )
{
  Finalize();

  m_pBuffer.reset( new PCMBufferList(fmt) );
  m_pBuffer->Initialize();


  SoundCore* pCore = GlobalPointer<SoundCore>::Get();
  m_pInfo = pCore->CreateObjectInfo();

  {
    boost::shared_ptr<SoundMessage::CreatePCMRealTime> pMessage( new SoundMessage::CreatePCMRealTime );
    pMessage->pData = m_pBuffer;
    pMessage->Param.Format = fmt;
    pMessage->Param.Length = pMessage->Param.Format.CalcLength(BufferSize);

    pCore->PostMessage( m_pInfo, pMessage );
  }

  SetVolume( m_Volume );

}

void PCMWriter::Finalize()
{
  if( !IsInitialized() ) { return ; }

  boost::shared_ptr<SoundMessage::DeleteObject> pMess( new SoundMessage::DeleteObject );
  GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );

  m_pInfo.reset();

  m_IsPlay = false;
  m_Volume = 1;
  m_Position = 0;
}

void PCMWriter::Play()
{
  if( !IsInitialized() ) { return ; }

  boost::shared_ptr<SoundMessage::Play> pMess( new SoundMessage::Play );
  GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );

  m_IsPlay = true;
}

void PCMWriter::Stop()
{
  if( !IsInitialized() ) { return ; }
  if( !IsPlay() ) { return ; }

  m_pBuffer->Clear();

  {
    boost::shared_ptr<SoundMessage::Stop> pMess( new SoundMessage::Stop );
    GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );
  }

  {
    boost::shared_ptr<SoundMessage::SetPosition> pMess( new SoundMessage::SetPosition );
    pMess->Position = 0.0;
    GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );
  }

  m_IsPlay = false;
}

bool PCMWriter::IsPlay() const
{
  if( !IsInitialized() ) { return false; }

  return m_IsPlay;
}

void PCMWriter::SetVolume( float Volume )
{
  if( !IsInitialized() ) { return ; }

  boost::shared_ptr<SoundMessage::SetVolume> pMess( new SoundMessage::SetVolume );
  pMess->Volume = Volume;
  GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );

  m_Volume = Volume;
}

float PCMWriter::GetVolume() const
{
  if( !IsInitialized() ) { return 1; }

  return m_Volume;
}

void PCMWriter::Write( const void* pData, size_t Size )
{
  Write( -1, pData, Size );
}

void PCMWriter::Write( double time, const void* pData, size_t Size )
{
  if( !IsInitialized() ) { return ; }
  if( Size==0 ) { return ; }

  const size_t t = time<0? ~0 : m_pBuffer->GetFormat().CalcLength(time);

  m_pBuffer->Create( t, pData, Size );
}


bool PCMWriter::IsInitialized() const
{
  return m_pInfo.get()!=NULL;
}

}