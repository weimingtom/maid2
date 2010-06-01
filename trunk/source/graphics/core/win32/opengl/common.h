#ifndef graphic_core_driver_win32_opengl_common_h
#define graphic_core_driver_win32_opengl_common_h

/*!	
    @file
    @brief	このディレクトリの共通ヘッダ
*/


#include"../../../../config/define.h"
#include"../../../../config/typedef.h"
#include"../../../../config/win32.h"

#include"../../../../auxiliary/macro.h"
#include"../../../../auxiliary/debug/assert.h"
#include"../../../../auxiliary/debug/warning.h"

#include"../../../pixelformat.h"
#include"../../idevice.h"




#include<string>
#include"iopenglobject.h"

namespace Maid { namespace Graphics {

  inline GLenum TARGETBLENDtoGLenum( BLENDSTATEPARAM::TARGET::BLEND b )
  {
    GLenum ret = GL_ZERO;

    switch( b )
    {
    case BLENDSTATEPARAM::TARGET::BLEND_ZERO:          { ret = GL_ZERO; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_ONE:           { ret = GL_ONE; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_SRC_COLOR:     { ret = GL_SRC_COLOR; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_INV_SRC_COLOR: { ret = GL_ONE_MINUS_SRC_COLOR; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_SRC_ALPHA:     { ret = GL_SRC_ALPHA; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_INV_SRC_ALPHA: { ret = GL_ONE_MINUS_SRC_ALPHA; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_DST_ALPHA:    { ret = GL_DST_ALPHA; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_INV_DST_ALPHA:{ ret = GL_ONE_MINUS_DST_ALPHA; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_DST_COLOR:    { ret = GL_DST_COLOR; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_INV_DST_COLOR:{ ret = GL_ONE_MINUS_DST_COLOR; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_SRC_ALPHA_SAT: { ret = GL_SRC_ALPHA_SATURATE; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_BLEND_FACTOR:  { MAID_WARNING( "BLEND_BLEND_FACTOR は定義されていません"); }break;
    case BLENDSTATEPARAM::TARGET::BLEND_INV_BLEND_FACTOR:{ MAID_WARNING( "BLEND_BLEND_FACTOR は定義されていません"); }break;
    }
    return ret;
  }

  inline GLint PIXELFORMATtoInternalFormat( PIXELFORMAT fmt )
  {
    GLint ret = GL_RGB;
    switch( fmt )
    {
    case PIXELFORMAT_R08G08B08I:    { ret = GL_RGB;  }break;
    case PIXELFORMAT_A08R08G08B08I: { ret = GL_RGB;  }break;
    case PIXELFORMAT_X08R08G08B08I: { ret = GL_RGBA; }break;
    case PIXELFORMAT_D15IS1:  { ret = GL_DEPTH_COMPONENT; }break;
    case PIXELFORMAT_D16I:    { ret = GL_DEPTH_COMPONENT; }break;
    case PIXELFORMAT_D32I:    { ret = GL_DEPTH_COMPONENT; }break;
    case PIXELFORMAT_D24IX8:  { ret = GL_DEPTH_COMPONENT; }break;
    case PIXELFORMAT_D24IS8:  { ret = GL_DEPTH_COMPONENT; }break;
    case PIXELFORMAT_D24IX4S4:{ ret = GL_DEPTH_COMPONENT; }break;
    case PIXELFORMAT_D24FS8:  { ret = GL_DEPTH_COMPONENT; }break;
    }

    return ret;
  }
}}

#endif
