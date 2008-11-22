#include"igraphicsdevice.h"


namespace Maid
{
IGraphicsDevice::~IGraphicsDevice()
{
  Finalize();
}


void IGraphicsDevice::IGraphicsDevice()
{
  SetupDevice();
  SerchDisplayMode( m_DisplayMode );
  SerchEnableFormat(m_EnableFormat);
}

void IGraphicsDevice::Finalize()
{
}

}


