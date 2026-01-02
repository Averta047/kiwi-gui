//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KObject.h"
#include "KText.h"
#include "KWidth.h"
#include "KHeight.h"
#include "KPosX.h"
#include "KPosY.h"
#include "KPopupMenu.h"
#include "KFont.h"
#include "KTheme.h"

class KWindow;
class KText;
class KWidth;
class KWidth;
class KPosX;
class KPosY;
class KPopupMenu;

class KWindow : public KObject
{
    friend LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    friend LRESULT CALLBACK ChildWindProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

public:
                            KWindow() = delete;
                            KWindow(KWindow* pParent, int PosX = CW_USEDEFAULT, int PosY = CW_USEDEFAULT);
    virtual                 ~KWindow();

public:
    KText                   Text;
    KWidth                  Width;
    KHeight                 Height;
    KPosX                   X;
    KPosY                   Y;
    KPopupMenu*             pPopupMenu;
    KFont                   Font;
    KTheme                  Theme;

    struct {
        bool Left           {true};
        bool Right          {false};
        bool Bottom         {false};
        bool Top            {true};
    } Anchors;

    struct {
        int MinHeight       {0};
        int MinWidth        {0};
        int MaxHeight       {0};
        int MaxWidth        {0};
    } Constraints;

    void                    Show();
    void                    Hide();
    
    void                    Maximize();
    void                    Minimize();

    void                    Enable();
    void                    Disable();

    void                    Destroy();

    virtual void            SetFocus();

    KEventCallbackFunc_t    OnReceiveCommandMessageEvent{nullptr};
    KEventCallbackFunc_t    OnDestroyEvent{nullptr};

    KWindow*                GetParent() const;

    // todo:
    // invalidate
#if 1
    // win32 specific
    void                    StoreCurrentRect();
    const HWND              GetHandle() const;
#endif

protected:
    KWindow*                m_pParent;
    HWND                    m_hWinHandle; // TODO: move to private and make public GetHandle() method

    // TODO: Remove.
    struct {
        DWORD       dwExStyle;
        LPCTSTR     lpClassName;
        LPCTSTR     lpWindowName;
        DWORD       dwStyle;
        int         nX;         // set in KWindow c-tor
        int         nY;         // set in KWindow c-tor
        int         nWidth;     // set in KWindow c-tor
        int         nHeight;    // set in KWindow c-tor
        HWND        hWndParent; // set in KWindow c-tor
        HMENU       hMenu;      // set in KObject via KWindow
        HINSTANCE   hInstance;  // set in KWindow c-tor
        LPVOID      lpParam;
    }   m_sWindowArgs;          // arguments for CreateWindowEx

    void                    Create();

    virtual LRESULT         OnReceiveCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);  // All Events
    virtual LRESULT         OnCreate();     // WM_CREATE
    virtual LRESULT         OnNCCreate();   // WM_NCCREATE
    virtual LRESULT         OnClose();      // WM_CLOSE
    virtual LRESULT         OnDestroy();    // WM_DESTROY
    virtual LRESULT         OnFocus();      // WM_SETFOCUS
    virtual LRESULT         OnKillFocus();  // WM_KILLFOCUS
    virtual LRESULT         OnSetText(LPARAM lParam);    // WM_SETTEXT
    virtual LRESULT         OnResize(HWND hWnd, WPARAM wParam, LPARAM lParam); // WM_SIZE
    virtual LRESULT         OnGetMinMax(LPARAM lParam);    // WM_GETMINMAXINFO
    virtual LRESULT         OnContextMenu(WPARAM wParam, LPARAM lParam); //WM_CONTEXTMENU

    LRESULT                 ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT         ProcessCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT         ProcessMenuCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT         ProcessNotifyMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual int             GetDebugIndex() = 0;
    void                    CreateDebugName();

    // children windows accessors
    void                    AddChildrenWindow(KWindow* pChildren);
    void                    RemoveChildrenWindow(KWindow* pChildren);
    KWindow*                FindChildrenByID(const WORD SearchedId);

private:
    std::vector<KWindow*>  m_Children;

    // old child window position. Used by Anchors.
    RECT                    m_rcOldPosition{};

    void                    GetRect(RECT& rcOutput);
};

