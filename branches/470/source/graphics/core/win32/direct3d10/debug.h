#ifndef graphic_core_driver_win32_direct3d10_debug_h
#define graphic_core_driver_win32_direct3d10_debug_h

/*!	
    @file
    @brief	デバッグ関数
*/


#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include"common.h"

#include<string>

std::string DebugStringD3D10( BOOL b );
std::string DebugStringD3D10( D3D10_BLEND type );
std::string DebugStringD3D10( D3D10_BLEND_OP type );
std::string DebugStringD3D10( D3D10_USAGE usage );
std::string DebugStringD3D10( D3D10_BIND_FLAG flag );
std::string DebugStringD3D10( D3D10_CPU_ACCESS_FLAG flag );
std::string DebugStringD3D10( D3D10_RESOURCE_MISC_FLAG flag );
std::string DebugStringD3D10( D3D10_MAP type );
std::string DebugStringD3D10( D3D10_INPUT_CLASSIFICATION type );


std::string DebugStringD3D10( const D3D10_TEXTURE2D_DESC& desc );
std::string DebugStringD3D10( const D3D10_SUBRESOURCE_DATA* desc );

std::string DebugStringD3D10( const D3D10_BLEND_DESC& desc );
std::string DebugStringD3D10( const D3D10_BUFFER_DESC& desc );
std::string DebugStringD3D10( const D3D10_INPUT_ELEMENT_DESC& desc );

#endif