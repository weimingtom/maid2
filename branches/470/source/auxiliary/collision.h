#ifndef maid2_auxiliary_collision_h
#define maid2_auxiliary_collision_h

/*!
 	@file
 	@brief いろいろな当たり判定を行う関数
  
 	@par 参考文献
 \n		 著書　コンピューターグラフィックス　理論と実践
 \n		 ISBN  4-274-06405-0
 \n		 出版　オーム社
  
  	@par 使用例
 	@code
 	if( Maid::Collision<sint32>::IsPointToRect( 0, 0, -100, -100, 100, 100 ) )
 	{
 		//	あたった時の処理
 	}else
 	{
 		//	はずしたときの処理
 	}
 	@endcode
 */
#include"../config/define.h"
#include"../config/typedef.h"
#include"macro.h"

#include"mathematics.h"

namespace Maid
{
	/*!
	 	@class Collision collision.h
	 	@brief いろいろな当たり判定を行う関数
	 */

	template<typename TYPE>
	class Collision
	{
	public:

		typedef LINE2D_TEMPLATE<TYPE> LINE2D;
		typedef POINT2D_TEMPLATE<TYPE> POINT2D;
		typedef VECTOR2D_TEMPLATE<TYPE> VECTOR2D;
		typedef RECT2D_TEMPLATE<TYPE> RECT2D;

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 点 to 四角形
		/*!
		 	@param  x			[i ]	点の座標
		 	@param  y			[i ]	点の座標
		 	@param  rect		[i ]	四角形の座標
		 
		  @return 四角形の中に点があるなら true
		\n        四角形の中に点がないなら false
		 */
		static bool IsPointToRect( const POINT2D& pos, const RECT2D_TEMPLATE<TYPE>& rect )
		{
			return IsPointToRect( pos.x, pos.y, rect.x, rect.y, rect.GetRight(), rect.GetBottom() );
		}

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 点 to 四角形
		/*!
		 	@param  x			[i ]	点の座標
		 	@param  y			[i ]	点の座標
		 	@param  rect		[i ]	四角形の座標
		 
		    @return 四角形の中に点があるなら true
		\n			四角形の中に点がないなら false
		 */
		static bool IsPointToRect( TYPE x, TYPE y, const RECT2D_TEMPLATE<TYPE>& rect )
		{
			return IsPointToRect( x, y, rect.x, rect.y, rect.GetRight(), rect.GetBottom() );
		}

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 点 to 四角形
		/*!
		 	@param  x			[i ]	点の座標
		 	@param  y			[i ]	点の座標
		 	@param  left		[i ]	四角形の座標
		 	@param  top			[i ]	四角形の座標
		 	@param  right		[i ]	四角形の座標
		 	@param  bottom		[i ]	四角形の座標
		 
		    @return 四角形の中に点があるなら true
		\n			四角形の中に点がないなら false
		 */
		static bool IsPointToRect( TYPE x, TYPE y, TYPE left, TYPE top, TYPE right, TYPE bottom )
		{
			if( left<=x && x<right )
			{
				if( top<=y && y<bottom )
				{
					return true;
				}
			}
			return false;
		}

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 点 to 箱
		/*!
		 	@param  pos			[i ]	点の座標
		 	@param  rect		[i ]	箱の座標
		 
		    @return 四角形の中に点があるなら true
		\n			四角形の中に点がないなら false
		 */
		static bool IsPointToRect( const POINT3D_TEMPLATE<TYPE>& pos, const RECT3D_TEMPLATE<TYPE>& rect )
		{
			return IsPointToRect( pos.x, pos.y, pos.z, rect.Left, rect.Top, rect.Front, rect.Right, rect.Bottom, rect.Back );
		}

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 点 to 箱
		/*!
		 	@param  x			[i ]	点の座標
		 	@param  y			[i ]	点の座標
		 	@param  z			[i ]	点の座標
		 	@param  rect		[i ]	箱の座標
		 
		    @return 四角形の中に点があるなら true
		\n			四角形の中に点がないなら false
		 */
		static bool IsPointToRect( TYPE x, TYPE y, TYPE z, const RECT3D_TEMPLATE<TYPE>& rect )
		{
			return IsPointToRect( x, y, z, rect.Left, rect.Top, rect.Front, rect.Right, rect.Bottom, rect.Back );
		}
		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 点 to 箱
		/*!
		 	@param  x			[i ]	点の座標
		 	@param  y			[i ]	点の座標
		 	@param  z			[i ]	点の座標
		 	@param  left		[i ]	四角形の座標
		 	@param  top			[i ]	四角形の座標
		 	@param  right		[i ]	四角形の座標
		 	@param  bottom		[i ]	四角形の座標
		 
		    @return 四角形の中に点があるなら true
		\n			四角形の中に点がないなら false
		 */
		static bool IsPointToRect( TYPE x, TYPE y, TYPE z, TYPE left, TYPE top, TYPE front, TYPE right, TYPE bottom, TYPE back )
		{
			if( left<=x && x<right )
			{
				if( top<=y && y<bottom )
				{
					if( front<=z && z<back )
					{
						return true;
					}
				}
			}
			return false;
		}

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 点 to 回転した箱
		/*!
		 	@param  pos			[i ]	点の座標
		 	@param  rc  		[i ]	調べたい四角(基準はあわせてね)
		 	@param  rad			[i ]	rcをZ回転させる量（ラジアン度）
		 
		    @return 四角形の中に点があるなら true
		\n			四角形の中に点がないなら false
		 */
    static bool PointRect( const POINT2D& pos, const RECT2D& rc, float rad )
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

