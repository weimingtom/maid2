#ifndef maid2_graphics_blendstate_h
#define maid2_graphics_blendstate_h

/*!
    @file
    @brief  BlendState
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"graphicscore.h"

namespace Maid
{
  class BlendState
      :public IBlendState
      ,public GlobalPointer<GraphicsCore>
  {
  public:
    BlendState();
    virtual ~BlendState();

    enum TEMPLATE
    {
      TEMPLATE_NORMAL, //  そのまま転送
      TEMPLATE_ALPHA,  //  普通のα計算
      TEMPLATE_ADD,    //  加算
      TEMPLATE_SUB,    //  減算
      TEMPLATE_MUL,    //  乗算
      TEMPLATE_SCREEN,   //  スクリーン
      TEMPLATE_DARKEN,   //  比較(暗)
      TEMPLATE_LIGHTTEN, //  比較(明)
    };

    void Create( TEMPLATE t );
    void Create( const Graphics::BLENDSTATEPARAM& state );
    void Delete();

  };	
}

#endif