#ifndef sample_0130guitest_app_h
#define sample_0130guitest_app_h

#include"../../source/config/define.h"
#include"../../source/framework/gameframework.h"
#include"../../source/framework/gui.h"
#include"../../source/graphics/graphics2drender.h"
#include"../../source/graphics/font.h"


#include"buttonsample.h"
#include"checkboxsample.h"
#include"radiobuttonsample.h"
#include"slidersample.h"
#include"windowsample.h"
#include"textboxsample.h"
#include"contextmenusample.h"


class MyApp : public Maid::IGameThread
{
public:
  MyApp();

protected:
  bool SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf );
  void Initialize();
  void UpdateFrame();
  void UpdateDraw();
  void Finalize();


private:
  void ReciveMessage( Maid::IGUIParts::ID id, const Maid::IGUIParam& param );


private:
  Maid::GraphicsCommandControl m_Command;
  Maid::Graphics2DRender       m_Render;

  Maid::Font m_hFont;
  std::list<Maid::String> m_MessageLog;

  int m_Frame;
private:
  ButtonSample      m_Button;
  CheckBoxSample    m_CheckBox;
  RadioButtonSample m_RadioButton[3];
  Maid::GUIRadioGroup     m_RadioGroup;
  SliderSample      m_Slider;
  SliderSample      m_Slider20;
  SliderSample      m_Slider20b;


  ButtonSample      m_WindowButton;
  ButtonSample      m_HideButton;
  ButtonSample      m_MiniButton;
  WindowSample      m_Window;

  TextBoxSample      m_TextBox;

  ContextMenuChildSample  m_ContextSubMenu1[3];
  ContextMenuChildSample  m_ContextSubMenu2[3];
  ContextMenuChildSample  m_ContextSubMenu3[3];
  Maid::IGUIContextMenu   m_ContextMenu;

  Maid::GUIManager      m_Manager;
};

#endif