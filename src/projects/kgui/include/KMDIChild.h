//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KWindow.h"
#include "KMenu.h"
#include "KMDIClient.h"

LRESULT CALLBACK MDIChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class KMDIChild : public KWindow
{
    friend LRESULT CALLBACK MDIChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
private:
    static int              m_IndexPool;
    KMenu*                  m_pMenu{nullptr};

    virtual void            Initialize();
    void                    Register_Class();

    LRESULT                 OnCreate();                 // WM_CREATE
    LRESULT                 OnNCCreate();               // WM_NCCREATE
    LRESULT                 OnClose();                  // WM_CLOSE

    int                     GetDebugIndex();

public:
    KMDIChild() = delete;
    KMDIChild(KMDIClient* pParent = nullptr, int PosX = CW_USEDEFAULT, int PosY = CW_USEDEFAULT);
    ~KMDIChild();

#if 0
    /* Menu */
    void                    SetMenu(KMenu* const pMenu);
    BOOL                    IsMenuEnabled();
    void                    ClearMenu();
#endif

    KEventCallbackFunc_t    OnCloseEvent{nullptr};
};