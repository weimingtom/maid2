#ifndef maid2_sound_gamesoundmanager_h
#define maid2_sound_gamesoundmanager_h

#include"../config/define.h"
#include"../auxiliary/globalpointer.h"

#include<set>
#include<vector>

namespace Maid { 

  class GameSoundManager
    : public GlobalPointer<GameSoundManager>
  {
  public:
    GameSoundManager();

    void Initialize();

    enum TYPE
    {
      TYPE_GLOBAL=0,  //  全体
      TYPE_SE,      //  効果音
      TYPE_BGM,     //  ＢＧＭ
      TYPE_VOICE,   //  音声
    };

    void SetVolume( TYPE type, double vol );
    double GetVolume( TYPE type )const;

    void SetMuteState( TYPE type, bool IsMute );
    bool GetMuteState( TYPE type )const;

    void UpdateFrame();

    double CalcScale( TYPE type )const;
    int CalcFrame( double time )const;

  private:
    void UpdateVolume();

    struct TYPEDATA
    {
      TYPEDATA():Volume(0.5f),IsMute(false){}
      double Volume;
      bool  IsMute;
    };

    std::vector<TYPEDATA> m_Data;
    int                   m_FPS;
  };
}

#endif
