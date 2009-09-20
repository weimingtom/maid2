#ifndef maid2_sound_core_win32_directsound8_common_h
#define maid2_sound_core_win32_directsound8_common_h

#include"../../../../config/define.h"
#include"../../../../config/win32.h"
#include"../../../../framework/win32/com_ptr.h"

#define DIRECTSOUND_VERSION 0x0800

#include<mmreg.h>
#include<dsound.h>

#pragma comment(lib,"dsound.lib")


namespace Maid { namespace Sound {


	typedef com_ptr<IDirectSound8>		SPDIRECTSOUND8;
	typedef com_ptr<IDirectSoundBuffer>	SPDIRECTSOUNDBUFFER;
	typedef com_ptr<IDirectSoundBuffer8>	SPDIRECTSOUNDBUFFER8;

}}


#endif
