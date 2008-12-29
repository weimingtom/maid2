/*!
	  @file
	  @brief シーンクラスのベース
 */

#ifndef framework_iscene_h
#define framework_iscene_h

#include"../config/define.h"
#include"../graphics/graphicsobjecttemplate.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  class ISceneInput
  {
  public:
    virtual ~ISceneInput(){}
  };

  typedef	boost::shared_ptr<ISceneInput>  SPSCENEINPUT;

  class ISceneOutput
  {
  public:
    virtual ~ISceneOutput(){}
  };

  typedef	boost::shared_ptr<ISceneOutput>  SPSCENEOUTPUT;


  class IScene
  {
  public:
    IScene():m_IsExit(false){}
    virtual ~IScene(){}

    //! このシーンを初期化する
    /*!
 	      @param	pInput	[i ]	作成パラメータ
     */
    virtual void Initialize( const SPSCENEINPUT& pInput )=0;

		//! このシーンを片付ける
		/*!
        @param	pOutput	[i ]	終了パラメータ
		 */
    virtual void Finalize( SPSCENEOUTPUT& pOutput )=0;

    //! フレーム毎の処理を行う
    /*!
     */
    virtual void UpdateFrame()=0;

    //! 描画処理を行う
    /*!
     */
    virtual void UpdateDraw()=0;

    //! シーン切り替えの直前に呼ばれる。描画処理を書く
    /*!
        @param  target  [ o]  こいつに描画する。 シーン間の切り替えに使う。
     */
    virtual void LastDraw( IRenderTarget& target )=0;


    void ExitScene() { m_IsExit = true; }
    bool IsExit() const{ return m_IsExit; }
  private:
    bool m_IsExit;
  };

  typedef	boost::shared_ptr<IScene>  SPSCENE;

}

#endif
