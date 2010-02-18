/*!
    @file
    @brief	4x4マトリックステンプレート
 */



#ifndef maid2_auxiliary_mathematics_matrix4d_h
#define maid2_auxiliary_mathematics_matrix4d_h

#include"../../config/define.h"


#include"rect2d.h"
#include"point3d.h"
#include"point4d.h"
#include"vector3d.h"
#include"vector4d.h"
#include"function.h"

namespace Maid
{ 
  /*!
      @brief	4x4マトリックスを保持する構造体
   */
  template<typename TYPE>
  struct MATRIX4D_TEMPLATE
  { 
    typedef RECT2D_TEMPLATE<TYPE> RECT2;
    typedef POINT3D_TEMPLATE<TYPE> POINT3;
    typedef POINT4D_TEMPLATE<TYPE> POINT4;
    typedef VECTOR3D_TEMPLATE<TYPE> VECTOR3;
    typedef VECTOR4D_TEMPLATE<TYPE> VECTOR4;
    typedef MATRIX4D_TEMPLATE<TYPE> MATRIX;

    /*!
        @brief	配列、メンバの両方から指定できるようにする
     */
    union {
      /*!	@brief	こっちはメンバ	 */
      struct {
        TYPE        m11, m12, m13, m14;
        TYPE        m21, m22, m23, m24;
        TYPE        m31, m32, m33, m34;
        TYPE        m41, m42, m43, m44;
      };
      TYPE m[4][4];	//!<	こっちは配列
    };


	  //!	コンストラクタ
	  MATRIX4D_TEMPLATE(){};
	  //!	コンストラクタ
	  MATRIX4D_TEMPLATE(	TYPE s11,TYPE s12,TYPE s13,TYPE s14,
	                  TYPE s21,TYPE s22,TYPE s23,TYPE s24,
	                  TYPE s31,TYPE s32,TYPE s33,TYPE s34,
	                  TYPE s41,TYPE s42,TYPE s43,TYPE s44 )
	                  :m11(s11), m12(s12), m13(s13), m14(s14),
	                   m21(s21), m22(s22), m23(s23), m24(s24),
	                   m31(s31), m32(s32), m33(s33), m34(s34),
	                   m41(s41), m42(s42), m43(s43), m44(s44)
	  {

	  }


	  //! 単位行列をセットする
	  /*!
	   */
	  const MATRIX& SetIdentity()
	  {
      m11=1;m12=0;m13=0;m14=0;
      m21=0;m22=1;m23=0;m24=0;
      m31=0;m32=0;m33=1;m34=0;
      m41=0;m42=0;m43=0;m44=1;

      return *this;
	  };


	  //! スケール行列をセットする
	  /*!
  	 	  @param	s	[i ]	スケール値
	   */
	  const MATRIX& SetScale(TYPE s)
	  {
      SetScale( s, s, s );
      return *this;
	  }

	  //! スケール行列をセットする
	  /*!
        @param	x	[i ]	スケール値
        @param	y	[i ]	スケール値
        @param	z	[i ]	スケール値
	   */
	  const MATRIX& SetScale(TYPE x, TYPE y, TYPE z)
	  {
      m11=x; m12=0; m13=0; m14=0;
      m21=0; m22=y; m23=0; m24=0;
      m31=0; m32=0; m33=z; m34=0;
      m41=0; m42=0; m43=0; m44=1;
      return *this;
    }

	  //! 移動行列をセットする
	  /*!
        @param	x	[i ]	Ｘ軸の移動量
        @param	y	[i ]	Ｙ軸の移動量
        @param	z	[i ]	Ｚ軸の移動量
	   */
	  const MATRIX& SetTranslate(TYPE x,TYPE y,TYPE z)
	  {
      m11=1;m12=0;m13=0;m14=0;
      m21=0;m22=1;m23=0;m24=0;
      m31=0;m32=0;m33=1;m34=0;
      m41=x;m42=y;m43=z;m44=1;
      return *this;
	  }

