/*!
 	@file
 	@brief	乱数生成クラス
 */

#ifndef maid2_auxiliary_rand_h
#define maid2_auxiliary_rand_h

#include"../config/define.h"
#include"../config/typedef.h"

#include <boost/random.hpp>

namespace Maid
{
	class Rand
	{
  public:
		int		Get( int Min, int Max );
		bool	Check(int n);
		void	SetSeed(unt Seed);

  private:
    boost::mt19937  m_Generator;
	};
}
#endif
