#ifndef maid2_sound_gamesound_h
#define maid2_sound_gamesound_h

#include"../config/define.h"
#include"../auxiliary/counter.h"
#include"gamesoundmanager.h"
#include"soundfile.h"

namespace Maid { 



  class GameSound 
    : public GlobalPointer<GameSoundManager>
  {
    friend GameSoundManager;
  public:
    GameSound();
    GameSound( const GameSound& rhs );
    virtual ~GameSound();

    enum TYPE
    {
      TYPE_GLOBAL,
      TYPE_SE,
      TYPE_BGM,
      TYPE_VOICE,
    };

    void Load( TYPE type, const String& filename );
    void Load( TYPE type, const String& filename, const SOUNDJUMPPOINTLIST& List );
    void Destroy();
    bool IsLoading()const;

    enum
    {
      LOOPPLAY = -1
    };

    void Play();
    void Play( int count );
    void Play( int count, double pos );
    void Stop();
    void Resume();
    void Pause();

    void SetVolume( double volume, double time );
    void FadeOut( double time );
    void SetPosition( double time );

    bool IsPlay() const;

    double GetPosition() const;
    double GetVolume() const;

    bool IsEmpty() const;
    String GetFileName() const;

    void SetPlayCount( int count );
    int  GetPlayCount() const;

  private:
    void _Play();
    void UpdateFrame();
    void OnGlobalVolumeChange();
    double GetScaleVolume()const;


  private:
    SoundFile m_Sound;

    int m_PlayCount;  //  残り再生回数

    bool  m_IsPlaying;
    LinearCounter<double>  m_RealVolume;

    bool m_IsFadeOuting;
    double m_PrevPosition;
    double m_Volume;

    TYPE  m_Type;
  private:
    static std::set<GameSound*> s_ObjectList;

  };




}

#endif
