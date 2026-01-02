//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KWindow.h"
#include "KChecked.h"

class KCheckBox : public KWindow
{
protected:
private:
    static int              m_IndexPool;

    int                     GetDebugIndex();
    LRESULT                 ProcessCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT                 ProcessNotifyMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT                 OnReceiveCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
public:
                            KCheckBox() = delete;
                            KCheckBox(KWindow* pParent, int PosX = CW_USEDEFAULT, int PosY = CW_USEDEFAULT);
                            ~KCheckBox();

    KChecked                Checked;

public:
    KEventCallbackFunc_t    OnClickEvent{nullptr};
    KEventCallbackFunc_t    OnDisableEvent{nullptr};
    KEventCallbackFunc_t    OnDoubleClickedEvent{nullptr};
    KEventCallbackFunc_t    OnSetFocusEvent{nullptr};
    KEventCallbackFunc_t    OnKillFocusEvent{nullptr};
};

