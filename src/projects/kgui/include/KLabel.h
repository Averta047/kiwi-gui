//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once
#include "KWindow.h"

class KLabel : public KWindow
{
protected:
    virtual int             GetDebugIndex();
private:
    static int              m_IndexPool;

    LRESULT                 ProcessCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT                 ProcessNotifyMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
                            KLabel() = delete;
                            KLabel(KWindow* pParent, int PosX = CW_USEDEFAULT, int PosY = CW_USEDEFAULT);
                            ~KLabel();

    KEventCallbackFunc_t    OnClickEvent{nullptr};
    KEventCallbackFunc_t    OnDoubleClickedEvent{nullptr};
    KEventCallbackFunc_t    OnDisableEvent{nullptr};
    KEventCallbackFunc_t    OnEnableEvent{nullptr};
};

