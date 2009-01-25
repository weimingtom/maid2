#ifndef maid2_graphics_camera_h
#define maid2_graphics_camera_h

/*!
    @file
    @brief	３Ｄカメラ
 */

#include"../config/define.h"
#include"../auxiliary/mathematics/point.h"
#include"../auxiliary/mathematics/vector.h"
#include"../auxiliary/mathematics/matrix.h"

namespace Maid
{
	class Camera
	{
	public:
		Camera();
		virtual ~Camera();

		void SetPerspective( float Fov, float Aspect, float Near, float Far );
		void SetOrthogonal( float Fov, float Aspect, float Near, float Far, float ProjectionDepth );

		void SetPosition( const POINT3DF&  pos );
		void SetTarget  ( const POINT3DF&  pos );
		void SetUpVector( const VECTOR3DF& vec );

		void Set2DPosition( int Width, int Height, float Near, float Far );

		const MATRIX4DF& GetViewMatrix() const;
		const MATRIX4DF& GetProjectionMatrix() const;

		void  Move( const VECTOR3DF& offset );
		void  Move( const VECTOR3DF& pos, const VECTOR3DF& target );

		const POINT3DF&  GetPosition() const;
		const POINT3DF&  GetTarget  () const;
		const VECTOR3DF& GetUpVector() const;

		bool	IsCameraIn( const POINT3DF& pos )const;

	private:
		POINT3DF	m_Pos;		// カメラの位置
		POINT3DF	m_Target;	// カメラの注目点
		VECTOR3DF	m_Up;		// 上の向き

		MATRIX4DF	m_ViewMatrix;
		MATRIX4DF	m_ProjectionMatrix;

		bool	m_IsCalcViewMatrix;	//!<	m_ViewMatrixを再計算したか？
	};


}
#endif