	  //! Ｘ軸回転行列をセットする
	  /*!
        @param	rad	[i ]	回転量（ラジアン）
	   */
	  const MATRIX& SetRotationX(TYPE rad)
	  {
      const TYPE s = Math<TYPE>::sin(rad);
      const TYPE c = Math<TYPE>::cos(rad);
      m11=1; m12= 0; m13=0; m14=0;
      m21=0; m22= c; m23=s; m24=0;
      m31=0; m32=-s; m33=c; m34=0;
      m41=0; m42= 0; m43=0; m44=1;
      return *this;
	  }


	  //! Ｙ軸回転行列をセットする
	  /*!
        @param	rad	[i ]	回転量（ラジアン）
	   */
	  const MATRIX& SetRotationY(TYPE rad)
	  {
      const TYPE s = Math<TYPE>::sin(rad);
      const TYPE c = Math<TYPE>::cos(rad);

      m11=c; m12=0; m13=-s; m14=0;
      m21=0; m22=1; m23= 0; m24=0;
      m31=s; m32=0; m33= c; m34=0;
      m41=0; m42=0; m43= 0; m44=1;
      return *this;
	  }

	  //! Ｚ軸回転行列をセットする
	  /*!
        @param	rad	[i ]	回転量（ラジアン）
	   */
	  const MATRIX& SetRotationZ(TYPE rad)
	  {
      const TYPE s = Math<TYPE>::sin(rad);
      const TYPE c = Math<TYPE>::cos(rad);

      m11=c ; m12=s; m13=0; m14=0;
      m21=-s; m22=c; m23=0; m24=0;
      m31=0 ; m32=0; m33=1; m34=0;
      m41=0 ; m42=0; m43=0; m44=1;
      return *this;
	  }

	  //! 任意軸回転行列をセットする
	  /*!
        @param	rad	[i ]	回転量（ラジアン）
        @param	vec	[i ]	基準にするベクトル（正規化しておくこと）
	   */
	  const MATRIX& SetRotationXYZ( TYPE rad, const VECTOR3& vec )
	  {
      /*
        任意ベクトルR(Rx, Ry, Rz)を軸としてT回転する行列。

        | (1-cosT)*Rx*Rx +    cosT, (1-cosT)*Rx*Ry - Rz*sinT, (1-cosT)*Rx*Rz + Ry*sinT, 0 |
        | (1-cosT)*Rx*Ry + Rz*sinT, (1-cosT)*Ry*Ry +    cosT, (1-cosT)*Ry*Rz - Rx*sinT, 0 |
        | (1-cosT)*Rx*Rz - Ry*sinT, (1-cosT)*Ry*Rz + Rx*sinT, (1-cosT)*Rz*Rz +    cosT, 0 |
        |                        0,                        0,                        0, 1 |
      */
      const TYPE sinT = Math<TYPE>::sin(rad);
      const TYPE cosT = Math<TYPE>::cos(rad);
      const TYPE ac= 1-cosT;
      const TYPE Rx= vec.x;
      const TYPE Ry= vec.y;
      const TYPE Rz= vec.z;

      m11=ac*Rx*Rx +    cosT; m12=ac*Rx*Ry - Rz*sinT; m13=ac*Rx*Rz + Ry*sinT; m14=0;
      m21=ac*Rx*Ry + Rz*sinT; m22=ac*Ry*Ry +    cosT; m23=ac*Ry*Rz - Rx*sinT; m24=0;
      m31=ac*Rx*Rz - Ry*sinT; m32=ac*Ry*Rz + Rx*sinT; m33=ac*Rz*Rz +    cosT; m34=0;
      m41=                 0; m42=                 0; m43=                 0; m44=1;
      return *this;
	  }

