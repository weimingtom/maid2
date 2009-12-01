#ifndef maid2_sound_soundfile_h
#define maid2_sound_soundfile_h

#include"../config/define.h"
#include"../auxiliary/globalpointer.h"
#include"../auxiliary/jobcachetemplate.h"
#include"../auxiliary/string.h"
#include"../auxiliary/memorybuffer.h"


#include"soundcore.h"
#include"soundmessagebase.h"
#include"pcmreader/ipcmreader.h"
#include"pcmreader/custompcmreader.h"

#include<vector>

namespace Maid {

  namespace KEEPOUT
  {
    class SoundFileInput : public IJobInput
    {
    public:
      enum LOADTYPE
      {
        LOADTYPE_AUTO,
        LOADTYPE_STATIC,
        LOADTYPE_STREAM,
      };

      SoundFileInput( const String& name, SoundCore* p, LOADTYPE t, const SOUNDJUMPPOINTLIST& List )
        :FileName(name), pCore(p),Type(t),JumpList(List){}
    
      String FileName;
      SoundCore* pCore;
      LOADTYPE Type;
      SOUNDJUMPPOINTLIST  JumpList;
    };
    inline bool operator < ( const SoundFileInput& lha, const SoundFileInput& rha ) 
    {
      if( lha.FileName < rha.FileName ) { return true; }
      if( lha.FileName > rha.FileName ) { return false; }

      return lha.Type < rha.Type;
    }

    class SoundFileOutput : public IJobOutput
    {
    public:
      SPSOUNDOBJECTINFO pInfo;
      SPSOUNDMESSAGE pMessage;
    };

    class SoundFileFunction : public IJobFunction
    {
    public:
      void Execute( const IJobInput& Input, IJobOutput& Output );

    private:
      SPSOUNDMESSAGE CreateStatic( const SPMEMORYBUFFER& pFileImage, const SPPCMREADER& pDecorder );
      SPSOUNDMESSAGE CreateStream( const SPMEMORYBUFFER& pFileImage, const SPPCMREADER& pDecorder, const CustomPCMReader::JUMPPOINTLIST& jump );
    };
  }

  class SoundFile
    : public GlobalPointer<SoundCore>
	{
	public:
		SoundFile();
		SoundFile( const SoundFile& obj );
		virtual ~SoundFile();

    void LoadFile( const String& filename );
    void LoadFileStream( const String& filename );
    void LoadFileStream( const String& filename, const SOUNDJUMPPOINTLIST& List );
    void LoadFileStatic( const String& filename );
    void Destroy();
    bool IsLoading()const;

    void Play();
    void Stop();

    void SetLoopState( bool IsLoop );
    void SetPosition( double time );
    void SetVolume( double Volume );

    bool IsPlay() const;
    bool IsLoopState() const;
    double GetPosition() const;
    double GetVolume() const;

    bool IsEmpty() const;

    String GetFileName() const;
    SoundFile& operator=(const SoundFile &obj);
  private:
    void LoadCheck();

    typedef JobCacheTemplate<KEEPOUT::SoundFileInput,KEEPOUT::SoundFileFunction, KEEPOUT::SoundFileOutput> CACHE;
    CACHE m_Cache;

    SPSOUNDOBJECTINFO m_pInfo;

    bool  m_IsPlay;
    bool  m_IsLoopPlay;
    double m_Volume;
    double m_Position;
  };
}


#endif
