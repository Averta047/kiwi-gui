//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KButton.h"

class KGroupBox : public KWindow
{
protected:
private:
    static int              m_IndexPool;

    int                     GetDebugIndex();
    LRESULT                 ProcessCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT                 ProcessNotifyMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    KGroupBox() = delete;
    KGroupBox(KWindow* pParent, int PosX = CW_USEDEFAULT, int PosY = CW_USEDEFAULT);
    ~KGroupBox();
};

