#ifndef framework_oggplayerlocal_h
#define framework_oggplayerlocal_h

#include"../config/define.h"

#include"oggdecoder.h"


//  OggPlayer が内部で勝手に使っているヘッダ
//  なので oggplayerX.cpp 以外では #include禁止


static const int FRAMECOUNT = 5;
static const double SAOUNDTIME = 2.0;

#endif
