#include"guimanager.h"
#include"../../auxiliary/debug/assert.h"

namespace Maid
{

GUIManager::GUIManager()
  :m_pMouse(NULL),m_pKeybord(NULL),m_IsEnable(false),m_IsVisible(false)
{

}

GUIManager::~GUIManager()
{
  Finalize();
}

void GUIManager::Initialize( const Mouse* pMouse, const Keybord* pKey )
{
  Finalize();

  m_pMouse   = pMouse;
  m_pKeybord = pKey;
}

void GUIManager::Finalize()
{
  m_IsEnable  = false;
  m_IsVisible = false;
  m_PartsList.clear();
  m_pMouse   = NULL;
  m_pKeybord = NULL;
}


void GUIManager::UpdateFrame()
{
  if( !m_IsEnable ) { return ; }

  //  とりあえず管理してるフレームを進める
  for( PARTSLIST::iterator ite =m_PartsList.begin();
                           ite!=m_PartsList.end(); ++ite )
  {
    ite->second->UpdateFrame();
  }


  //  次に起こっているメッセージを探す



}

void GUIManager::UpdateDraw( RenderTargetBase& Target )
{
  if( !m_IsVisible ) { return ; }
  for( PARTSLIST::iterator ite =m_PartsList.begin();
                           ite!=m_PartsList.end(); ++ite )
  {
    ite->second->UpdateDraw(Target);
  }
}

void GUIManager::SetEnable( bool IsEnable )
{
  m_IsEnable = IsEnable;
}

void GUIManager::SetVisible( bool IsVisible )
{
  m_IsVisible = IsVisible;
}

void GUIManager::RegisterParts( const SPGUIPARTS& pParts )
{
  pParts->SendMessage( GUIMESSAGE_CREATE() );
  m_PartsList[pParts->GetID()] = pParts;
}

void GUIManager::DeleteParts( const SPGUIPARTS& pParts )
{
  MAID_ASSERT( !IsExitstParts(pParts), "指定したパーツは存在していません " << pParts->GetID() );

  m_PartsList.erase( m_PartsList.find(pParts->GetID()) );
  pParts->SendMessage( GUIMESSAGE_DESTROY() );
}

bool GUIManager::IsExitstParts( const SPGUIPARTS& pParts ) const
{
  PARTSLIST::const_iterator ite = m_PartsList.find(pParts->GetID());

  return ite!=m_PartsList.end();
}

SPGUIPARTS GUIManager::FindParts( IGUIParts::ID id ) const
{
  PARTSLIST::const_iterator ite = m_PartsList.find(id);

  MAID_ASSERT( ite==m_PartsList.end(), "指定したパーツは存在していません " << id );

  return ite->second;
}

}




