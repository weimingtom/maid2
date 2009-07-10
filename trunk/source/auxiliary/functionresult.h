/*!
  @file 
  @brief	関数の戻り値を調べる
 */

#ifndef maid2_auxiliary_functionresult_h
#define maid2_auxiliary_functionresult_h


namespace Maid
{
  enum FUNCTIONRESULT
  {
    FUNCTIONRESULT_ERROR,    //  関数が失敗
    FUNCTIONRESULT_OK,       //  関数が正常に動いた
  };

  inline bool FUNCTIONRESULT_FAILE( FUNCTIONRESULT v ) { return v==FUNCTIONRESULT_ERROR; }
  inline bool FUNCTIONRESULT_SUCESS( FUNCTIONRESULT v ) { return !FUNCTIONRESULT_FAILE(v); }

}



#endif