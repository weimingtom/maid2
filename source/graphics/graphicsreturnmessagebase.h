/*!	
	@file
	@brief	返信メッセージ 定義
*/

#ifndef maid2_graphics_graphicsreturnmessagebase_h
#define maid2_graphics_graphicsreturnmessagebase_h

#include"../config/define.h"

#include<boost/smart_ptr.hpp>
#include<vector>

namespace Maid
{
  //	描画スレッドから帰ってくるメッセージ

  namespace GraphicsReturnMessage
  {
    class Base
    {
      typedef uintptr_t OBJECTID;
      enum MESSAGE
      {
        COPYRENDERTARGET,		//	サーフェスの取得
        DRIVERCAPS,		        //	ドライバ情報の取得
      };
      Base( MESSAGE mess ) : Message(mess){}
      const MESSAGE	Message;
    };
  }

  typedef	boost::shared_ptr<GraphicsReturnMessage::Base> SPGRAPHICSRETURNMESSAGE;
  typedef	std::vector<SPGRAPHICSRETURNMESSAGE>           GRAPHICSRETURNMESSAGEQUE;

}

#endif