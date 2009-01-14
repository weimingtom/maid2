#include"guimessageque.h"


namespace Maid
{
GUIMessageQue::~GUIMessageQue()
{

}

void GUIMessageQue::PostMessage( IGUIParts* pTarget, const IGUIParam& Param )
{
  MESSAGE m;
  m.pTarget = pTarget;
  m.pParam.reset(Param.Clone());

  m_Que.push_back( m );
}


void GUIMessageQue::Execute()
{
  while( true )
  {
    if( m_Que.empty() ) { break; }

    MESSAGEQUE dat;
    dat.swap(m_Que);

    for( size_t i=0; i<dat.size(); ++i )
    {
      MESSAGE& m = dat[i];

      m.pTarget->SendMessage( *(m.pParam) );
    }
  }

}

}