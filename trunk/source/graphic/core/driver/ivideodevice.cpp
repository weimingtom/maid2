#include"ivideodevice.h"


namespace Maid
{
IVideoDevice::~IVideoDevice()
{
  Finalize();
}


void IVideoDevice::Initialize()
{
  SerchDisplayMode( m_DisplayMode );
  SerchEnableFormat(m_EnableFormat);
  SetupDevice();
}

void IVideoDevice::Finalize()
{
}

}


