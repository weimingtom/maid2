/*!
	@file
	@brief Win32用 VideoCard列挙ヘッダ
 */

#ifndef graphic_core_driver_win32_videocardlist_h
#define graphic_core_driver_win32_videocardlist_h


#include"../../../../config/define.h"
#include"../../../../config/win32.h"

#include<d3d9.h>

#include"../../videocardlistbase.h"

#include"../../../../framework/win32/dllwrapper.h"
#include"../../../../framework/win32/com_ptr.h"
#include"../../../../framework/win32/window.h"


namespace Maid
{
  class VideoCardList : public VideoCardListBase
  {
  public:
    VideoCardList( Window& window );
    void Initialize();
    int GetVideoCardCount();
    VIDEOCARDINFO GetVideoCardInfo( int no );
    SPVIDEOCARD CreateVideoCard( int No, const CREATEPARAM& Info );

  private:
    Window&  m_Window;

  private:
    DllWrapper          m_D3D9DLL;
    com_ptr<IDirect3D9> m_pDirect3D09;

  private:
  };
}



#endif
