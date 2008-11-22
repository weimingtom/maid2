#include"igraphicsdevice.h"


namespace Maid
{
IGraphicsDevice::~IGraphicsDevice()
{
  Finalize();
}


void IGraphicsDevice::Initialize()
{
  SetupDevice();
  SerchDisplayMode( m_DisplayMode );
  SerchEnableFormat(m_EnableFormat);
}

void IGraphicsDevice::Finalize()
{
}

}


