#ifndef maid2_graphics_core_idrawcommandexecute_h
#define maid2_graphics_core_idrawcommandexecute_h

#include"../../config/define.h"

#include"idrawcommand.h"

namespace Maid { namespace Graphics {

  /*!	
      @class	IDrawCommandExecute idrawcommandexecute.h
      @brief	描画コンテキスト 即時実行タイプ
  */

  class IDrawCommandExecute : public IDrawCommand
  {
  public:

  };
  typedef	boost::shared_ptr<IDrawCommandExecute>	SPDRAWCOMMANDEXECUTE;

}}

#endif