	  //! 透視投影変換行列をセットする
	  /*!
        @param	Left	[i ]	左のカリング座標
        @param	Right	[i ]	右のカリング座標
        @param	Top		[i ]	上のカリング座標
        @param	Bottom	[i ]	下のカリング座標
        @param	Near	[i ]	手前のカリング座標
        @param	Far		[i ]	奥のカリング座標
	   */
	  const MATRIX& SetFrustum(TYPE Left, TYPE Right,TYPE Top,TYPE Bottom,TYPE Near,TYPE Far )
	  {
      const TYPE w = Right - Left;
      const TYPE h = Top - Bottom;
      const TYPE d = Far - Near;

      m11=      2*Near/w; m12=             0; m13=            0; m14=0;
      m21=             0; m22=      2*Near/h; m23=            0; m24=0;
      m31=(Right+Left)/w; m32=(Top+Bottom)/h; m33=Far/d; m34=1;
      m41=             0; m42=             0; m43=-Far*Near/d; m44=0;
      return *this;
	  }

	  //! 透視投影変換行列をセットする
	  /*!
        @param	Fovx	[i ]	視野 (ラジアン)
        @param	Aspect	[i ]	アスペクト比(width/height)
        @param	Near	[i ]	手前のカリング座標
        @param	Far		[i ]	奥のカリング座標
	   */
	  const MATRIX& SetPerspective(TYPE Fovx, TYPE Aspect,TYPE Near,TYPE Far)
	  {
      const TYPE Top		= Near * TYPE(Math<TYPE>::tan(Fovx/2));
      const TYPE Bottom	= -Top;
      const TYPE Right	= Top * Aspect;
      const TYPE Left		= -Right;
      return SetFrustum(Left,Right,Top,Bottom,Near,Far);
	  }

	  //! 平行投影変換行列をセットする
	  /*!
        @param	Fovx	[i ]	視野 (ラジアン)
        @param	Aspect	[i ]	アスペクト比(width/height)
        @param	Near	[i ]	手前のカリング座標
        @param	Far		[i ]	奥のカリング座標
	   */
	  const MATRIX& SetOrthogonal( TYPE Fovx, TYPE Aspect,TYPE Near,TYPE Far )
	  {
      const TYPE Top		= Near * Math<TYPE>::tan(Fovx/2);
      const TYPE Bottom	= -Top;
      const TYPE Right	= Top * Aspect;
      const TYPE Left	= -Right;

      const TYPE w = Right - Left;
      const TYPE h = Top - Bottom;
      const TYPE d = Far - Near;

      m11=           2/w; m12=             0; m13=              0; m14=0;
      m21=             0; m22=           2/h; m23=              0; m24=0;
      m31=             0; m32=             0; m33=            1/d; m34=0;
      m41=(Right+Left)/w; m42=(Top+Bottom)/h; m43=Near/(Near-Far); m44=1;

      return *this;
	  }

	  //! ビュー行列をセットする
	  /*!
        @param	Eye		[i ]	カメラの位置
        @param	Target	[i ]	カメラの注視点
        @param	Up		[i ]	カメラの上の向き
	   */
	  const MATRIX& SetLookAt(const POINT3& Eye, const POINT3& Target, const VECTOR3& Up)
	  {
      VECTOR3 w,u,v,d;

      {	// Ｚ軸ベクトルの計算
        w.Convert(Eye,Target);
        w.Normalize();
      }
      {	// Ｘ軸ベクトルの計算
        u = VectorCross( w, Up );
        u.Normalize();
      }
      {	// Ｙ軸ベクトルの計算
        v = VectorCross( u, w );
        v.Normalize();
      }
      {
        VECTOR3 k;
        k.Convert(Eye);

        d.x = -VectorDot( k, u );
        d.y = -VectorDot( k, v );
        d.z = -VectorDot( k, w );
      }

      m11=u.x; m12=v.x; m13=w.x; m14=0;
      m21=u.y; m22=v.y; m23=w.y; m24=0;
      m31=u.z; m32=v.z; m33=w.z; m34=0;
      m41=d.x; m42=d.y; m43=d.z; m44=1;
      return *this;
	  }

