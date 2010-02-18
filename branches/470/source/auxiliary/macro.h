/*!
 	@file
 	@brief	各種マクロ宣言
 */

#ifndef maid2_auxiliary_macro_h
#define maid2_auxiliary_macro_h





//! コンパイル時にメッセージを出力します
/*!
 	@def COMPILERMSG
 
 	@par 使用例
 	@code

    #pragma COMPILERMSG("Don't forget to fix this code.")	

 	@endcode
 */
#define COMPILERMSGSTR(x)    #x
#define COMPILERMSGSTR2(X)    COMPILERMSGSTR(X)
#define COMPILERMSG(msg) message(__FILE__ "(" COMPILERMSGSTR2(__LINE__) ") : コンパイルメッセージ　： " msg)


//! 配列の個数を数えます
/*!
 	@def NUMELEMENTS
 
 	@par 使用例
 	@code
 	
 	int Pixel[100];
 	printf( "%d", NUMELEMENTS(Pixel) );	// 100 と表示されます
 
 	@endcode
  
 */
#define	NUMELEMENTS(x)	(sizeof(x) / sizeof(x[0]))

//! else if の省略形
/*!
 
 	if() のあとに else if と書くと見た目がダサイので定義してあります
 
 	@par 使用例
 	@code
 	if( rhs.y <  lhs.y ) { return true }
 	ef( rhs.y == lhs.y ) { return rhs.x <  lhs.x }
 
 	@endcode
  
 */
//#define ef else if


#define PI (3.141592654f)	//!<	円周率


//!	100% を 0xFF として　a% を求める
/*!
 	@def ALPHA
 
 	@param	a	[i ]	求めたい値(0xFF-0x00)
 
 	@return 0-100 の値
 */
#define ALPHA( a ) (0xFF*(a)/100)			

//!	度数をラジアンに変換する
/*!
 	@param	deg	[i ]	求めたい度数
 
 	@return ラジアン度
 */
#define DEGtoRAD( deg ) ((deg)*(PI/180.0f))


//!	ラジアンを度数に変換する
/*!
 	\param	rad	[i ]	求めたい度数
 
 	\return 度数
 */
#define RADtoDEG( rad ) ((rad)*(180.0f/PI))


//!	ビットＯＮ
/*!
 	@param	Dst	[io]	変化する先
 	@param	Bit	[i ]	ＯＮにしたいビット数(0オリジン）
 
 */
#define BitOn( Dst, Bit ) ((Dst) |= (1<<(Bit)))

//!	ビットＯＦＦ
/*!
  @param	Dst	[io]	変化する先
  @param	Bit	[i ]	ＯＦＦにしたいビット数(0オリジン）
 
 */
#define BitOff( Dst, Bit ) ((Dst) &= ~(1<<(Bit)))


//!	ビットがたっているか調べる
/*!
    @param	Flag	[i ]	チェックする値
    @param	Bit		[i ]	チェックしたいビット数(0オリジン）

    @return ビットがＯＮだったら true
\n			    ビットがＯＦＦだったら false
	*/
#define IsBitOn( Flag, Bit ) (((Flag)&(1<<(Bit)))==(1<<(Bit)))


//!	複数ビットがたっているか調べる	
/*!
    @param	Flag	[i ]	チェックする値
    @param	data	[i ]	チェックしたい値

    @return 全部ＯＮだったら true
\n			    １つでもＯＦＦだったら false
	*/
#define IsFlag( Flag, data ) (((Flag)&(data))==(data))		

//!	8BitCode ４つを32BitCodeに変換する	
/*!
    ファイル上では a,b,c,d で並ぶ

    @param	a	[i ]	第１code
    @param	b	[i ]	第２code
    @param	c	[i ]	第３code
    @param	d	[i ]	第４code

    @return 32BitCode
 */
#define StrToBinary32( a, b, c, d) (((a)<<0) | ((b)<<8) | ((c)<<16) | ((d)<<24))

//!	8BitCode ２つを16BitCodeに変換する	
/*!
    ファイル上では a,b,c,d で並ぶ
	
    @param	a	[i ]	第１code
    @param	b	[i ]	第２code
	
    @return 32BitCode
 */
#define StrToBinary16( a, b ) (((a)<<0) | ((b)<<8))

//!	32 Bit値のエンディアンを変換する
/*!
	  @param	big	[i ]	変換元エンディアンの値
	
	  @return 変換後の値
 */
#define ConvertEndian32( big ) ((((big)&0xFF000000)>>24) | (((big)&0x00FF0000)>> 8) | (((big)&0x0000FF00)<< 8) | (((big)&0x000000FF)<<24))

//!	16 Bit値のエンディアンを変換する
/*!
	  @param	big	[i ]	変換元エンディアンの値
	
	  @return 変換後の値
 */
#define ConvertEndian16( big ) ((((big)&0xFF00)>> 8) | (((big)&0x00FF)<< 8))


#include<memory.h>
//!	指定した範囲を 0 クリアする	
/*!
	  @param	pDst	[i ]	クリア先
	  @param	Len		[i ]	何バイトクリアするか
 */
#define ZERO( pDst, Len ) (::memset( (pDst), 0, (Len) ))


//!	範囲内に入っているか？
/*!
 	@param	lha		[i ]	値１
 	@param	Target	[i ]	チェックするもの
 	@param	rha		[i ]	値２
 */
#define IsRange( lha, Target,  rha ) (((lha)<=(Target)) && ((Target)<=(rha)))


#endif