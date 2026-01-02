//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KWindow.h"
#include "KWindowStyles.h"

class KButton : public KWindow
{
public:
                            KButton() = delete;
                            KButton(KWindow* pParent, int PosX = CW_USEDEFAULT, int PosY = CW_USEDEFAULT);
                            ~KButton();

public:
    KBS_DefaultPush         DefaultPushStyle; // Change Button look

    KEventCallbackFunc_t    OnClickEvent{nullptr};
    KEventCallbackFunc_t    OnPaintEvent{nullptr};
    KEventCallbackFunc_t    OnHiliteEvent{nullptr};
    KEventCallbackFunc_t    OnUnhiliteEvent{nullptr};
    KEventCallbackFunc_t    OnDisableEvent{nullptr};
    KEventCallbackFunc_t    OnPushedEvent{nullptr};
    KEventCallbackFunc_t    OnUnpushedEvent{nullptr};
    KEventCallbackFunc_t    OnDoubleClickedEvent{nullptr};
    KEventCallbackFunc_t    OnSetFocusEvent{nullptr};
    KEventCallbackFunc_t    OnKillFocusEvent{nullptr};

protected:
    virtual int             GetDebugIndex();

private:
    static int              m_IndexPool;

    LRESULT                 ProcessCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT                 ProcessNotifyMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