	  //! ビューポート行列をセットする
	  /*!
        @param	rc		[i ]	表示範囲
        @param	MinZ	[i ]	Zの最小値
        @param	MaxZ	[i ]	Zの最大値
	   */
	  const MATRIX& SetViewport(const RECT2& rc, const TYPE MinZ, const TYPE MaxZ )
	  {
      const TYPE x = rc.Pos.x;
      const TYPE y = rc.Pos.y;
      const TYPE w = rc.Size.Width;
      const TYPE h = rc.Size.Height;

      m11=  w/2; m12=    0; m13=        0; m14=0;
      m21=    0; m22= -h/2; m23=        0; m24=0;
      m31=    0; m32=    0; m33=MaxZ-MinZ; m34=0;
      m41=x+w/2; m42=h/2+y; m43=     MinZ; m44=1;
      return *this;
	  }

	  //! 転置行列の作成
	  /*!
        @return 転置行列
	   */
	  MATRIX	GetTranspose() const
	  {
      return MATRIX
        (
	        m11, m21, m31, m41,
	        m12, m22, m32, m42,
	        m13, m23, m33, m43,
	        m14, m24, m34, m44
        );
	  }
	  //! 逆行列の作成
	  /*!
        @return 逆行列
	   */
	  MATRIX	GetInverse() const
	  {
      MATRIX tmp1 = *this;
      MATRIX tmp2;

      tmp2.SetIdentity();

      for( sint32 i = 0; i<4; i++)
      {
        for( sint32 j = i; j < 4; j++)
        {
          if(tmp1.m[j][i] != 0)
          {
            const TYPE a = tmp1.m[j][i];
            tmp1.MulRow(j, 1 / a);
            tmp2.MulRow(j, 1 / a);
          }
        }
        for( sint32 k = i+1; k<4; k++)
        {
          if(tmp1.m[k][i] != 0)
          {
            tmp1.SubRow(k, tmp1.GetRow(i));
            tmp2.SubRow(k, tmp2.GetRow(i));
          }
        }
      }

      for( sint32 i = 4-1; i>= 0; i--)
      {
        for(sint32 j = i; j >= 0; j--)
        {
          if(tmp1.m[j][i] != 0)
          {
            const TYPE a = tmp1.m[j][i];
            tmp1.MulRow(j, 1 / a);
            tmp2.MulRow(j, 1 / a);
          }
        }
        for(sint32 k = i - 1; k >= 0; k--)
        {
          if(tmp1.m[k][i] != 0)
          {
            tmp1.SubRow(k, tmp1.GetRow(i));
            tmp2.SubRow(k, tmp2.GetRow(i));
          }
        }
      }
      return tmp2;
	  }


	  //! 指定した行(横方向）を定数倍する
	  void MulRow( int row, TYPE a)
	  {
      m[row][0] *= a;
      m[row][1] *= a;
      m[row][2] *= a;
      m[row][3] *= a;
	  }

	  //! 指定した列（縦方向）を定数倍する
	  void MulCol( int col, TYPE a)
	  {
      m[0][col] *= a;
      m[1][col] *= a;
      m[2][col] *= a;
      m[3][col] *= a;
	  }

	  //! 指定した行(横方向）を加算する
	  void AddRow(int row, const VECTOR4& v)
	  {
      m[row][0] += v.x;
      m[row][1] += v.y;
      m[row][2] += v.z;
      m[row][3] += v.w;
	  }

	  //! 指定した列（縦方向）を加算する
	  void AddCol(int row, const VECTOR4& v)
	  {
      m[0][col] += v.x;
      m[1][col] += v.y;
      m[2][col] += v.z;
      m[3][col] += v.w;
	  }

	  //! 指定した行(横方向）を減算する
	  void SubRow(int row, const VECTOR4& v)
	  {
      m[row][0] -= v.x;
      m[row][1] -= v.y;
      m[row][2] -= v.z;
      m[row][3] -= v.w;
	  }

