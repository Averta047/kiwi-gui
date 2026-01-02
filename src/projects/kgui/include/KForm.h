//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KWindow.h"
#include "KMenu.h"
#include "KWindowStyles.h"

class KForm : public KWindow
{
    friend LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    friend class KMDIClient;

public:
                            KForm(KWindow* pParent = nullptr, int PosX = CW_USEDEFAULT, int PosY = CW_USEDEFAULT);
                            ~KForm();

    KResizable              Resizable;

                            /* Menu */
    void                    SetMenu(KMenu* const pMenu);
    BOOL                    IsMenuEnabled();
    void                    ClearMenu();

private:
    static int              m_IndexPool;
    KMenu*                  m_pMenu;
    HWND                    m_hMDIClient;

    virtual void            Initialize();
    void                    Register_Class();

    virtual LRESULT         ProcessMenuCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    int                     GetDebugIndex();
};

