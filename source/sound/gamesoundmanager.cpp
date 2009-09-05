#include"gamesoundmanager.h"
#include"gamesound.h"



namespace Maid { 


  /*!
      @class  GameSoundManager gamesoundmanager.h
      @brief  GameSound を管理しているクラス
  */

GameSoundManager*  GlobalPointer<GameSoundManager>::s_pPointer;

GameSoundManager::GameSoundManager()
:m_FPS(60)
{

}

//! 初期化
/*!
    GameSound はこの呼び出し以降使用できます
 */
void GameSoundManager::Initialize()
{
  GlobalPointer<GameSoundManager>::Set();

  m_Data = std::vector<TYPEDATA>(4);
  m_FPS = 60;

}


//! ボリュームの設定
/*!
    @param  type  [i ]  設定先
    @param  vol   [i ]  ボリューム
 */
void GameSoundManager::SetVolume( TYPE type, double vol )
{
  m_Data[type].Volume = vol;
  UpdateVolume();
}

//! ボリュームの取得
/*!
    @param  type  [i ]  取得したいタイプ

    @return  ボリューム
 */
double GameSoundManager::GetVolume( TYPE type ) const
{
  return m_Data[type].Volume;
}


//! フレーム毎の更新
/*!
    この関数をゲーム側で毎フレーム呼び出すこと。
 */
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

//! 指定したチャンネルの基本ボリュームを取得する
/*!
 */
double GameSoundManager::CalcScale( TYPE type )const
{
  if( GetMuteState(type) || GetMuteState(TYPE_GLOBAL) ) { return 0.0f; }
  if( type==TYPE_GLOBAL ) { return m_Data[TYPE_GLOBAL].Volume; }

  const double global = m_Data[TYPE_GLOBAL].Volume;
  const double vol = m_Data[type].Volume;

  return vol * global;
}

//! 指定した時間が何フレームになるか計算する
/*!
    @param  time  計算したい時間（1.0f = １秒）
 */
int GameSoundManager::CalcFrame( double time )const
{
  const double fps = double(m_FPS);
  const double frame = fps * time;

  return int( frame+0.5f);
}

//! ミュート状態の変更
/*!
    @param  type    [i ]  設定するチャンネル
    @param  IsMute  [i ]  true でミュート
 */
void GameSoundManager::SetMuteState( TYPE type, bool IsMute )
{
  m_Data[type].IsMute = IsMute;
  UpdateVolume();
}


//! ミュートしてるか？
/*!
    @param  type    [i ]  調べたいチャンネル

    @return  ミュートなら true
 */
bool GameSoundManager::GetMuteState( TYPE type )const
{
  return m_Data[type].IsMute;
}



}

