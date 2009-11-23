#include"soundcore.h"
#include"soundmessage1.h"

#include"soundobjectpcmstatic.h"
#include"soundobjectpcmstream.h"
#include"soundobjectpcmrealtime.h"
#include"soundobjectnull.h"

#include"../auxiliary/debug/warning.h"

namespace Maid {


//! サウンドオブジェクトの登録
/*!
    @param  pInfo [i ] 登録先
    @param  pObj  [i ] 登録するもの
 */
void SoundCore::RegisterObject( const SPSOUNDOBJECTINFO& pInfo, const SPSOUNDOBJECT& pObj )
{
  DATA dat;
  dat.pInfo   = pInfo;
  dat.pObject = pObj;
  m_ObjectList[pInfo->GetID()] = dat;
}


//! サウンドオブジェクトの破棄
/*!
    @param  pInfo [i ] 登録先
 */
void SoundCore::DeleteObject( const SPSOUNDOBJECTINFO& pInfo )
{
  OBJECTLIST::iterator ite = m_ObjectList.find(pInfo->GetID());
  if( ite==m_ObjectList.end() ) { return ; }

  m_ObjectList.erase(ite);
}

//! サウンドオブジェクトの取得
/*!
    @param  pInfo [i ] 登録先
 */
SPSOUNDOBJECT SoundCore::GetObject( const SPSOUNDOBJECTINFO& pInfo )
{
  OBJECTLIST::iterator ite = m_ObjectList.find(pInfo->GetID());
  if( ite==m_ObjectList.end() ) { return SPSOUNDOBJECT(new SoundObjectNULL); }

  return ite->second.pObject;
}


//! サウンドを監視しているスレッド
/*!
 */
unt SoundCore::ThreadFunction(volatile ThreadController::BRIGEDATA& brige)
{
  Sound::SPDEVICE& pDevice = m_Param.pDevice;
  pDevice->Initialize();

  PCMFORMAT fmt;
  fmt.Channels = 2;
  fmt.SamplesPerSecond = 44100;
  fmt.BitPerSamples = 16;
  pDevice->SetFormat(fmt);


  unt oldtime = m_Timer.Get();

  while( true )
  {
    if( brige.IsExit ) 
    {
      //  終了命令がでてても、メッセージが残っている場合は処理を続ける
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


    {
      //  定期的な更新
      const unt newtime = m_Timer.Get();
      for( OBJECTLIST::iterator ite=m_ObjectList.begin(); ite!=m_ObjectList.end(); ++ite )
      {
        DATA& dat = ite->second;

        dat.pObject->Update();

        dat.pInfo->m_IsPlay   = dat.pObject->IsPlay();
        dat.pInfo->m_Position = dat.pObject->GetPosition();
      }
    }
    //  メッセージを処理した分へらす
    {
      ThreadMutexLocker lock(m_MessageMutex);
      for( int i=0; i<(int)Mess.size(); ++i )
      {
        const SPSOUNDMESSAGE& pMess = Mess[i];
        pMess->pInfo->m_MessageCount -= 1;
      }
    }
    {
      const unt newtime = m_Timer.Get();
      const unt sa = newtime-oldtime;

      oldtime = newtime;
      //  あまりにも処理が早い場合は少しお休み
      if( sa<20 ) { ThreadController::Sleep( 20-sa ); }
//      if( sa<10 ) { ThreadController::Sleep( 1 ); }
    }

  }

  //  ここにきたときに、オブジェクトが残っている場合はリークしている
  MAID_ASSERT( !m_ObjectList.empty(), "オブジェクトが残っています" );

  m_Param.pDevice->Finalize();

  return 0;
}


//! メッセージを処理する実態
/*!
    @param Mess [i ]  処理するメッセージ
 */
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

      boost::shared_ptr<SoundObjectPCMStatic> pObj( new SoundObjectPCMStatic );
      pObj->Initialize( pBuffer, m.pData );
      RegisterObject( m.pInfo, pObj );
    
    }break;

  case SoundMessage::Base::CREATE_PCMSTREAM:
    {
      const SoundMessage::CreatePCMStream& m = static_cast<const SoundMessage::CreatePCMStream&>(Mess);
 
      Sound::SPBUFFER pBuffer = pDevice->CreateBuffer( m.Param );
      if( pBuffer.get()==NULL ) { break; }

      boost::shared_ptr<SoundObjectPCMStream> pObj( new SoundObjectPCMStream );
      pObj->Initialize( pBuffer, m.pDecoder, m.pData );
      RegisterObject( m.pInfo, pObj );

    }break;

