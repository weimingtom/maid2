#include "rand.h"
#include "debug/assert.h"


namespace Maid
{
/*!
 	@class	Rand rand.h
 	@brief	乱数生成クラス
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Min 0<= x < Max の範囲で乱数を取得する
/*!
 	こちらは負数有りなので気をつけること
 
 	@param	Min	[i ]	最小値
 	@param	Max	[i ]	最大値
 */
int	Rand::Get( int Min, int Max )
{
	MAID_ASSERT( !(Min<=Max), "引数の指定が反対です" <<  Min << " <= " << Max );

  boost::uniform_smallint<> dst( Min, Max );
  boost::variate_generator<
			  boost::mt19937&, boost::uniform_smallint<>
		  > die( m_Generator, dst );
	return die();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 1/n の確率を満たしているか調べる
/*!
 	@param	n	[i ]	確率
 */
bool Rand::Check(int n)
{
	return Get(1,n)==0;
}


void Rand::SetSeed( unt Seed )
{
	m_Generator.seed(boost::uint32_t(Seed) );
}


}
