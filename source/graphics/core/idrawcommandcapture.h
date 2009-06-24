#ifndef maid2_graphics_core_idrawcommandcapture_h
#define maid2_graphics_core_idrawcommandcapture_h

#include"../../config/define.h"

#include"idrawcommand.h"

namespace Maid { namespace Graphics {

  /*!	
      @class	IDrawCommandCapture idrawcommandcapture.h
      @brief	描画コンテキスト コマンド録画実行タイプ
  */

  class IDrawCommandCapture : public IDrawCommand
  {
  public:

  };

  typedef	boost::shared_ptr<IDrawCommandCapture>	SPDRAWCOMMANDCAPTURE;


}}

#endif