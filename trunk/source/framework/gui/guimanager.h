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
#include<boost/function.hpp>

namespace Maid
{
  class GUIManager : public IGUIParts
  {
  public:
    GUIManager();
    ~GUIManager();

    typedef boost::function2<void, ID, const IGUIParam&>	RECIVEFUNCTION;
    void Initialize( const RECIVEFUNCTION& Func, const Mouse* pMouse, const Keybord* pKey /*const FEP* pFep */);
    void Finalize();

    void RegisterParts( IGUIParts::ID id, const SPGUIPARTS& pParts );
    void DeleteParts( const SPGUIPARTS& pParts );
    bool IsExitstParts( const SPGUIPARTS& pParts )const;
    SPGUIPARTS FindParts( IGUIParts::ID id )const; 



  protected:
    virtual bool LocalIsMouseIn( const POINT2DI& pos ) const;
    virtual void OnUpdateFrame();
    virtual void OnUpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const VECTOR2DI& offset );
    virtual void ReciveMessage( ID src, const IGUIParam& Param );

  private:
    SPGUIPARTS FindParts( const POINT2DI& pos ) const;

    RECIVEFUNCTION  m_ReciveFunction;

    const Mouse*    m_pMouse;
    const Keybord*  m_pKeybord;

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

  #define MakeReciveMessageFunction(pFunc,pObject) boost::bind( pFunc, pObject, _1, _2 )

}




#endif
