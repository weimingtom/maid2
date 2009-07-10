/*!
 	@file
 	@brief レジストリキーを取得・作成するクラス
*/

#ifndef maid2_framework_win32_registry_h
#define maid2_framework_win32_registry_h

#include"../../config/define.h"
#include"../../config/win32.h"
#include"../../auxiliary/macro.h"
#include"../../auxiliary/string.h"
#include"../../auxiliary/exception.h"
#include"../../auxiliary/functionresult.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  class Registry
  {
  public:
    Registry();
    ~Registry();

    FUNCTIONRESULT Open( HKEY hKey, const String& SubKey );
    FUNCTIONRESULT Create( HKEY hKey, const String& SubKey );
    void Close();

    FUNCTIONRESULT SetValue( const String& Name, unt32 Data );
    FUNCTIONRESULT SetValue( const String& Name, const String& Data );
    FUNCTIONRESULT SetValue( const String& Name, const void* pData, unt32 Length );

    void GetValue( const String& Name, unt32& Data );
    void GetValue( const String& Name, String& Data );
    void GetValue( const String& Name, boost::shared_array<unt08>& pData, unt32& Length );

    void DeleteValue( const String& ValueName );

    static void DeleteKey( HKEY hKey, const String& SubKey );
    static void DeleteValue( HKEY hKey, const String& SubKey, const String& ValueName );

  private:
    HKEY  m_hKey;
  };
}

#endif
