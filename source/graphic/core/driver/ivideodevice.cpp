#include"ivideodevice.h"


namespace Maid
{
IVideoDevice::~IVideoDevice()
{
  Finalize();
}


void IVideoDevice::Initialize()
{
  SetupDevice();
  SerchDisplayMode( m_DisplayMode );
  SerchEnableFormat(m_EnableFormat);
}

void IVideoDevice::Finalize()
{
}

}