  case SoundMessage::Base::CREATE_PCMREALTIME:
    {
      const SoundMessage::CreatePCMRealTime& m = static_cast<const SoundMessage::CreatePCMRealTime&>(Mess);
 
      Sound::SPBUFFER pBuffer = pDevice->CreateBuffer( m.Param );
      if( pBuffer.get()==NULL ) { break; }

      boost::shared_ptr<SoundObjectPCMRealTime> pObj( new SoundObjectPCMRealTime );
      pObj->Initialize( pBuffer, m.pData );
      RegisterObject( m.pInfo, pObj );

    }break;


  case SoundMessage::Base::CREATE_CLONE:
    {
      const SoundMessage::CreateClone& m = static_cast<const SoundMessage::CreateClone&>(Mess);

      SPSOUNDOBJECT pObj = GetObject( m.pSrc );

      switch( pObj->GetType() )
      {
      case ISoundObject::TYPE_STATIC:
        {
          SoundObjectPCMStatic* p = static_cast<SoundObjectPCMStatic*>(pObj.get());

          Sound::SPBUFFER pBuffer = pDevice->DuplicateBuffer( p->GetBuffer() );

          boost::shared_ptr<SoundObjectPCMStatic> pNewObj( new SoundObjectPCMStatic );
          pNewObj->Initialize( pBuffer, p->GetData() );
          RegisterObject( m.pInfo, pNewObj );
        }break;

      case ISoundObject::TYPE_STREAM:
        {
          SoundObjectPCMStream* p = static_cast<SoundObjectPCMStream*>(pObj.get());

          Sound::SPBUFFER pBuffer = pDevice->DuplicateBuffer( p->GetBuffer() );
          SPCUSTOMPCMREADER pCustom( new CustomPCMReader );
          {
            SPPCMREADER pDec = CreatePCMReader( p->GetData()->GetPointer(0), p->GetData()->GetSize() );
            if( pDec.get()==NULL )
            {
              MAID_WARNING( "デコーダの複製に失敗" );
              break;
            }

            pDec->Initialize();

            pCustom->Initialize( pDec, p->GetCustomPCMReader()->GetJumpList() );
          }

          boost::shared_ptr<SoundObjectPCMStream> pNewObj( new SoundObjectPCMStream );
          pNewObj->Initialize( pBuffer, pCustom, p->GetData() );
          RegisterObject( m.pInfo, pNewObj );
        }break;
      }
    }break;

  case SoundMessage::Base::DELETEOBJECT:
    {
      const SoundMessage::DeleteObject& m = static_cast<const SoundMessage::DeleteObject&>(Mess);
    
      DeleteObject( m.pInfo );
    }break;

  case SoundMessage::Base::PLAY:
    {
      const SoundMessage::Play& m = static_cast<const SoundMessage::Play&>(Mess);
      SPSOUNDOBJECT pObj = GetObject( m.pInfo );

      m.pInfo->m_IsPlay = true;
      pObj->Play();
    }break;

  case SoundMessage::Base::STOP:
    {
      const SoundMessage::Stop& m = static_cast<const SoundMessage::Stop&>(Mess);

      SPSOUNDOBJECT pObj = GetObject( m.pInfo );
      m.pInfo->m_IsPlay = false;
      pObj->Stop();
    }break;

  case SoundMessage::Base::SETPOSITION:
    {
      const SoundMessage::SetPosition& m = static_cast<const SoundMessage::SetPosition&>(Mess);
    
      SPSOUNDOBJECT pObj = GetObject( m.pInfo );
      m.pInfo->m_Position = m.Position;
      pObj->SetPosition( m.Position );
    }break;

  case SoundMessage::Base::SETVOLUME:
    {
      const SoundMessage::SetVolume& m = static_cast<const SoundMessage::SetVolume&>(Mess);

      SPSOUNDOBJECT pObj = GetObject( m.pInfo );

      m.pInfo->m_Volume = m.Volume;
      pObj->SetVolume( m.Volume );

    }break;

  case SoundMessage::Base::SETLOOPSTATE:
    {
      const SoundMessage::SetLoopState& m = static_cast<const SoundMessage::SetLoopState&>(Mess);
    
      SPSOUNDOBJECT pObj = GetObject( m.pInfo );
      pObj->SetLoopState( m.IsLoop );
    }break;

  case SoundMessage::Base::SETJUMPPOINT:
    {
      const SoundMessage::SetJumpPoint& m = static_cast<const SoundMessage::SetJumpPoint&>(Mess);
    
    }break;
  }
}

}


