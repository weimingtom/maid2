#include"gamesoundmanager.h"
#include"gamesound.h"



namespace Maid { 

GameSoundManager*  GlobalPointer<GameSoundManager>::s_pPointer;

GameSoundManager::GameSoundManager()
:m_FPS(60)
{

}

void GameSoundManager::Initialize()
{
  GlobalPointer<GameSoundManager>::Set();

  m_Data = std::vector<TYPEDATA>(4);
  m_FPS = 60;

}

void GameSoundManager::SetVolume( TYPE type, float vol )
{
  m_Data[type].Volume = vol;
  UpdateVolume();
}

float GameSoundManager::GetVolume( TYPE type ) const
{
  return m_Data[type].Volume;
}

void GameSoundManager::UpdateFrame()
{
  for( std::set<GameSound*>::iterator ite=GameSound::s_ObjectList.begin();
                                ite != GameSound::s_ObjectList.end();
                                ++ite )
  {
    (*ite)->UpdateFrame();
  }
}

void GameSoundManager::UpdateVolume()
{
  for( std::set<GameSound*>::iterator ite=GameSound::s_ObjectList.begin();
                                ite != GameSound::s_ObjectList.end();
                                ++ite )
  {
    (*ite)->OnGlobalVolumeChange();
  }
}


float GameSoundManager::CalcScale( TYPE type )const
{
  if( GetMuteState(type) ) { return 0.0f; }
  if( type==TYPE_GLOBAL ) { return m_Data[TYPE_GLOBAL].Volume; }

  const float global = m_Data[TYPE_GLOBAL].Volume;
  const float vol = m_Data[type].Volume;

  return vol * global;
}

int GameSoundManager::CalcFrame( float time )const
{
  const float fps = float(m_FPS);
  const float frame = fps * time / 1000.0f;

  return int( frame+0.5f);
}

void GameSoundManager::SetMuteState( TYPE type, bool IsMute )
{
  m_Data[type].IsMute = IsMute;
  UpdateVolume();
}

bool GameSoundManager::GetMuteState( TYPE type )const
{
  return m_Data[type].IsMute;
}



}

