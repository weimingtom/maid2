#include"videodevice.h"


namespace Maid
{
void VideoDevice::Initialize()
{
  SetupDevice();
  SerchDisplayMode( m_DisplayMode );
  SerchEnableFormat(m_EnableFormat)
}

void VideoDevice::Finalize()
{

}

}


