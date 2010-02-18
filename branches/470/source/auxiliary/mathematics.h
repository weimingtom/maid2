/*!
 	@file
 	@brief	数学テンプレート
 */

#ifndef maid2_auxiliary_mathematics_h
#define maid2_auxiliary_mathematics_h

#include"../config/define.h"

#include"mathematics/point.h"
#include"mathematics/line.h"
#include"mathematics/size.h"
#include"mathematics/rect.h"
#include"mathematics/vector.h"
#include"mathematics/matrix.h"
#include"mathematics/quaternion.h"

#include"mathematics/function.h"



//	各種合成
namespace Maid
{
	//	Point + Vector
	template<typename TYPE>	POINT2D_TEMPLATE<TYPE> operator+( const POINT2D_TEMPLATE<TYPE>& lha, const VECTOR2D_TEMPLATE<TYPE>& rha ){ return POINT2D_TEMPLATE<TYPE>( lha.x+rha.x, lha.y+rha.y); }
	template<typename TYPE>	POINT3D_TEMPLATE<TYPE> operator+( const POINT3D_TEMPLATE<TYPE>& lha, const VECTOR3D_TEMPLATE<TYPE>& rha ){ return POINT3D_TEMPLATE<TYPE>( lha.x+rha.x, lha.y+rha.y, lha.z+rha.z); }
	template<typename TYPE>	POINT4D_TEMPLATE<TYPE> operator+( const POINT4D_TEMPLATE<TYPE>& lha, const VECTOR4D_TEMPLATE<TYPE>& rha ){ return POINT4D_TEMPLATE<TYPE>( lha.x+rha.x, lha.y+rha.y, lha.z+rha.z, lha.w+rha.w); }

	//	Point += Vector
	template<typename TYPE>	POINT2D_TEMPLATE<TYPE>& operator+=( POINT2D_TEMPLATE<TYPE>& lha, const VECTOR2D_TEMPLATE<TYPE>& rha ){ lha.x+=rha.x; lha.y+=rha.y; return lha;}
	template<typename TYPE>	POINT3D_TEMPLATE<TYPE>& operator+=( POINT3D_TEMPLATE<TYPE>& lha, const VECTOR3D_TEMPLATE<TYPE>& rha ){ lha.x+=rha.x; lha.y+=rha.y; lha.z+=rha.z; return lha;}
	template<typename TYPE>	POINT4D_TEMPLATE<TYPE>& operator+=( POINT4D_TEMPLATE<TYPE>& lha, const VECTOR4D_TEMPLATE<TYPE>& rha ){ lha.x+=rha.x; lha.y+=rha.y; lha.z+=rha.z; lha.w+=rha.w; return lha;}

	//	Point - Vector
	template<typename TYPE>	POINT2D_TEMPLATE<TYPE> operator-( const POINT2D_TEMPLATE<TYPE>& lha, const VECTOR2D_TEMPLATE<TYPE>& rha ){ return POINT2D_TEMPLATE<TYPE>( lha.x-rha.x, lha.y-rha.y); }
	template<typename TYPE>	POINT3D_TEMPLATE<TYPE> operator-( const POINT3D_TEMPLATE<TYPE>& lha, const VECTOR3D_TEMPLATE<TYPE>& rha ){ return POINT3D_TEMPLATE<TYPE>( lha.x-rha.x, lha.y-rha.y, lha.z-rha.z); }
	template<typename TYPE>	POINT4D_TEMPLATE<TYPE> operator-( const POINT4D_TEMPLATE<TYPE>& lha, const VECTOR4D_TEMPLATE<TYPE>& rha ){ return POINT4D_TEMPLATE<TYPE>( lha.x-rha.x, lha.y-rha.y, lha.z-rha.z, lha.w-rha.w); }

	//	Point -= Vector
	template<typename TYPE>	POINT2D_TEMPLATE<TYPE>& operator-=( POINT2D_TEMPLATE<TYPE>& lha, const VECTOR2D_TEMPLATE<TYPE>& rha ){ lha.x-=rha.x; lha.y-=rha.y; return lha;}
	template<typename TYPE>	POINT3D_TEMPLATE<TYPE>& operator-=( POINT3D_TEMPLATE<TYPE>& lha, const VECTOR3D_TEMPLATE<TYPE>& rha ){ lha.x-=rha.x; lha.y-=rha.y; lha.z-=rha.z; return lha;}
	template<typename TYPE>	POINT4D_TEMPLATE<TYPE>& operator-=( POINT4D_TEMPLATE<TYPE>& lha, const VECTOR4D_TEMPLATE<TYPE>& rha ){ lha.x-=rha.x; lha.y-=rha.y; lha.z-=rha.z; lha.w-=rha.w; return lha;}


#ifdef USE_DIRECT3DX9
  void D3DXMathInitialize();
  void D3DXMathFinalize();
#endif


}



#endif