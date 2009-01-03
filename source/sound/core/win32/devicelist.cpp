#include"devicelist.h"
#include"directsound8/devicedirectsound8.h"

#include"../../../auxiliary/debug/warning.h"


namespace Maid { namespace Sound {

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! DirectSoundドライバの能力を調べる関数
/*!	
 *	詳しくはDXヘルプを参照
 *
 */
BOOL CALLBACK DeviceList::DirectSoundEnumCallback( LPGUID pGUID, LPCWSTR strDesc, LPCWSTR strDrvName, LPVOID pContext )
{
  std::vector<DIRECTSOUND8DEVICE>* pList = (std::vector<DIRECTSOUND8DEVICE>*)pContext;

  DIRECTSOUND8DEVICE  dev;

  if( pGUID!=NULL )
  {
    dev.pGUID.reset( new GUID );
    ::memcpy( dev.pGUID.get(), pGUID, sizeof(GUID) );
  }

  dev.Desc = String::ConvertUNICODEtoMAID(strDesc);
  dev.Driver = String::ConvertUNICODEtoMAID(strDrvName);

  pList->push_back( dev );

  return TRUE;
}


DeviceList::DeviceList( Window& window )
  :m_Window(window)
{

}

void DeviceList::Initialize()
{

  { //  directsound8 を調べる
	  m_dsoundDLL.Load( MAIDTEXT("dsound.dll") );
		typedef  HRESULT (WINAPI *SoundEnum)( LPDSENUMCALLBACK, LPVOID );
		SoundEnum pDirectSoundEnumerate = (SoundEnum)m_dsoundDLL.GetProcAddress(MAIDTEXT("DirectSoundEnumerateW"));
		pDirectSoundEnumerate( &DeviceList::DirectSoundEnumCallback,  &m_DirectSound8Device );
  }

}

void DeviceList::GetList( std::vector<INFO>& ret )
{
  INFO info;

  info.Name = MAIDTEXT("DirectSound8:primary");

  ret.push_back( info );

}

SPDEVICE DeviceList::CreateDevice( int DeviceID )
{
#pragma  COMPILERMSG( "当面は引数に関係なくdirectsound8 プライマリデバイスです" )

  return SPDEVICE( new DeviceDirectSound8( m_DirectSound8Device[0].pGUID,m_dsoundDLL,m_Window) );

}

}}


