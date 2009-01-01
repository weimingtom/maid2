/*!	
    @file
    @brief	サウンドコア
*/

#ifndef maid2_sound_soundcore_h
#define maid2_sound_soundcore_h

#include"../config/define.h"
#include"../auxiliary/globalpointer.h"
#include"../auxiliary/thread.h"


#include"core/idevice.h"

#include"soundmessagebase.h"
#include"isoundobject.h"
#include<map>


namespace Maid {

  class SoundCore : public GlobalPointer<SoundCore>
	{
	public:
		SoundCore();
		virtual ~SoundCore();

    struct INITIALIZEPARAM
    {
      Sound::SPDEVICE pDevice;
      bool  IsBackGround;       //  スレッドを作って別途処理する。
    };

		void Initialize( const INITIALIZEPARAM& param );
		void Finalize();

    void Update();

    typedef unt OBJECTID;
    OBJECTID    CreateObjectID();
 
    void PostMessage( OBJECTID id, const SPSOUNDMESSAGE& pMess );

  private:  //  スレッド間をまたがないメンバ
    OBJECTID   m_CreateCount;
    ThreadMutex m_ObjectMutex;

    void RegisterObject( OBJECTID id, const SPSOUNDOBJECT& pObj );
    void DeleteObject( OBJECTID id );
    SPSOUNDOBJECT GetObject( OBJECTID id );

    typedef std::map<OBJECTID,SPSOUNDOBJECT> OBJECTLIST;
    OBJECTLIST  m_ObjectList;


  private:  //  スレッド間をまたぐメンバ
    ThreadMutex       m_MessageMutex;
    SOUNDMESSAGEQUE   m_MessageQue;


  private:
    unt ThreadFunction(volatile ThreadController::BRIGEDATA& brige);
    void ExecuteMessage( const SoundMessage::Base& Mess );
    ThreadController    m_AccessThread;
    INITIALIZEPARAM     m_Param;
  };
}


#endif
