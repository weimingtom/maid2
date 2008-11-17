/*!
    @file
    @brief ビデオカードの列挙を行うベースクラス
 */

#ifndef graphic_core_videocardlistbase_h
#define graphic_core_videocardlistbase_h


#include"../config/define.h"
#include"../../auxiliary/string.h"

#include"driver/videocard.h"


namespace Maid
{
  class VideoCardListBase
  {
  public:
    virtual ~VideoCardListBase(){}

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 初期化
    /*!
     */
    virtual void Initialize()=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 使えるビデオカードの総数を取得する
    /*!
        @return	ビデオカードの総数
     */
    virtual int GetVideoCardCount()=0;

    struct VIDEOCARDINFO
    {
      String  Name; //!<  ビデオカードの名前
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 指定したビデオカードの性能を取得する
    /*!
        @param  no  [i ]  取得したい番号(0< no < GetVideoCardCount() であること)

        @return	ビデオカードの総数
     */
    virtual VIDEOCARDINFO GetVideoCardInfo( int no )=0;


    struct CREATEPARAM
    {
      bool  IsSoftwareVertexProcessing; //!<  頂点変換をソフトウェアで行うか？
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 指定したビデオカードを作成する
    /*!
        @param  No  [i ]  作成したい番号(0< no < GetVideoCardCount() であること)

        @return	作成されたビデオカード
     */
    virtual SPVIDEOCARD CreateVideoCard( int No, const CREATEPARAM& Info ) =0;

  };
}


#endif
