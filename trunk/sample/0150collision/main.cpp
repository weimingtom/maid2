#include"../../source/auxiliary/collision.h"
#include"../../source/auxiliary/mathematics.h"
#include"../../source/auxiliary/debug/assert.h"


#include<stdio.h>

#include<vector>

using namespace Maid;


/*
四角形と点、線、四角形、
ボックスと点、線、ボックスのあたり判定
*/

	template<typename TYPE>
  class CollisionEx : public Collision<TYPE>
	{
	public:
    static bool PointRect2d( const POINT2D& pos, const RECT2D& rc, float rad )
    {
      const POINT2D src[4] =
      {
        POINT2D(rc.GetLT() ),
        POINT2D(rc.GetRT() ),
        POINT2D(rc.GetRB() ),
        POINT2D(rc.GetLB() ),
      };

      const POINT2D dst[4] = 
      {
        VectorRotate( VECTOR2D(src[0]), rad ).ConvertPOINT2D(),
        VectorRotate( VECTOR2D(src[1]), rad ).ConvertPOINT2D(),
        VectorRotate( VECTOR2D(src[2]), rad ).ConvertPOINT2D(),
        VectorRotate( VECTOR2D(src[3]), rad ).ConvertPOINT2D(),
      };

      return PointVertex2d( pos, dst, NUMELEMENTS(dst) );
    }

    //  ２次元の点と多角形。vecの周りはそろえておくこと
    static bool PointVertex2d( const POINT2D& pos, const POINT2D* vec, size_t len )
    {
      MAID_ASSERT( len<3, "三角形以上でお願いします" );

      int count = 0;
      int zero  = 0;
      for( size_t i=0; i<len; ++i )
      {
        const VECTOR2D v0 = VECTOR2D(pos,vec[i  ]);
        const VECTOR2D v1 = VECTOR2D(pos,vec[(i+1)%len]);

        const float cross = VectorCross(v0,v1);

        if( cross==0 ) { ++zero;  }
        if( cross<0  ) { ++count; }
      }

      if( count==0 || (count+zero)==len ) { return true; }

      return false;
    }
  };




void main()
{
  const POINT2DI pos(11,0);
  const RECT2DI  rc( POINT2DI(-10,-10), SIZE2DI(20,20) );
  const float rot = DEGtoRAD(0);

  bool ret = CollisionEx<float>::PointRect2d(pos,rc,rot );


  int k=0;

}

