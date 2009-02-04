#ifndef maid2_sound_soundfile_h
#define maid2_sound_soundfile_h

#include"../config/define.h"
#include"../auxiliary/globalpointer.h"
#include"../auxiliary/jobcachetemplate.h"
#include"../auxiliary/string.h"


#include"soundcore.h"
#include"pcmdecoder.h"


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

      SoundFileInput( const String& name, SoundCore* p, LOADTYPE t )
        :FileName(name), pCore(p),Type(t){}
    
      String FileName;
      SoundCore* pCore;
      LOADTYPE Type;
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
    };

    class SoundFileFunction : public IJobFunction
    {
    public:
      void Execute( const IJobInput& Input, IJobOutput& Output );

    private:
      SPSOUNDMESSAGE CreateStatic( const boost::shared_ptr<std::vector<unt08> >& pFileImage, const SPPCMDECODER& pDecorder );
      SPSOUNDMESSAGE CreateStream( const boost::shared_ptr<std::vector<unt08> >& pFileImage, const SPPCMDECODER& pDecorder );
    };
  }

  class SoundFile
    : public GlobalPointer<SoundCore>
	{
	public:
		SoundFile();
		virtual ~SoundFile();

    void LoadFile( const String& filename );
    void LoadFileStream( const String& filename );
    void LoadFileStatic( const String& filename );
    void Destroy();
    bool IsLoading()const;

    void Play();
    void Stop();

    void SetLoopState( bool IsLoop );
    void SetPosition( float time );
    void SetVolume( float Volume );

    bool IsPlay() const;
    bool IsLoopState() const;
    float GetPosition() const;
    float GetVolume() const;

    bool IsEmpty() const;
  private:
    void LoadCheck();

    typedef JobCacheTemplate<KEEPOUT::SoundFileInput,KEEPOUT::SoundFileFunction, KEEPOUT::SoundFileOutput> CACHE;
    CACHE m_Cache;

    SPSOUNDOBJECTINFO m_pInfo;

    bool  m_IsPlay;
    bool  m_IsLoopPlay;
    float m_Volume;
    float m_Position;
  };
}


#endif
