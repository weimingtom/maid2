#include"videodevice.h"


namespace Maid
{
VideoDevice::~VideoDevice()
{
  Finalize();
}


void VideoDevice::Initialize()
{
  SerchDisplayMode( m_DisplayMode );
  SerchEnableFormat(m_EnableFormat);
  SetupDevice();
}

void VideoDevice::Finalize()
{
}

}