	  VECTOR4 GetRow(int row) const
	  {
      VECTOR4 tmp;
      tmp.x = m[row][0];
      tmp.y = m[row][1];
      tmp.z = m[row][2];
      tmp.w = m[row][3];

      return tmp;
	  }

	  VECTOR4 GetCol(int col) const
	  {
      VECTOR4 tmp;
      tmp.x = m[0][col];
      tmp.y = m[1][col];
      tmp.z = m[2][col];
      tmp.w = m[3][col];

      return tmp;
	  }
  };


  template<typename TYPE>
  inline MATRIX4D_TEMPLATE<TYPE> operator*( const MATRIX4D_TEMPLATE<TYPE>& lha, const MATRIX4D_TEMPLATE<TYPE>& rha )
  {
    MATRIX4D_TEMPLATE<TYPE> r;

    r.m11 = lha.m11*rha.m11 + lha.m12*rha.m21 + lha.m13*rha.m31 + lha.m14*rha.m41;
    r.m21 = lha.m21*rha.m11 + lha.m22*rha.m21 + lha.m23*rha.m31 + lha.m24*rha.m41;
    r.m31 = lha.m31*rha.m11 + lha.m32*rha.m21 + lha.m33*rha.m31 + lha.m34*rha.m41;
    r.m41 = lha.m41*rha.m11 + lha.m42*rha.m21 + lha.m43*rha.m31 + lha.m44*rha.m41;

    r.m12 = lha.m11*rha.m12 + lha.m12*rha.m22 + lha.m13*rha.m32 + lha.m14*rha.m42;
    r.m22 = lha.m21*rha.m12 + lha.m22*rha.m22 + lha.m23*rha.m32 + lha.m24*rha.m42;
    r.m32 = lha.m31*rha.m12 + lha.m32*rha.m22 + lha.m33*rha.m32 + lha.m34*rha.m42;
    r.m42 = lha.m41*rha.m12 + lha.m42*rha.m22 + lha.m43*rha.m32 + lha.m44*rha.m42;

    r.m13 = lha.m11*rha.m13 + lha.m12*rha.m23 + lha.m13*rha.m33 + lha.m14*rha.m43;
    r.m23 = lha.m21*rha.m13 + lha.m22*rha.m23 + lha.m23*rha.m33 + lha.m24*rha.m43;
    r.m33 = lha.m31*rha.m13 + lha.m32*rha.m23 + lha.m33*rha.m33 + lha.m34*rha.m43;
    r.m43 = lha.m41*rha.m13 + lha.m42*rha.m23 + lha.m43*rha.m33 + lha.m44*rha.m43;

    r.m14 = lha.m11*rha.m14 + lha.m12*rha.m24 + lha.m13*rha.m34 + lha.m14*rha.m44;
    r.m24 = lha.m21*rha.m14 + lha.m22*rha.m24 + lha.m23*rha.m34 + lha.m24*rha.m44;
    r.m34 = lha.m31*rha.m14 + lha.m32*rha.m24 + lha.m33*rha.m34 + lha.m34*rha.m44;
    r.m44 = lha.m41*rha.m14 + lha.m42*rha.m24 + lha.m43*rha.m34 + lha.m44*rha.m44;

    return r;
  }

  template<typename TYPE>
  inline MATRIX4D_TEMPLATE<TYPE>& operator*=( MATRIX4D_TEMPLATE<TYPE>& lha, const MATRIX4D_TEMPLATE<TYPE>& rha )
  {
    lha = lha*rha;
    return lha;
  }

  template<typename TYPE>
  inline VECTOR4D_TEMPLATE<TYPE> operator*( const VECTOR4D_TEMPLATE<TYPE>& lha, const MATRIX4D_TEMPLATE<TYPE>& rha )
  {
    VECTOR4 r;

    r.x = lha.x*rha.m11 + lha.y*rha.m21 + lha.z*rha.m31 + lha.w*rha.m41;
    r.y = lha.x*rha.m12 + lha.y*rha.m22 + lha.z*rha.m32 + lha.w*rha.m42;
    r.z = lha.x*rha.m13 + lha.y*rha.m23 + lha.z*rha.m33 + lha.w*rha.m43;
    r.w = lha.x*rha.m14 + lha.y*rha.m24 + lha.z*rha.m34 + lha.w*rha.m44;

    return r;
  }