      return PointVertex( pos, dst, NUMELEMENTS(dst) );
    }

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 点 to 多角形
		/*!
		 	@param  pos			[i ]	点の座標
		 	@param  vec  		[i ]	多角形をあらわす頂点配列
		 	@param  len			[i ]	vecの個数(len角形ってこと)
		 
		    @return 多角形の中に点があるなら true
		\n			多角形の中に点がないなら false
		 */
    static bool PointVertex( const POINT2D& pos, const POINT2D* vec, size_t len )
    {
      MAID_ASSERT( !(2<=len), "二角形以上でお願いします" );

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

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 点 to 円
		/*!
		 	@param  x		[i ]	点の座標
		 	@param  y		[i ]	点の座標
		 	@param  r		[i ]	円の半径
		 
		    @return 円の中に点があるなら true
		\n			円の中に点がないなら false
		 */
		static bool IsPointToCircle( TYPE x, TYPE y, TYPE r )
		{
			return x*x+y*y < r*r;
		}

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 円 to 円
		/*!
		 	@param  x0		[i ]	円0の座標
		 	@param  y0		[i ]	円0の座標
		 	@param  r0		[i ]	円0の半径
		 	@param  x1		[i ]	円1の座標
		 	@param  y1		[i ]	円1の座標
		 	@param  r1		[i ]	円1の半径
		 
		    @return 円同士がぶつかるなら true
		\n			ちがうなら false
		 */
		static bool IsCircleToCircle( TYPE x0, TYPE y0, TYPE r0, TYPE x1, TYPE y1, TYPE r1 )
		{
			const TYPE x = x0 - x1;
			const TYPE y = y0 - y1;

			const TYPE r = r0+r1;
			return x*x+y*y < r*r;
		}

		static bool IsCircleToCircle( const POINT2D& p0, TYPE r0, const POINT2D& p1, TYPE r1 )
		{
			return IsCircleToCircle( p0.x, p0.y, r0, p1.x, p1.y, r1 );
		}

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 直線 to 四角形
		/*!
		 
		 	@param  Line		[i ]	直線
		 	@param  Rect		[i ]	矩形
		 
		    @return 重なっているならTRUE
		 			重なっていないならTRUE
		 */
		static bool IsLineToRect( const LINE2D& Line, const Maid::RECT2D_TEMPLATE<TYPE>& Rect  )
		{
			// 線が矩形内に入っているときもある
			if( IsPointToRect( Line.p0.x, Line.p0.y, Rect ) ) { return true; }
			if( IsPointToRect( Line.p1.x, Line.p1.y, Rect ) ) { return true; }

			// 矩形の４辺がぶつかっているか？
			const LINE2D side[4] = {
				LINE2D( Rect.Left,  Rect.Top,    Rect.Right, Rect.Top),		//	上辺
				LINE2D( Rect.Right, Rect.Top,    Rect.Right, Rect.Bottom),	//	右辺
				LINE2D( Rect.Left,  Rect.Top,    Rect.Left,  Rect.Bottom),	//	左辺
				LINE2D( Rect.Left,  Rect.Bottom, Rect.Right, Rect.Bottom),	//	下辺
			};

			for( unt32 i=0; i<NUMELEMENTS(side); ++i )
			{
				if ( IsLineToLine( side[i], Line ) ) 
				{
					return true;
				}
			}

			return false;
		}



		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 四角形 to 四角形
		/*!
		 
		 	@param  Rect0		[i ]	矩形１
		 	@param  Rect1		[i ]	矩形２
		 
		    @return 重なっているならTRUE
		 			重なっていないならTRUE
		 */
		static bool IsRectToRect( const Maid::RECT2D_TEMPLATE<TYPE>& Rect0, const Maid::RECT2D_TEMPLATE<TYPE>& Rect1 )
		{
			return IsRectToRect( Rect0.x, Rect0.y, Rect0.GetRight(), Rect0.GetBottom(),
								 Rect1.x, Rect1.y, Rect1.GetRight(), Rect1.GetBottom() );
		}
		
		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 四角形 to 四角形
		/*!
		 
		 	@param  left0		[i ]	矩形１
		 	@param  top0
		 	@param  right0
		 	@param  bottom0	
		 	@param  left1		[i ]	矩形２
		 	@param  top1
		 	@param  right1
		 	@param  bottom1	
		 
		    @return 重なっているならTRUE
		 			重なっていないならTRUE
		 */
		static bool IsRectToRect( TYPE left0, TYPE top0, TYPE right0, TYPE bottom0, TYPE left1, TYPE top1, TYPE right1, TYPE bottom1 )
		{
			if ( left0   > right1 ) { return false; }
			if ( right0  < left1  ) { return false; }
			if ( top0    > bottom1) { return false; }
			if ( bottom0 < top1   ) { return false; }

			return true;
		}

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 直線 to 直線
		/*!
		 
		 	@param  p0x0		[i ]	直線１
		 	@param  p0y0
		 	@param  p1x0
		 	@param  p1y0	
		 	@param  p0x1		[i ]	直線２
		 	@param  p0y1
		 	@param  p1x1
		 	@param  p1y1	
		 
		    @return 交差しているならTRUE
		 			交差していないならTRUE
		 */
		static bool IsLineToLine( TYPE p0x0, TYPE p0y0, TYPE p1x0, TYPE p1y0, TYPE p0x1, TYPE p0y1, TYPE p1x1, TYPE p1y1 )
		{
			return IsLineToLine(	LINE2D_TEMPLATE<TYPE>(p0x0,p0y0,p1y0,p1y0),
									LINE2D_TEMPLATE<TYPE>(p0x1,p0y1,p1y1,p1y1) );
		}

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 直線 to 直線
		/*!
		 
		 	@param  Line1		[i ]	直線１
		 	@param  Line2		[i ]	直線２
		 
		    @return 交差しているならTRUE
		 			交差していないならTRUE
		 */
		static bool IsLineToLine( const LINE2D_TEMPLATE<TYPE>& Line1, const LINE2D_TEMPLATE<TYPE>& Line2 )
		{
			float32 ret1, ret2;	//	int などでやるとオーバーフローするときがあるので実数型で行う

			{	//	直線１が直線２を交差しているか？
				const TYPE a1 = Side( Line2.p0.x, Line2.p0.y, Line1 );
				const TYPE a2 = Side( Line2.p1.x, Line2.p1.y, Line1 );
				ret1 = (float32)a1*a2;
			}
			{	//	直線２が直線１を交差しているか？
				const TYPE a1 = Side( Line1.p0.x, Line1.p0.y, Line2 );
				const TYPE a2 = Side( Line1.p1.x, Line1.p1.y, Line2 );
				ret2 = (float32)a1*a2;
			}

			return (ret1<=0) && (ret2<=0);
		}


		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 点が線分のどちら側にあるか調べる
		/*!
		 	@param  x			[i ]	点の座標
		 	@param  y			[i ]	点の座標
		 	@param  Line		[i ]	直線データ
		  
		    @return 線分の右側or下にある場合 + を返す
		\n			線分の直線上にある場合 0 を返す
		\n			線分の左側or上にある場合 - を返す
		 */
		static TYPE Side( TYPE x, TYPE y, const LINE2D_TEMPLATE<TYPE>& Line )
		{
			return Side( x, y, Line.p0.x, Line.p0.y, Line.p1.x, Line.p1.y );
		}

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 点が線分のどちら側にあるか調べる
		/*!
		 	@param  x		[i ]	点の座標
		 	@param  y		[i ]	点の座標
		 	@param  p0x		[i ]	線分の点１
		 	@param  p0y		[i ]	線分の点１
		 	@param  p1x		[i ]	線分の点２
		 	@param  p1y		[i ]	線分の点２
		 
		    @return 線分の右側にある場合 + を返す
		\n			線分の直線上にある場合 0 を返す
		\n			線分の左側にある場合 - を返す
		 */
		static TYPE Side( TYPE x, TYPE y, TYPE p0x, TYPE p0y, TYPE p1x, TYPE p1y )
		{
			return (x-p0x)*(y-p1y) - (x-p1x)*(y-p0y);
		}

		enum
		{
			OUTCODE_TOP    = 0x01,
			OUTCODE_BOTTOM = 0x02,
			OUTCODE_RIGHT  = 0x04,
			OUTCODE_LEFT   = 0x08,
		};

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 直線と直線が交差しているかを調べる
		/*!
		 	戻り値が true の場合 pointに交差している点がセットされる
		 
		 	@param  a		[i ]	直線１の始点
		 	@param  b		[i ]	直線１の終点
		 	@param  c		[i ]	直線２の始点
		 	@param  d		[i ]	直線２の終点
		 	@param  point	[ o]	交差している場合、座標がセットされる。交差してないなら不定
		 
		 	戻り値	交差していると true
		\n			交差していないと false
		 */
		static bool ClipLineToLine( const POINT2D& a, const POINT2D& b, const POINT2D& c, const POINT2D& d, POINT2D& point )
		{
			const VECTOR2D V1( a, b );

			const VECTOR2D V2( c, d );

			TYPE t,s;

			{
				const TYPE tmp = V1.y * V2.x - V1.x * V2.y;
				if( tmp==0 ) { return false; }
				t   = ((a.x - c.x)*V2.y - (a.y - c.y)*V2.x) / tmp;
			}

			{
				const TYPE tmp = V2.y * V1.x - V2.x * V1.y;
				if( tmp==0 ) { return false; }
				s   = ((c.x - a.x)*V1.y - (c.y - a.y)*V1.x) / tmp;
			}

			if( 0.0f<=t && t<=1.0f && 0.0f<=s && s<=1.0f )
			{
				const TYPE x = a.x + t * V1.x;
				const TYPE y = a.y + t * V1.y;

				point.x = x;
				point.y = y;

				return true;
			}
			return false;
		}

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 直線と矩形が交差しているかを調べる
		/*!
		 	戻り値が true の場合 それぞれ交差している点がセットされる
		 
		 	@param  begin	[io]	始点
		 	@param  end		[io]	終点
		 	@param  plane	[i ]	矩形
		 
		 	戻り値	交差していると true
		\n			交差していないと false
		 */
		static bool ClipLineToRect( POINT2D& begin, POINT2D& end, const RECT2D& plane )
		{
			// Cohen-Sutherland ラインクリッピングアルゴリズム　というらしい

			unt32 out0, out1;
			bool	accept = false;	//	交差しているか
			bool	done   = false;	//	do while ループを終わるか

			out0 = CompOutCode( begin, plane );
			out1 = CompOutCode(   end, plane );

			do{
				if     ( (out0|out1)==0 ) { accept=true; done=true; }
				else if( (out0&out1)!=0 ) { done=true; }
				else
				{
					TYPE x,y;
					const unt32 test = out0? out0:out1;

					//	交点を求める
					if( test&OUTCODE_TOP )
					{
						x = begin.x+(end.x-begin.x)*(plane.y-begin.y)/(end.y-begin.y);
						y = plane.y;
					}else if( test&OUTCODE_BOTTOM )
					{
						x = begin.x+(end.x-begin.x)*(plane.GetBottom()-begin.y)/(end.y-begin.y);
						y = plane.GetBottom();
					}else if( test&OUTCODE_RIGHT )
					{
						y = begin.y+(end.y-begin.y)*(plane.GetRight()-begin.x)/(end.x-begin.x);
						x = plane.GetRight();
					}else 
					{
						y = begin.y+(end.y-begin.y)*(plane.x-begin.x)/(end.x-begin.x);
						x = plane.x;
					}


					//	交点のセット
					if( test==out0 )
					{
						begin.x = x;
						begin.y = y;
						out0 = CompOutCode( begin, plane );
					}else
					{
						end.x = x;
						end.y = y;
						out1 = CompOutCode( end, plane );
					}
				}

			}while( !done );

			return accept;
		}

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		//! 点が矩形から見てどこにあるのかを調べる
		/*!
		 	戻り値が true の場合 それぞれ交差している点がセットされる
		 
		 	@param  pos		[i ]	点
		 	@param  plane	[i ]	矩形
		 
		 	戻り値	矩形から見た点の位置がフラグにセットされる
		\n			矩形の中にあれば０が戻る
		 */
		static unt CompOutCode( const POINT2D& pos, const RECT2D& plane )
		{
			unt	code = 0;

			if( pos.y < plane.x					  ) { code |= OUTCODE_TOP;    }
      else
      if(         plane.GetBottom() < pos.y ) { code |= OUTCODE_BOTTOM; }

			if(         plane.GetRight() < pos.x ) { code |= OUTCODE_RIGHT; }
			else
      if( pos.x < plane.x					 ) { code |= OUTCODE_LEFT;  }

			return code;
		}
	};
}




#endif
