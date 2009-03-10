#include"vector2d.h"
#include"vector3d.h"
#include"vector4d.h"

#include"matrix4d.h"
#include"quaternion.h"

namespace Maid 
{ 
  
  namespace d3dxmatrix 
  {
    TRANSLATION Translation = NULL;
    ROTATIONX RotationX = NULL;
    ROTATIONY RotationY = NULL;
    ROTATIONZ RotationZ = NULL;
    ROTATIONAXIS RotationAxis = NULL;
    PERSPECTIVEOFFCENTERLH PerspectiveOffCenterLH = NULL;
    PERSPECTIVEFOVLH PerspectiveFovLH = NULL;
    LOOKATLH LookAtLH = NULL;
    INVERSE Inverse = NULL;
    MULTIPLY Multiply = NULL;
    VEC4TRANSFORM Vec4Transform = NULL;


    void Initialize( HMODULE hModule )
    {
      Translation = (TRANSLATION)GetProcAddress( hModule, "D3DXMatrixTranslation" );
      RotationX   = (ROTATIONX)GetProcAddress( hModule, "D3DXMatrixRotationX" );
      RotationY   = (ROTATIONY)GetProcAddress( hModule, "D3DXMatrixRotationY" );
      RotationZ   = (ROTATIONZ)GetProcAddress( hModule, "D3DXMatrixRotationZ" );
      RotationAxis = (ROTATIONAXIS)GetProcAddress( hModule, "D3DXMatrixRotationAxis" );
      PerspectiveOffCenterLH = (PERSPECTIVEOFFCENTERLH)GetProcAddress( hModule, "D3DXMatrixPerspectiveOffCenterLH" );
      PerspectiveFovLH  = (PERSPECTIVEFOVLH)GetProcAddress( hModule, "D3DXMatrixPerspectiveFovLH" );
      LookAtLH  = (LOOKATLH)GetProcAddress( hModule, "D3DXMatrixLookAtLH" );
      Inverse   = (INVERSE)GetProcAddress( hModule, "D3DXMatrixInverse" );
      Multiply  = (MULTIPLY)GetProcAddress( hModule, "D3DXMatrixMultiply" );
      Vec4Transform = (VEC4TRANSFORM)GetProcAddress( hModule, "D3DXVec4Transform" );
    }

    void Finalize()
    {
      Translation = NULL;
      RotationX = NULL;
      RotationY = NULL;
      RotationZ = NULL;
      RotationAxis = NULL;
      PerspectiveOffCenterLH = NULL;
      PerspectiveFovLH = NULL;
      LookAtLH = NULL;
      Inverse = NULL;
      Multiply = NULL;
      Vec4Transform = NULL;
    }
  }


  namespace d3dxquaternion
  {
    ROTATIONAXIS RotationAxis = NULL;
    NORMALIZE Normalize = NULL;
    INVERSE   Inverse = NULL;
    LENGTHAQ  LengthSq = NULL;
    MATRIXROTATIONQUATERNION MatrixRotationQuaternion = NULL;
    MULTIPLY  Multiply = NULL;

    void Initialize( HMODULE hModule )
    {
      RotationAxis = (ROTATIONAXIS)GetProcAddress( hModule, "D3DXQuaternionRotationAxis" );
      Normalize    = (NORMALIZE)GetProcAddress( hModule, "D3DXQuaternionNormalize" );
      Inverse      = (INVERSE)GetProcAddress( hModule, "D3DXQuaternionInverse" );
      LengthSq     = (LENGTHAQ)GetProcAddress( hModule, "D3DXQuaternionLengthSq" );
      MatrixRotationQuaternion = (MATRIXROTATIONQUATERNION)GetProcAddress( hModule, "D3DXMatrixRotationQuaternion" );
      Multiply     = (MULTIPLY)GetProcAddress( hModule, "D3DXQuaternionMultiply" );
    }

    void Finalize()
    {
      RotationAxis = NULL;
      Normalize    = NULL;
      Inverse      = NULL;
      LengthSq     = NULL;
      MatrixRotationQuaternion = NULL;
      Multiply     = NULL;
    }
  }


  namespace d3dxvector2d
  {
    NORMALIZE Normalize = NULL;
    DOT Dot = NULL;
    CCW ccw = NULL;

    void Initialize( HMODULE hModule )
    {
      Normalize = (NORMALIZE)GetProcAddress( hModule, "D3DXVec2Normalize" );
      Dot       = (DOT)GetProcAddress( hModule, "D3DXVec2Dot" );
      ccw       = (CCW)GetProcAddress( hModule, "D3DXVec2CCW" );
    }

    void Finalize()
    {
      Normalize = NULL;
      Dot       = NULL;
      ccw       = NULL;
    }
  }

  namespace d3dxvector3d
  {
    NORMALIZE Normalize = NULL;
    DOT   Dot = NULL;
    CROSS Cross = NULL;

    void Initialize( HMODULE hModule )
    {
      Normalize = (NORMALIZE)GetProcAddress( hModule, "D3DXVec3Normalize" );
      Dot       = (DOT)GetProcAddress( hModule, "D3DXVec3Dot" );
      Cross     = (CROSS)GetProcAddress( hModule, "D3DXVec3Cross" );
    }

    void Finalize()
    {
      Normalize = NULL;
      Dot       = NULL;
      Cross     = NULL;
    }
  }

  namespace d3dxvector4d
  {
    NORMALIZE Normalize = NULL;
    DOT Dot = NULL;

    void Initialize( HMODULE hModule )
    {
      Normalize = (NORMALIZE)GetProcAddress( hModule, "D3DXVec4Normalize" );
      Dot       = (DOT)GetProcAddress( hModule, "D3DXVec4Dot" );
    }

    void Finalize()
    {
      Normalize = NULL;
      Dot       = NULL;
    }
  }
}
