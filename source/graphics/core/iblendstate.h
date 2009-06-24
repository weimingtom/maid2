#ifndef maid2_graphics_core_iblendstate_h
#define maid2_graphics_core_iblendstate_h


#include"../../config/define.h"
#include"idevicechild.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

/*
  BlendStateってのは、転送先と転送元の色調節＆合成方法を決めるステート。

  PixelShaderから出てきた色が src ですでにRenderTargetにあるピクセルが dst とすると
  (dst*A) + (src*B) の処理で A,B,+記号の３つの設定を行う。

  うまくやればスクリーン合成もできるからね！
*/


  struct BLENDSTATEPARAM
  {
    struct TARGET
    {
      enum BLEND
      {
        BLEND_ZERO,
        BLEND_ONE,
        BLEND_SRC_COLOR,
        BLEND_INV_SRC_COLOR,
        BLEND_SRC_ALPHA,
        BLEND_INV_SRC_ALPHA,
        BLEND_DST_ALPHA,
        BLEND_INV_DST_ALPHA,
        BLEND_DST_COLOR,
        BLEND_INV_DST_COLOR,
        BLEND_SRC_ALPHA_SAT,
        BLEND_BLEND_FACTOR,
        BLEND_INV_BLEND_FACTOR,
      };

      enum OPERATION
      {
        OPERATION_ADD,
        OPERATION_SUBTRACT,
        OPERATION_REV_SUBTRACT,
        OPERATION_MIN,
        OPERATION_MAX,
      };

      enum MASK
      {
        MASK_RED  =0x01,
        MASK_GREEN=0x02,
        MASK_BLUE =0x04,
        MASK_ALPHA=0x08,
        MASK_ALL = MASK_RED|MASK_GREEN|MASK_BLUE|MASK_ALPHA
      };
      bool  Enable;

      BLEND ColorSrc;
      BLEND ColorDst;
      OPERATION ColorOp;
      BLEND AlphaSrc;
      BLEND AlphaDst;
      OPERATION AlphaOp;
      unt08 WriteMask;

      TARGET()
        :Enable(false)
        ,ColorSrc (BLEND_ONE)
        ,ColorDst (BLEND_ZERO)
        ,ColorOp  (OPERATION_ADD)
        ,AlphaSrc (BLEND_ONE)
        ,AlphaDst (BLEND_ZERO)
        ,AlphaOp  (OPERATION_ADD)
        ,WriteMask(MASK_ALL)
      {}
    };

    bool  AlphaToCoverageEnable;  //  さっぱり不明
    bool  IndependentBlendEnable; //  さっぱり不明

    TARGET  Target[8];

    BLENDSTATEPARAM()
      :AlphaToCoverageEnable(false)
      ,IndependentBlendEnable(false)
    {
    }
  };

  class IBlendState : public IDeviceChild
  {
  public:
    IBlendState( const BLENDSTATEPARAM& param ):m_Param(param){}
    const BLENDSTATEPARAM& GetParam() const { return m_Param; }

  private:
    BLENDSTATEPARAM m_Param;
  };

  typedef	boost::shared_ptr<IBlendState>	SPBLENDSTATE;
}}

#endif