/*!
	  @file
	  @brief シーンクラスを管理するクラスのテンプレ
 */

#ifndef framework_isceneadmin_h
#define framework_isceneadmin_h

#include"../config/define.h"

#include"iscene.h"

namespace Maid
{
  class ISceneAdmin
  {
  public:
    virtual ~ISceneAdmin(){}

    void Initialize();
    void UpdateFrame();
    void UpdateDraw();

  protected:
    virtual SPSCENE CreateFirstScene()=0; 
    virtual void OnEndScene( const SPSCENEOUTPUT& pOut, SPSCENE& pNext )=0;

  private:
    SPSCENE m_pCurrentScene;
  };
}

#endif
