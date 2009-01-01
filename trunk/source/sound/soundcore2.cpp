#include"soundcore.h"
#include"soundobjectlocal.h"
#include"soundmessage1.h"



namespace Maid {

void SoundCore::RegisterObject( OBJECTID id, const SPSOUNDOBJECT& pObj )
{
  m_ObjectList[id] = pObj;
}

void SoundCore::DeleteObject( OBJECTID id )
{
  OBJECTLIST::iterator ite = m_ObjectList.find(id);
  if( ite==m_ObjectList.end() ) { return ; }

  m_ObjectList.erase(ite);
}

SPSOUNDOBJECT SoundCore::GetObject( OBJECTID id )
{
  OBJECTLIST::iterator ite = m_ObjectList.find(id);
  if( ite==m_ObjectList.end() ) { return SPSOUNDOBJECT(); }

  return ite->second;
}

unt SoundCore::ThreadFunction(volatile ThreadController::BRIGEDATA& brige)
{
  Sound::SPDEVICE& pDevice = m_Param.pDevice;
  pDevice->Initialize();

  Sound::BUFFERFORMAT fmt;
  fmt.Channels = 2;
  fmt.SamplesPerSecond = 44100;
  fmt.BitPerSamples = 16;

  pDevice->SetFormat(fmt);

  while( true )
  {
    if( brige.IsExit ) 
    {
       ThreadMutexLocker Lock(m_MessageMutex);
       if( m_MessageQue.empty() ) { break; }
    }

    SOUNDMESSAGEQUE Mess;
    { //	とりあえず溜まっているメッセージを拾う
      ThreadMutexLocker Lock(m_MessageMutex);
      Mess.swap(m_MessageQue);
    }

    //  メッセージを順番に処理
    for( int i=0; i<(int)Mess.size(); ++i )
    {
      const SPSOUNDMESSAGE& pMess = Mess[i];
      ExecuteMessage( *pMess );
    }

    //  定期的なストリームバッファの更新

    //	一仕事終わったのでしばし休憩
    ThreadController::Sleep( 10 );
  }


  pDevice->Finalize();

  return 0;
}

void SoundCore::ExecuteMessage( const SoundMessage::Base& Mess )
{
  Sound::SPDEVICE& pDevice = m_Param.pDevice;

  switch( Mess.Message )
  {
  case SoundMessage::Base::CREATE_PCMSTATIC:
    {
      const SoundMessage::CreatePCMStatic& m = static_cast<const SoundMessage::CreatePCMStatic&>(Mess);

      Sound::SPBUFFER pBuffer = pDevice->CreateBuffer( m.Param );

      if( pBuffer.get()==NULL ) { break; }

      Sound::IBuffer::LOCKDATA dat;
      pBuffer->Lock( 0, 0, dat );

      memcpy( dat.pData1, &(m.pData->begin()), m.pData->size() );

      pBuffer->Unlock( dat );

//      boost::shared_ptr<SoundObjectPCMStatic> pObj( new SoundObjectPCMStatic );
//      pObj->pBuffer = pBuffer;
//      RegisterObject( m.ID, pObj );
    
    }break;

  case SoundMessage::Base::CREATE_PCMSTREAM:
    {
      const SoundMessage::CreatePCMStream& m = static_cast<const SoundMessage::CreatePCMStream&>(Mess);
    
    }break;

  case SoundMessage::Base::CREATE_CLONE:
    {
      const SoundMessage::CreateClone& m = static_cast<const SoundMessage::CreateClone&>(Mess);
   
    }break;

  case SoundMessage::Base::DELETEOBJECT:
    {
      const SoundMessage::DeleteObject& m = static_cast<const SoundMessage::DeleteObject&>(Mess);
    
      DeleteObject( m.ID );
    }break;

  case SoundMessage::Base::PLAY:
    {
      const SoundMessage::Play& m = static_cast<const SoundMessage::Play&>(Mess);
    
    }break;

  case SoundMessage::Base::STOP:
    {
      const SoundMessage::Stop& m = static_cast<const SoundMessage::Stop&>(Mess);
    
    }break;

  case SoundMessage::Base::SETPOSITION:
    {
      const SoundMessage::SetPosition& m = static_cast<const SoundMessage::SetPosition&>(Mess);
    
    }break;

  case SoundMessage::Base::SETVOLUME:
    {
      const SoundMessage::SetVolume& m = static_cast<const SoundMessage::SetVolume&>(Mess);
    
    }break;

  case SoundMessage::Base::SETLOOPSTATE:
    {
      const SoundMessage::SetLoopState& m = static_cast<const SoundMessage::SetLoopState&>(Mess);
    
    }break;

  case SoundMessage::Base::SETJUMPPOINT:
    {
      const SoundMessage::SetJumpPoint& m = static_cast<const SoundMessage::SetJumpPoint&>(Mess);
    
    }break;
  }
}

}


