#ifndef maid2_graphics_core_irasterizerstate_h
#define maid2_graphics_core_irasterizerstate_h


#include"../../config/define.h"
#include"idevicechild.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

/*
  RasterizerState ってのは、pixelshader の前に本当に描画するのかとか
  描画する量を決める設定
*/

  struct RASTERIZERSTATEPARAM
  {
    enum FILL
    {
      FILL_WIREFRAME,
      FILL_SOLID,
    };

    enum CULLING
    {
      CULLING_NONE=0,
      CULLING_LEFT,	//!<	反時計回りにカリング
      CULLING_RIGHT	//!<	時計回りにカリング
    };

    FILL    Fill;
    CULLING Culling;
    bool MultiSample;

    RASTERIZERSTATEPARAM()
      :Fill(FILL_SOLID)
      ,Culling(CULLING_LEFT)
      ,MultiSample(false)
    {
    }
  };

  class IRasterizerState : public IDeviceChild
  {
  public:
    IRasterizerState( const RASTERIZERSTATEPARAM& param ):m_Param(param){}
    const RASTERIZERSTATEPARAM& GetParam() const { return m_Param; }

  private:
    RASTERIZERSTATEPARAM m_Param;
   };

  typedef	boost::shared_ptr<IRasterizerState>	SPRASTERIZERSTATE;
}}

#endif