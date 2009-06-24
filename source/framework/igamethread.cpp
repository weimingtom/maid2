#include"igamethread.h"

#include"igameframework.h"
#include"../auxiliary/debug/warning.h"

namespace Maid
{

using namespace OSMessage;

IGameThread::IGameThread()
  : m_IsExitGame(false)
  , m_pFrameWork(NULL)
  , m_IsAspectLock(false)
{

}

IGameThread::~IGameThread()
{

}


bool	IGameThread::SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
{
  return true;
}


bool IGameThread::BootCheck()
{
  return true;
}


void	IGameThread::Initialize()
{
}


void	IGameThread::Finalize()
{
}



bool IGameThread::IsExitGame()
{
	return m_IsExitGame;
}

void IGameThread::OnExitGame()
{
	m_IsExitGame = true;
}

GraphicsCore& IGameThread::GetGraphicsCore() { return GetFrameWork()->GetGraphicsCore(); }
SoundCore&    IGameThread::GetSoundCore()    { return GetFrameWork()->GetSoundCore();    }
Mouse&        IGameThread::GetMouse()        { return GetFrameWork()->GetMouse();        }
Keybord&      IGameThread::GetKeybord()      { return GetFrameWork()->GetKeybord();      }
TextInput&    IGameThread::GetTextInput()    { return GetFrameWork()->GetTextInput();    }
JobPool&      IGameThread::GetJobPool()      { return GetFrameWork()->GetJobPool();    }

const GraphicsCore& IGameThread::GetGraphicsCore() const { return GetFrameWork()->GetGraphicsCore(); }
const SoundCore&    IGameThread::GetSoundCore()    const { return GetFrameWork()->GetSoundCore();    }
const Mouse&        IGameThread::GetMouse()        const { return GetFrameWork()->GetMouse();        }
const Keybord&      IGameThread::GetKeybord()      const { return GetFrameWork()->GetKeybord();      }
const TextInput&    IGameThread::GetTextInput()    const { return GetFrameWork()->GetTextInput();    }
const JobPool&      IGameThread::GetJobPool()      const { return GetFrameWork()->GetJobPool();    }

void IGameThread::TimerReset()
{
  return GetFrameWork()->GetTimer().Reset();
}

void IGameThread::SetScreenSize( const SIZE2DI& size )
{
  GraphicsCore& gra = GetGraphicsCore();

  GraphicsCore::SWAPCHAINFORMAT fmt = gra.GetSwapChainFormat();

  fmt.Size = size;

  gra.SetSwapChainFormat(fmt);

  UpdateMouseMapping();
}

void IGameThread::SetFullScreenState( bool IsFull )
{
  GraphicsCore& gra = GetGraphicsCore();
  Mouse& mou = GetMouse();

  gra.SetFullScreenState( IsFull );

  UpdateMouseMapping();
}

void IGameThread::SetAspectLock( bool lock )
{
  m_IsAspectLock = lock;
  UpdateMouseMapping();
}


void IGameThread::UpdateMouseMapping()
{
  GraphicsCore& gra = GetGraphicsCore();
  Mouse& mou = GetMouse();

  if( IsFullScreen() && IsAspectLock() )
  { //  フルスクリーンの時は即座に反映させる
    gra.SetAspectLock( true );
    const RECT2DI port = gra.GetBackBuffer().CalcDrawArea();
    mou.SetClipRange( port );
  }else
  {
    const SIZE2DI size = gra.GetBackBuffer().GetSize();

    gra.SetAspectLock( false );
    mou.SetClipRange( RECT2DI(POINT2DI(0,0),size) );
  }
}


bool IGameThread::IsFullScreen() const
{
  return GetGraphicsCore().IsFullScreen();
}

bool IGameThread::IsAspectLock() const
{
  return m_IsAspectLock;
}


const IGameThread::DEFAULTCONFIG& IGameThread::GetDefaultConfig() const
{
  return const_cast<IGameThread*>(this)->GetFrameWork()->GetDefaultConfig();
}


IGameThread::OSMESSAGESTATE IGameThread::OnOSMessage( const OSMessage::Base& Message )
{
  switch( Message.Message )
  {
  case Base::PUSHCLOSEBUTTON: { OnExitGame(); }break;
  case Base::PUSHMAXIMIZEBUTTON:
    {
      SetFullScreenState( true );
    }break;

  case Base::CHANGESCREENMODE:
    {
      const bool IsFull = GetGraphicsCore().IsFullScreen();
      SetFullScreenState( !IsFull );
    }break;
  }

  return OSMESSAGESTATE_DEFAULT;
}

IGameFrameWork* IGameThread::GetFrameWork(){ return m_pFrameWork; }
const IGameFrameWork* IGameThread::GetFrameWork() const { return m_pFrameWork; }



}