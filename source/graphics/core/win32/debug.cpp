#include"debug.h"

#include"../../../auxiliary/macro.h"

static std::string _sprintf( const char* format, ...)
{
  va_list args;
  char buf[1024];

  va_start(args,format);
  vsprintf_s(buf, format, args);

  return std::string(buf);
}


std::string DebugStringDXGI( DXGI_FORMAT fmt )
{
  std::string ret;
  switch( fmt )
  {
  case DXGI_FORMAT_UNKNOWN:               { ret = "UNKNOWN"; }break;

  case DXGI_FORMAT_R32G32B32A32_TYPELESS: { ret = "R32G32B32A32_TYPELESS"; }break;
  case DXGI_FORMAT_R32G32B32A32_FLOAT:    { ret = "R32G32B32A32_FLOAT"; }break;
  case DXGI_FORMAT_R32G32B32A32_UINT:     { ret = "R32G32B32A32_UINT"; }break;
  case DXGI_FORMAT_R32G32B32A32_SINT:     { ret = "R32G32B32A32_SINT"; }break;

  case DXGI_FORMAT_R32G32B32_TYPELESS:    { ret = "R32G32B32_TYPELESS"; }break;
  case DXGI_FORMAT_R32G32B32_FLOAT:       { ret = "R32G32B32_FLOAT"; }break;
  case DXGI_FORMAT_R32G32B32_UINT:        { ret = "R32G32B32_UINT"; }break;
  case DXGI_FORMAT_R32G32B32_SINT:        { ret = "R32G32B32_SINT"; }break;

  case DXGI_FORMAT_R16G16B16A16_TYPELESS: { ret = "R16G16B16A16_TYPELESS"; }break;
  case DXGI_FORMAT_R16G16B16A16_FLOAT:    { ret = "R16G16B16A16_FLOAT"; }break;
  case DXGI_FORMAT_R16G16B16A16_UNORM:    { ret = "R16G16B16A16_UNORM"; }break;
  case DXGI_FORMAT_R16G16B16A16_UINT:     { ret = "R16G16B16A16_UINT"; }break;
  case DXGI_FORMAT_R16G16B16A16_SNORM:    { ret = "R16G16B16A16_SNORM"; }break;
  case DXGI_FORMAT_R16G16B16A16_SINT:     { ret = "R16G16B16A16_SINT"; }break;
  case DXGI_FORMAT_R32G32_TYPELESS:       { ret = "R32G32_TYPELESS"; }break;
  case DXGI_FORMAT_R32G32_FLOAT:          { ret = "R32G32_FLOAT"; }break;
  case DXGI_FORMAT_R32G32_UINT:           { ret = "R32G32_UINT"; }break;
  case DXGI_FORMAT_R32G32_SINT:           { ret = "R32G32_SINT"; }break;
  case DXGI_FORMAT_R32G8X24_TYPELESS:     { ret = "R32G8X24_TYPELESS"; }break;
  case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:  { ret = "D32_FLOAT_S8X24_UINT"; }break;
  case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS: { ret = "R32_FLOAT_X8X24_TYPELESS"; }break;
  case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT: { ret = "X32_TYPELESS_G8X24_UINT"; }break;

  case DXGI_FORMAT_R10G10B10A2_TYPELESS:{ ret = "R10G10B10A2_TYPELESS"; }break;
  case DXGI_FORMAT_R10G10B10A2_UNORM:   { ret = "R10G10B10A2_UNORM"; }break;
  case DXGI_FORMAT_R10G10B10A2_UINT:    { ret = "R10G10B10A2_UINT"; }break;
  case DXGI_FORMAT_R11G11B10_FLOAT:     { ret = "R11G11B10_FLOAT"; }break;
  case DXGI_FORMAT_R8G8B8A8_TYPELESS:   { ret = "R8G8B8A8_TYPELESS"; }break;
  case DXGI_FORMAT_R8G8B8A8_UNORM:      { ret = "R8G8B8A8_UNORM"; }break;
  case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB: { ret = "R8G8B8A8_UNORM_SRGB"; }break;
  case DXGI_FORMAT_R8G8B8A8_UINT:       { ret = "R8G8B8A8_UINT"; }break;
  case DXGI_FORMAT_R8G8B8A8_SNORM:      { ret = "R8G8B8A8_SNORM"; }break;
  case DXGI_FORMAT_R8G8B8A8_SINT:       { ret = "R8G8B8A8_SINT"; }break;
  case DXGI_FORMAT_R16G16_TYPELESS:     { ret = "R16G16_TYPELESS"; }break;
  case DXGI_FORMAT_R16G16_FLOAT:        { ret = "R16G16_FLOAT"; }break;
  case DXGI_FORMAT_R16G16_UNORM:        { ret = "R16G16_UNORM"; }break;
  case DXGI_FORMAT_R16G16_UINT:         { ret = "R16G16_UINT"; }break;
  case DXGI_FORMAT_R16G16_SNORM:        { ret = "R16G16_SNORM"; }break;
  case DXGI_FORMAT_R16G16_SINT:         { ret = "R16G16_SINT"; }break;
  case DXGI_FORMAT_R32_TYPELESS:        { ret = "R32_TYPELESS"; }break;
  case DXGI_FORMAT_D32_FLOAT:           { ret = "D32_FLOAT"; }break;
  case DXGI_FORMAT_R32_FLOAT:           { ret = "R32_FLOAT"; }break;
  case DXGI_FORMAT_R32_UINT:            { ret = "R32_UINT"; }break;
  case DXGI_FORMAT_R32_SINT:            { ret = "R32_SINT"; }break;
  case DXGI_FORMAT_R24G8_TYPELESS:      { ret = "R24G8_TYPELESS"; }break;
  case DXGI_FORMAT_D24_UNORM_S8_UINT:   { ret = "D24_UNORM_S8_UINT"; }break;
  case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:{ ret = "R24_UNORM_X8_TYPELESS"; }break;
  case DXGI_FORMAT_X24_TYPELESS_G8_UINT:{ ret = "X24_TYPELESS_G8_UINT"; }break;

  case DXGI_FORMAT_R8G8_TYPELESS: { ret = "R8G8_TYPELESS"; }break;
  case DXGI_FORMAT_R8G8_UNORM:    { ret = "R8G8_UNORM"; }break;
  case DXGI_FORMAT_R8G8_UINT:     { ret = "R8G8_UINT"; }break;
  case DXGI_FORMAT_R8G8_SNORM:    { ret = "R8G8_SNORM"; }break;
  case DXGI_FORMAT_R8G8_SINT:     { ret = "R8G8_SINT"; }break;
  case DXGI_FORMAT_R16_TYPELESS:  { ret = "R16_TYPELESS"; }break;
  case DXGI_FORMAT_R16_FLOAT:     { ret = "R16_FLOAT"; }break;
  case DXGI_FORMAT_D16_UNORM:     { ret = "D16_UNORM"; }break;
  case DXGI_FORMAT_R16_UNORM:     { ret = "R16_UNORM"; }break;
  case DXGI_FORMAT_R16_UINT:      { ret = "R16_UINT"; }break;
  case DXGI_FORMAT_R16_SNORM:     { ret = "R16_SNORM"; }break;
  case DXGI_FORMAT_R16_SINT:      { ret = "R16_SINT"; }break;

  case DXGI_FORMAT_R8_TYPELESS: { ret = "R8_TYPELESS"; }break;
  case DXGI_FORMAT_R8_UNORM:    { ret = "R8_UNORM"; }break;
  case DXGI_FORMAT_R8_UINT:     { ret = "R8_UINT"; }break;
  case DXGI_FORMAT_R8_SNORM:    { ret = "R8_SNORM"; }break;
  case DXGI_FORMAT_R8_SINT:     { ret = "R8_SINT"; }break;
  case DXGI_FORMAT_A8_UNORM:    { ret = "A8_UNORM"; }break;

  case DXGI_FORMAT_R1_UNORM:    { ret = "R1_UNORM"; }break;

  case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:{ ret = "R9G9B9E5_SHAREDEXP"; }break;
  case DXGI_FORMAT_R8G8_B8G8_UNORM:   { ret = "R8G8_B8G8_UNORM"; }break;
  case DXGI_FORMAT_G8R8_G8B8_UNORM:   { ret = "G8R8_G8B8_UNORM"; }break;

  case DXGI_FORMAT_BC1_TYPELESS:  { ret = "BC1_TYPELESS"; }break;
  case DXGI_FORMAT_BC1_UNORM:     { ret = "BC1_UNORM"; }break;
  case DXGI_FORMAT_BC1_UNORM_SRGB:{ ret = "BC1_UNORM_SRGB"; }break;
  case DXGI_FORMAT_BC2_TYPELESS:  { ret = "BC2_TYPELESS"; }break;
  case DXGI_FORMAT_BC2_UNORM:     { ret = "BC2_UNORM"; }break;
  case DXGI_FORMAT_BC2_UNORM_SRGB:{ ret = "BC2_UNORM_SRGB"; }break;
  case DXGI_FORMAT_BC3_TYPELESS:  { ret = "BC3_TYPELESS"; }break;
  case DXGI_FORMAT_BC3_UNORM:     { ret = "BC3_UNORM"; }break;
  case DXGI_FORMAT_BC3_UNORM_SRGB:{ ret = "BC3_UNORM_SRGB"; }break;
  case DXGI_FORMAT_BC4_TYPELESS:  { ret = "BC4_TYPELESS"; }break;
  case DXGI_FORMAT_BC4_UNORM:     { ret = "BC4_UNORM"; }break;
  case DXGI_FORMAT_BC4_SNORM:     { ret = "BC4_SNORM"; }break;
  case DXGI_FORMAT_BC5_TYPELESS:  { ret = "BC5_TYPELESS"; }break;
  case DXGI_FORMAT_BC5_UNORM:     { ret = "BC5_UNORM"; }break;
  case DXGI_FORMAT_BC5_SNORM:     { ret = "BC5_SNORM"; }break;

  case DXGI_FORMAT_B5G6R5_UNORM:  { ret = "B5G6R5_UNORM"; }break;
  case DXGI_FORMAT_B5G5R5A1_UNORM:{ ret = "B5G5R5A1_UNORM"; }break;
  case DXGI_FORMAT_B8G8R8A8_UNORM:{ ret = "B8G8R8A8_UNORM"; }break;
  case DXGI_FORMAT_B8G8R8X8_UNORM:{ ret = "B8G8R8X8_UNORM"; }break;

  case DXGI_FORMAT_FORCE_UINT: { ret = "FORCE_UINT"; }break;
  }

  ret += _sprintf( "(%0d)", fmt );
  return ret;
}




std::string DebugStringDXGI( const DXGI_SWAP_CHAIN_DESC& desc )
{
/*
    desc.BufferDesc.Width  = m_Window.GetClientSize().w;
    desc.BufferDesc.Height = m_Window.GetClientSize().h;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
    desc.BufferCount = 1;

    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = m_Window.GetHWND();
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
*/

  return std::string();
}