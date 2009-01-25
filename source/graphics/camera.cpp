#include"camera.h"
#include"../auxiliary/collision.h"


namespace Maid
{

/*!
    @class	Camera camera.h
    @brief	カメラクラス
\n          ビューマトリクス、プロジェクションマトリクスを管理する
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 *
 */
Camera::Camera()
  :m_IsCalcViewMatrix(false)
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 *
 */
Camera::~Camera()
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 透視射影マトリクスの作成
/*!
    @param  Fov     [i ]	カメラの視野角
    @param  Aspect  [i ]	スクリーンのアスペクト比(普通はWidth/Height)
    @param  Near    [i ]	クリップする手前の距離
    @param  Far     [i ]	クリップする奥の距離
 */
void Camera::SetPerspective( float Fov, float Aspect, float Near, float Far )
{
  m_ProjectionMatrix.SetPerspective(Fov,Aspect,Near,Far );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 正射影マトリクスの作成
/*!
    @param  Fov     [i ]  カメラの視野角
    @param  Aspect  [i ]  スクリーンのアスペクト比
    @param  Near    [i ]  クリップする手前の距離
    @param  Far     [i ]  クリップする奥の距離
    @param  ProjectionDepth [i ]  実際に射影する位置。カメラから見た座標はすべて
                                    (x,y,ProjectionDepth) にあるものとして描画される
 */
void Camera::SetOrthogonal( float Fov, float Aspect, float Near, float Far, float ProjectionDepth )
{
  MATRIX4DF m;

  m.SetOrthogonal( Fov, Aspect, ProjectionDepth, Far );

  const float CenterD = (ProjectionDepth-Near) / (Far - Near);
  m_ProjectionMatrix = m * MATRIX4DF().SetTranslate(0,0,CenterD);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ３Ｄ空間中で２Ｄ表示に便利なカメラ設定をする
/*!
    点(0,                      0,0)　がスクリーン座標の(0,0) に
\n  点(0,          -ScreenHeight,0)　がスクリーン座標の(0,ScreenHeight) に
\n  点(ScreenWidth,-ScreenHeight,0)　がスクリーン座標の(ScreenWidth,ScreenHeight) に
\n  なるようにカメラ座標が計算されます
 
    @param  ScreenWidth   [i ]  画面横サイズ
    @param  ScreenHeight  [i ]  画面縦サイズ
    @param  Near          [i ]  画面手前の距離
    @param  Far           [i ]  画面奥の距離
 */
void Camera::Set2DPosition( int ScreenWidth, int ScreenHeight, float Near, float Far )
{
  const float Width  = float(ScreenWidth /2);
  const float Height = float(ScreenHeight/2);
  const float Fovy   = DEGtoRAD(60.0f);
  const float Depth  = Height/Math<float>::tan(Fovy/2.0f);

  const POINT3DF DefaultPosition( Width, Height, -Depth );
  const POINT3DF DefaultTarget  ( Width, Height,      0 );

  SetPerspective( Fovy, Width/Height, Near, Far );
  SetPosition( DefaultPosition );
  SetTarget  ( DefaultTarget );
  SetUpVector( VECTOR3DF( 0, 1, 0) );
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! カメラのある座標の設定
/*!
    @param  pos [i ]  新しく設定する座標
 */
void Camera::SetPosition( const POINT3DF&  pos )
{
  m_Pos = pos;
  m_IsCalcViewMatrix = false;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! カメラの注視座標の設定
/*!
  @param  pos [i ]  新しく設定する座標
 */
void Camera::SetTarget  ( const POINT3DF&  pos )
{
  m_Target = pos;
  m_IsCalcViewMatrix = false;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! カメラの上方向の設定
/*!
 	@param	vec	[i ]	新しく設定する座標
 */
void Camera::SetUpVector( const VECTOR3DF& vec )
{
  m_Up = vec;
  m_Up.Normalize();
  m_IsCalcViewMatrix = false;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ビューマトリクスの取得
/*!
    @return ビューマトリクス
 */
const MATRIX4DF& Camera::GetViewMatrix() const
{
  if( !m_IsCalcViewMatrix )
  {
    Camera* pThis = const_cast<Camera*>(this);
    pThis->m_ViewMatrix.SetLookAt( m_Pos, m_Target, m_Up );

    pThis->m_IsCalcViewMatrix = true;
  }

  return m_ViewMatrix;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! プロジェクションマトリクスの取得
/*!
    @return プロジェクションマトリクス
 */
const MATRIX4DF& Camera::GetProjectionMatrix() const
{
  return m_ProjectionMatrix;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! カメラの移動
/*!
  注視座標も同じ方向に移動します

  @param  offset  [i ]  移動量
 */
void  Camera::Move( const VECTOR3DF& offset )
{
  Move( offset, offset );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! カメラの移動
/*!
    @param  pos     [i ]  カメラの座標の移動量
    @param  target  [i ]  注視座標の移動量
 */
void  Camera::Move( const VECTOR3DF& pos, const VECTOR3DF& target )
{
  m_Pos    += pos;
  m_Target += target;
  m_IsCalcViewMatrix = false;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! カメラ座標の取得
/*!
    @return カメラ座標
 */
const POINT3DF&  Camera::GetPosition() const
{
  return m_Pos;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 注視座標の取得
/*!
    @return 注視座標
 */
const POINT3DF&  Camera::GetTarget  () const
{
  return m_Target;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! カメラの上方向の取得
/*!
    @return カメラの上方向
 */
const VECTOR3DF& Camera::GetUpVector() const
{
  return m_Up;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定した３Ｄ座標がカメラ内に入っているか調べる
/*!
    @param  pos 調べる座標

    @return 作っているなら true
 */
bool Camera::IsCameraIn( const POINT3DF& pos ) const
{
  const MATRIX4DF& CameraMat = GetViewMatrix();
  const MATRIX4DF& ProjecMat = GetProjectionMatrix();

  MATRIX4DF vp = CameraMat * ProjecMat;

  const float x = pos.x*vp.m11 + pos.y*vp.m21 + pos.z*vp.m31 + vp.m41;
  const float y = pos.x*vp.m12 + pos.y*vp.m22 + pos.z*vp.m32 + vp.m42;
  const float z = pos.x*vp.m13 + pos.y*vp.m23 + pos.z*vp.m33 + vp.m43;

  POINT3DF p( x/z, y/z, ProjecMat.m33+ProjecMat.m43/z );

  return Collision<float>::IsPointToRect( p.x, p.y, p.z, -1, -1, -1, 1, 1, 1 );
}

}