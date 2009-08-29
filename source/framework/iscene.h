/*!
	  @file
	  @brief シーンクラスのベース
 */

#ifndef maid2_framework_iscene_h
#define maid2_framework_iscene_h

#include"../config/define.h"
#include"../graphics/graphicsobjecttemplate.h"
#include"../graphics/rendertargetbase.h"

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

    //! 初期化中か？
    /*!
 	      @return	初期化中なら true
    \n          完了したらfalse
     */
    virtual bool IsInitializing() const=0;


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
        @param  target  [i ]  描画先（すでに書き込まれていることがあるので、Clear()するときは慎重に）
        @param  depth   [i ]  描画先（すでに書き込まれていることがあるので、Clear()するときは慎重に）
     */
    virtual void UpdateDraw( const RenderTargetBase& target, const IDepthStencil& depth  )=0;


    //! 継承したクラスでシーン終了の信号を出す
    /*!
     */
    void ExitScene() { m_IsExit = true; }
    bool IsExit() const{ return m_IsExit; }
  private:
    bool m_IsExit;
  };

  typedef	boost::shared_ptr<IScene>  SPSCENE;

}

#endif