  template<typename TYPE>
  inline VECTOR4D_TEMPLATE<TYPE>& operator*=( VECTOR4D_TEMPLATE<TYPE>& lha, const MATRIX4D_TEMPLATE<TYPE>& rha )
  {
    lha = lha*rha;
    return lha;
  }

}
	//	Windows のときのみ DXLIB を使ったバージョンを存在させる
#ifdef USE_DIRECT3DX9
  #include"../../config/win32.h"
  #include<d3dx9.h>

  namespace Maid
  {
    namespace d3dxmatrix
    {
      typedef D3DXMATRIX* (WINAPI *TRANSLATION)(D3DXMATRIX*,FLOAT,FLOAT,FLOAT);
      typedef D3DXMATRIX* (WINAPI *ROTATIONX)(D3DXMATRIX*,FLOAT);
      typedef D3DXMATRIX* (WINAPI *ROTATIONY)(D3DXMATRIX*,FLOAT);
      typedef D3DXMATRIX* (WINAPI *ROTATIONZ)(D3DXMATRIX*,FLOAT);
      typedef D3DXMATRIX* (WINAPI *ROTATIONAXIS)(D3DXMATRIX*,CONST D3DXVECTOR3*,FLOAT);
      typedef D3DXMATRIX* (WINAPI *PERSPECTIVEOFFCENTERLH)(D3DXMATRIX*,FLOAT,FLOAT,FLOAT,FLOAT,FLOAT,FLOAT);
      typedef D3DXMATRIX* (WINAPI *PERSPECTIVEFOVLH)(D3DXMATRIX*,FLOAT,FLOAT,FLOAT,FLOAT);
      typedef D3DXMATRIX* (WINAPI *LOOKATLH)(D3DXMATRIX*,CONST D3DXVECTOR3*,CONST D3DXVECTOR3*,CONST D3DXVECTOR3*);
      typedef D3DXMATRIX* (WINAPI *INVERSE)(D3DXMATRIX*,FLOAT*,CONST D3DXMATRIX *);
      typedef D3DXMATRIX* (WINAPI *MULTIPLY)(D3DXMATRIX*,CONST D3DXMATRIX*,CONST D3DXMATRIX *);
      typedef D3DXVECTOR4*(WINAPI *VEC4TRANSFORM)(D3DXVECTOR4*,CONST D3DXVECTOR4*,CONST D3DXMATRIX*);
      
      extern TRANSLATION Translation;
      extern ROTATIONX RotationX;
      extern ROTATIONY RotationY;
      extern ROTATIONZ RotationZ;
      extern ROTATIONAXIS RotationAxis;
      extern PERSPECTIVEOFFCENTERLH PerspectiveOffCenterLH;
      extern PERSPECTIVEFOVLH PerspectiveFovLH;
      extern LOOKATLH LookAtLH;
      extern INVERSE Inverse;
      extern MULTIPLY Multiply;
      extern VEC4TRANSFORM Vec4Transform;

      void Initialize( HMODULE hModule );
      void Finalize();
    }

    const MATRIX4D_TEMPLATE<FLOAT>& MATRIX4D_TEMPLATE<FLOAT>::SetScale(FLOAT s)
    {
      return MATRIX4D_TEMPLATE<FLOAT>::SetScale( s, s, s );
    }

    const MATRIX4D_TEMPLATE<FLOAT>& MATRIX4D_TEMPLATE<FLOAT>::SetTranslate(FLOAT x,FLOAT y,FLOAT z)
    {
      d3dxmatrix::Translation( (D3DXMATRIX*)this, x, y, z );
      return *this;
    }
    const MATRIX4D_TEMPLATE<FLOAT>& MATRIX4D_TEMPLATE<FLOAT>::SetRotationX(FLOAT rad)
    {
      d3dxmatrix::RotationX( (D3DXMATRIX*)this, rad );
      return *this;
    }
    const MATRIX4D_TEMPLATE<FLOAT>& MATRIX4D_TEMPLATE<FLOAT>::SetRotationY(FLOAT rad)
    {
      d3dxmatrix::RotationY( (D3DXMATRIX*)this, rad );
      return *this;
    }
    const MATRIX4D_TEMPLATE<FLOAT>& MATRIX4D_TEMPLATE<FLOAT>::SetRotationZ(FLOAT rad)
    {
      d3dxmatrix::RotationZ( (D3DXMATRIX*)this, rad );
      return *this;
    }
    const MATRIX4D_TEMPLATE<FLOAT>& MATRIX4D_TEMPLATE<FLOAT>::SetRotationXYZ( FLOAT rad, const VECTOR3D_TEMPLATE<FLOAT>& vec )
    {
      d3dxmatrix::RotationAxis( (D3DXMATRIX*)this, (D3DXVECTOR3*)&vec, rad );
      return *this;
    }
    const MATRIX4D_TEMPLATE<FLOAT>& MATRIX4D_TEMPLATE<FLOAT>::SetFrustum(FLOAT Left, FLOAT Right,FLOAT Top,FLOAT Bottom,FLOAT Near,FLOAT Far )
    {
      d3dxmatrix::PerspectiveOffCenterLH( (D3DXMATRIX*)this, Left, Right, Top, Bottom, Near, Far );
      return *this;
    }

    const MATRIX4D_TEMPLATE<FLOAT>& MATRIX4D_TEMPLATE<FLOAT>::SetPerspective(FLOAT Fovx, FLOAT Aspect,FLOAT Near,FLOAT Far)
    {
      d3dxmatrix::PerspectiveFovLH( (D3DXMATRIX*)this, Fovx, Aspect, Near, Far );
      return *this;
    }
    const MATRIX4D_TEMPLATE<FLOAT>& MATRIX4D_TEMPLATE<FLOAT>::SetLookAt(const POINT3D_TEMPLATE<FLOAT>& Eye, const POINT3D_TEMPLATE<FLOAT>& Target, const VECTOR3D_TEMPLATE<FLOAT>& Up)
    {
      d3dxmatrix::LookAtLH( (D3DXMATRIX*)this, (D3DXVECTOR3*)&Eye, (D3DXVECTOR3*)&Target, (D3DXVECTOR3*)&Up );
      return *this;
    }
    MATRIX4D_TEMPLATE<FLOAT>	MATRIX4D_TEMPLATE<FLOAT>::GetInverse() const
    {
      MATRIX4D_TEMPLATE<FLOAT> tmp;
      d3dxmatrix::Inverse( (D3DXMATRIX*)&tmp, NULL, (D3DXMATRIX*)this );
      return tmp;
    }

    inline MATRIX4D_TEMPLATE<FLOAT> operator*( const MATRIX4D_TEMPLATE<FLOAT>& lha, const MATRIX4D_TEMPLATE<FLOAT>& rha )
    {
      MATRIX4D_TEMPLATE<FLOAT> r;
      d3dxmatrix::Multiply( (D3DXMATRIX*)&r, (D3DXMATRIX*)&lha, (D3DXMATRIX*)&rha );
      return r;
    }
    inline VECTOR4D_TEMPLATE<FLOAT> operator*( const VECTOR4D_TEMPLATE<FLOAT>& lha, const MATRIX4D_TEMPLATE<FLOAT>& rha )
    {
      VECTOR4D_TEMPLATE<FLOAT> r;
      d3dxmatrix::Vec4Transform( (D3DXVECTOR4*)&r, (D3DXVECTOR4*)&lha, (D3DXMATRIX*)&rha );
      return r;
    }
	}
#endif



#endif
