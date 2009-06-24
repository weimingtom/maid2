/*!
  @file 
  @brief	特定のクラス間だけでアクセスできるようにするテンプレート
 */

#ifndef maid2_auxiliary_globalpointer_h
#define maid2_auxiliary_globalpointer_h


#include"../config/define.h"
#include"debug/assert.h"

namespace Maid
{
  /*!
    @class  GlobalPointer globalpointer.h
    @brief  グローバルポインタテンプレート
    \n      全領域にわたって使われているようなものはこれを継承して使うとラクチン
    \n      設計より実装効率を重視したクラス
    \n
    \n      static変数使ってるので、このクラスを継承した場合は、そのクラス中に宣言もいれてください
   */
	template<class TYPE>
	class GlobalPointer
	{
	protected:

    //! ポインタの設定
    /*!
        最初に設定したものしか使えないので注意！
     */
		void Set()
		{
			MAID_ASSERT( s_pPointer!=NULL, "すでに実態が存在します" );
			s_pPointer = static_cast<TYPE*>(this);
		}

    //! ポインタの取得
    /*!
        @return	ポインタ
     */
		static TYPE* Get()
		{
			MAID_ASSERT( s_pPointer==NULL, "まだ設定されていません" );
			return s_pPointer;
		}

		void Clear() { s_pPointer = NULL; }

	private:
		static TYPE*	s_pPointer;
	};

}


#endif
