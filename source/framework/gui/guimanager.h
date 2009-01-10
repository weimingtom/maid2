#ifndef maid2_framework_gui_guimanager_h
#define maid2_framework_gui_guimanager_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"../../input/mouse.h"
#include"../../input/keybord.h"
#include"../../auxiliary/mathematics.h"
#include"iguiparts.h"
#include"guimessage.h"
#include<list>

namespace Maid
{
  class GUIManager
  {
  public:
    GUIManager();
    ~GUIManager();
    void Initialize( const Mouse* pMouse, const Keybord* pKey /*const FEP* pFep */);
    void Finalize();

    void UpdateFrame();
    void UpdateDraw( RenderTargetBase& Target, IDepthStencil& Depth );

    void SetEnable( bool IsEnable );
    void SetVisible( bool IsVisible );

    void RegisterParts( IGUIParts::ID id, const SPGUIPARTS& pParts );
    void DeleteParts( const SPGUIPARTS& pParts );
    bool IsExitstParts( const SPGUIPARTS& pParts )const;
    SPGUIPARTS FindParts( IGUIParts::ID id )const; 

  private:
    SPGUIPARTS FindParts( const POINT2DI& pos ) const;


  private:
    const Mouse*    m_pMouse;
    const Keybord*  m_pKeybord;

    bool      m_IsEnable;
    bool      m_IsVisible;


    typedef std::list<SPGUIPARTS> PARTSLIST;
    PARTSLIST   m_PartsList;
    SPGUIPARTS  m_pRoot;

    enum STATE
    {
      STATE_NORMAL,     //  通常時
      STATE_MOUSELDOWN, //  マウスの左ボタンを押しっぱなし状態

    };

    struct MESSAGE
    {
      MESSAGE( const SPGUIPARTS& parts, const SPGUIPARAM& param ):pParts(parts),pParam(param){}
      SPGUIPARTS pParts;
      SPGUIPARAM pParam;
    };

    typedef std::vector<MESSAGE> MESSAGELIST;

  private:  //  マウスがらみで使うパラメータ
    void GenerateMouseMessage( MESSAGELIST& list );
    SPGUIPARTS  m_pMouseDownParts;  //  マウスの左ボタンが押されているときに保持されるポインタ
    SPGUIPARTS  m_pMousePrevParts;  //  前のフレームでマウスがあった場所にあったオブジェクト
  };
}




#endif
