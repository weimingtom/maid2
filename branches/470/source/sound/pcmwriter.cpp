#include"pcmwriter.h"
#include"soundmessage1.h"
#include"../auxiliary/debug/warning.h"


namespace Maid {


PCMWriter::PCMWriter()
  :m_IsPlay(false)
  ,m_Volume(1)
  ,m_AddTime(0)
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

  m_AddTime = 0;
}

void PCMWriter::Finalize()
{
  if( !IsInitialized() ) { return ; }

  boost::shared_ptr<SoundMessage::DeleteObject> pMess( new SoundMessage::DeleteObject );
  GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );

  m_pInfo.reset();

  m_IsPlay = false;
  m_Volume = 1;
}

void PCMWriter::Play()
{
  if( !IsInitialized() ) { return ; }

  boost::shared_ptr<SoundMessage::Play> pMess( new SoundMessage::Play );
  GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );

  m_IsPlay = true;
}

void PCMWriter::Pause()
{
  if( !IsInitialized() ) { return ; }
  if( !IsPlay() ) { return ; }

  {
    boost::shared_ptr<SoundMessage::Stop> pMess( new SoundMessage::Stop );
    GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );
  }
  m_IsPlay = false;
}

void PCMWriter::Stop()
{
  if( !IsInitialized() ) { return ; }
  if( !IsPlay() ) { return ; }

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

  if( time<0 )
  {
    m_pBuffer->Create( m_AddTime, pData, Size );
    m_AddTime += Size;
  }else
  {
    const size_t t = m_pBuffer->GetFormat().CalcLength(time);
    if( t+Size < m_AddTime ) { return ; }  //  昔のデータを加えるのはまずい

    m_pBuffer->Create( t, pData, Size );
    m_AddTime = t + Size;
  }
}


bool PCMWriter::IsInitialized() const
{
  return m_pInfo.get()!=NULL;
}

}