#ifndef soundmessage1_h
#define soundmessage1_h

#include"soundmessagebase.h"
#include"isoundobject.h"

#include"core/ibuffer.h"

namespace Maid
{
	namespace SoundMessage
	{
		class CreatePCMStatic : public Base
		{
		public:
			CreatePCMStatic() : Base(CREATE_PCMSTATIC){}

      Sound::CREATEBUFFERPARAM Param;
			boost::shared_ptr<std::vector<unt08> >	pData;	//	PCMデータ
		};

		class CreatePCMStream : public Base
		{
		public:
			CreatePCMStream() : Base(CREATE_PCMSTREAM){}

      Sound::CREATEBUFFERPARAM Param;
//			SPSOUNDDECODER	pDecoder;	//	デコーダ
		};

		class CreateClone : public Base
		{
		public:
			CreateClone() : Base(CREATE_CLONE){}

			int	SrcID;
		};

    class DeleteObject : public Base
		{
		public:
			DeleteObject() : Base(DELETEOBJECT){}
		};


		class Play : public Base
		{
		public:
			Play() : Base(PLAY){}
		};

		class Stop : public Base
		{
		public:
			Stop() : Base(STOP){}
		};


		class SetPosition : public Base
		{
		public:
			SetPosition() : Base(SETPOSITION){}
			float	Position;
		};


    class SetJumpPoint : public Base
		{
		public:
			SetJumpPoint() : Base(SETJUMPPOINT){}

      std::vector<JUMPPOINT>   List;
		};

		class SetVolume : public Base
		{
		public:
			SetVolume() : Base(SETVOLUME){}
			float	Volume;
		};

		class SetLoopState : public Base
		{
		public:
			SetLoopState() : Base(SETLOOPSTATE){}
			bool	IsLoop;
		};
  }
}

#endif
