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
			MAID_ASSERT( _Get()!=NULL, "すでに実態が存在します" );
			_Get() = static_cast<TYPE*>(this);
		}

    //! ポインタの取得
    /*!
        @return	ポインタ
     */
		static TYPE* Get()
		{
			MAID_ASSERT( _Get()==NULL, "まだ設定されていません" );
			return _Get();
		}

		void Clear() { _Get() = NULL; }

	private:
    static TYPE*& _Get()
    {
		  static TYPE*	s_pPointer;
      return s_pPointer;
    }
	};

}


#endif
