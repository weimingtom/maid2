#ifndef graphic_core_driver_win32_debug_h
#define graphic_core_driver_win32_debug_h

/*!	
    @file
    @brief	デバッグ関数
*/


#include"../../../config/define.h"
#include"../../../config/typedef.h"

#include"dxgitypedef.h"

#include<string>

std::string DebugStringDXGI( DXGI_FORMAT eFormat );
std::string DebugStringDXGI( const DXGI_SWAP_CHAIN_DESC& desc );


#endif