#include"vector2d.h"
#include"vector3d.h"
#include"vector4d.h"

#include"matrix4d.h"
#include"quaternion.h"

#include"../debug/warning.h"
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

      if( Translation==NULL ) { MAID_WARNING( "Translationがありません"); }
      if( RotationX==NULL ) { MAID_WARNING( "RotationXがありません"); }
      if( RotationY==NULL ) { MAID_WARNING( "RotationYがありません"); }
      if( RotationZ==NULL ) { MAID_WARNING( "RotationZがありません"); }
      if( RotationAxis==NULL ) { MAID_WARNING( "RotationAxisがありません"); }
      if( PerspectiveOffCenterLH==NULL ) { MAID_WARNING( "PerspectiveOffCenterLHがありません"); }
      if( PerspectiveFovLH==NULL ) { MAID_WARNING( "PerspectiveFovLHがありません"); }
      if( LookAtLH==NULL ) { MAID_WARNING( "LookAtLHがありません"); }
      if( Inverse==NULL  ) { MAID_WARNING( "Inverseがありません"); }
      if( Multiply==NULL ) { MAID_WARNING( "Multiplyがありません"); }
      if( Vec4Transform==NULL ) { MAID_WARNING( "Vec4Transformがありません"); }
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
    MATRIXROTATIONQUATERNION MatrixRotationQuaternion = NULL;
    MULTIPLY  Multiply = NULL;

    void Initialize( HMODULE hModule )
    {
      RotationAxis = (ROTATIONAXIS)GetProcAddress( hModule, "D3DXQuaternionRotationAxis" );
      Normalize    = (NORMALIZE)GetProcAddress( hModule, "D3DXQuaternionNormalize" );
      Inverse      = (INVERSE)GetProcAddress( hModule, "D3DXQuaternionInverse" );
      MatrixRotationQuaternion = (MATRIXROTATIONQUATERNION)GetProcAddress( hModule, "D3DXMatrixRotationQuaternion" );
      Multiply     = (MULTIPLY)GetProcAddress( hModule, "D3DXQuaternionMultiply" );

      if( RotationAxis==NULL ) { MAID_WARNING( "RotationAxisがありません"); }
      if( Normalize==NULL ) { MAID_WARNING( "Normalizeがありません"); }
      if( Inverse==NULL ) { MAID_WARNING( "Inverseがありません"); }
      if( MatrixRotationQuaternion==NULL ) { MAID_WARNING( "MatrixRotationQuaternionがありません"); }
      if( Multiply==NULL ) { MAID_WARNING( "Multiplyがありません"); }
    }

    void Finalize()
    {
      RotationAxis = NULL;
      Normalize    = NULL;
      Inverse      = NULL;
      MatrixRotationQuaternion = NULL;
      Multiply     = NULL;
    }
  }


  namespace d3dxvector2d
  {
    NORMALIZE Normalize = NULL;

    void Initialize( HMODULE hModule )
    {
      Normalize = (NORMALIZE)GetProcAddress( hModule, "D3DXVec2Normalize" );

      if( Normalize==NULL ) { MAID_WARNING( "Normalizeがありません"); }
    }

    void Finalize()
    {
      Normalize = NULL;
    }
  }

  namespace d3dxvector3d
  {
    NORMALIZE Normalize = NULL;

    void Initialize( HMODULE hModule )
    {
      Normalize = (NORMALIZE)GetProcAddress( hModule, "D3DXVec3Normalize" );
      if( Normalize==NULL ) { MAID_WARNING( "Normalizeがありません"); }
    }

    void Finalize()
    {
      Normalize = NULL;
    }
  }

  namespace d3dxvector4d
  {
    NORMALIZE Normalize = NULL;

    void Initialize( HMODULE hModule )
    {
      Normalize = (NORMALIZE)GetProcAddress( hModule, "D3DXVec4Normalize" );
      if( Normalize==NULL ) { MAID_WARNING( "Normalizeがありません"); }
    }

    void Finalize()
    {
      Normalize = NULL;
    }
  }
}
