#ifndef graphic_core_driver_win32_direct3d09_debug_h
#define graphic_core_driver_win32_direct3d09_debug_h

/*!	
    @file
    @brief	デバッグ関数
*/


#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include"common.h"

#include<string>

std::string DebugStringD3D09( D3DFORMAT eFormat );
std::string DebugStringD3D09( D3DMULTISAMPLE_TYPE type );
std::string DebugStringD3D09( D3DSWAPEFFECT type );
std::string DebugStringD3D09( BOOL b );
std::string DebugStringD3D09PRESENTFLAG( UINT flag );
std::string DebugStringD3D09PRESENT( UINT flag );

std::string DebugStringD3D09( const D3DPRESENT_PARAMETERS& desc );


#endif