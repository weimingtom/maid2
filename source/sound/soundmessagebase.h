#ifndef maid2_sound_soundmessagebase_h
#define maid2_sound_soundmessagebase_h

/*!	
	@file
	@brief	メッセージ処理関数のベース
*/

#include"../config/define.h"
#include<boost/smart_ptr.hpp>
#include<vector>

namespace Maid
{
  class SoundCore;
	namespace SoundMessage
	{
		class Base
		{
      friend  SoundCore;
		public:
			//	サウンドメッセージ
			enum MESSAGE
			{
				NONE,				//!<	何もしない

				CREATE_PCMSTATIC, //!<	オンメモリサウンドとして生成
				CREATE_PCMSTREAM, //!<	ストリームサウンドとして生成
				CREATE_CLONE,     //!<	クローンを作成

				DELETEOBJECT,     //!<	破棄

				PLAY,		//!<	再生	
				STOP,		//!<	ストップ
				SETPOSITION,//!<	再生場所の移動
				SETVOLUME,		//!<	ボリュームの変更

        SETLOOPSTATE, //!<  ループ再生するか？
        SETJUMPPOINT, //!<  指定したポイントまできたら戻る位置
			};

			Base( MESSAGE mess ) : Message(mess){}

			const MESSAGE	Message;
    private:
			int		ID;
		};
	}

	typedef	boost::shared_ptr<SoundMessage::Base>	SPSOUNDMESSAGE;
	typedef	std::vector<SPSOUNDMESSAGE>			SOUNDMESSAGEQUE;

  struct JUMPPOINT
  {
	  float	Boot;
	  float	Jump;
  };
}

#endif
