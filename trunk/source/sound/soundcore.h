/*!	
    @file
    @brief	サウンドコア
*/

#ifndef maid2_sound_soundcore_h
#define maid2_sound_soundcore_h

#include"../config/define.h"
#include"../auxiliary/globalpointer.h"
#include"../auxiliary/thread.h"
#include"../auxiliary/timer.h"


#include"core/idevice.h"

#include"soundmessagebase.h"
#include"isoundobject.h"
#include"pcmreader/ipcmreader.h"
#include<map>


namespace Maid {

  class SoundCore 
    : public GlobalPointer<SoundCore>
	{
	public:
		SoundCore();
		virtual ~SoundCore();

    struct INITIALIZEPARAM
    {

      Sound::SPDEVICE pDevice;
      bool  IsBackGround;       //  スレッドを作って別途処理する。
      unt32 ProcesserMask;

      INITIALIZEPARAM()
        :IsBackGround(true)
        ,ProcesserMask(1)
      {

      }
    };

		void Initialize( const INITIALIZEPARAM& param );
		void Finalize();

    void Update();

    SPSOUNDOBJECTINFO    CreateObjectInfo();
 
    SPPCMREADER CreatePCMReader( const void* pData, size_t Size ) const;

    void PostMessage( const SPSOUNDOBJECTINFO& pInfo, const SPSOUNDMESSAGE& pMess );

  private:  //  スレッド間をまたがないメンバ

    void RegisterObject( const SPSOUNDOBJECTINFO& pInfo, const SPSOUNDOBJECT& pObj, const String& ShreadID );
    void DeleteObject( const SPSOUNDOBJECTINFO& pInfo );
    SPSOUNDOBJECT GetObject( const SPSOUNDOBJECTINFO& pInfo );
    Sound::SPBUFFER GetSharedBuffer( const String& ShreadID );

    struct DATA
    {
      SPSOUNDOBJECTINFO pInfo;
      SPSOUNDOBJECT     pObject;
      String            ShreadID;
    };

    typedef std::map<unt,DATA> OBJECTLIST;
    OBJECTLIST  m_ObjectList;

    Timer       m_Timer;


  private:  //  スレッド間をまたぐメンバ
    ThreadMutex       m_MessageMutex;
    SOUNDMESSAGEQUE   m_MessageQue;
    unt          m_CreateCount;
    ThreadMutex m_ObjectMutex;


  private:
    unt ThreadFunction(volatile ThreadController::BRIGEDATA& brige);
    void ExecuteMessage( const SoundMessage::Base& Mess );
    INITIALIZEPARAM     m_Param;
    ThreadController    m_AccessThread;
  };
}


#endif
