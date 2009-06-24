/*!
  @file 
  @brief	関数の戻り値を調べる
 */

#ifndef maid2_auxiliary_functionresult_h
#define maid2_auxiliary_functionresult_h


namespace Maid
{
  enum FUCTIONRESULT
  {
    FUCTIONRESULT_ERROR,    //  関数が失敗
    FUCTIONRESULT_OK,       //  関数が正常に動いた
  };

  inline bool FUCTIONRESULT_FAILE( FUCTIONRESULT v ) { return v==FUCTIONRESULT_ERROR; }
  inline bool FUCTIONRESULT_SUCESS( FUCTIONRESULT v ) { return !FUCTIONRESULT_FAILE(v); }

}



#endif