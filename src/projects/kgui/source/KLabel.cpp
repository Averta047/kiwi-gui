//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KLabel.h"

// Defines
#define CLASS_NAME TEXT("KLabel")
#define WIN32_CLASS_NAME TEXT("STATIC")
#define DEFAULT_TEXT TEXT("Caption")

#define DEFAULT_WIDTH  75
#define DEFAULT_HEIGHT 25

// Local Memory
int KLabel::m_IndexPool = 1;

// Prototypes
LRESULT CALLBACK ChildWindProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

KLabel::KLabel(KWindow* pParent, int PosX, int PosY)
    :
    KWindow(
        pParent, 
        PosX, 
        PosY
    )
{
    this->m_Name =          CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =     0;
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_VISIBLE | WS_CHILD | SS_NOTIFY;
    this->m_sWindowArgs.nWidth =        DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =       DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =         reinterpret_cast<HMENU>(this->m_ID);;
    this->m_sWindowArgs.lpParam =       this;

    this->Create();

    BOOL fResult = SetWindowSubclass(this->m_hWinHandle, ChildWindProc, 0, (DWORD_PTR)this);

    if (FALSE == fResult) {
        MSG_ERROR(TEXT("SetWindowSubclass failed with error = 0x%X"), GetLastError());
    }
}

KLabel::~KLabel()
{
}

int KLabel::GetDebugIndex()
{
    int dReturn = this->m_IndexPool;
    this->m_IndexPool++;
    return dReturn;
}

LRESULT KLabel::ProcessCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT tResult = 0; // should return 1 if not used (no CB registered)

    if (LOWORD(wParam) == static_cast<WORD>(this->m_ID)) {
        switch (HIWORD(wParam)) {
        case STN_CLICKED:
            MSG_LOG(TEXT("STN_CLICKED"));
            if (nullptr != OnClickEvent) {
                OnClickEvent(this, { uMsg, wParam, lParam });
            }
            tResult = 1;
            break;
        case STN_DBLCLK:
            MSG_LOG(TEXT("STN_DBLCLK"));
            if (nullptr != OnDoubleClickedEvent) {
                OnDoubleClickedEvent(this, { uMsg, wParam, lParam });
            }
            tResult = 1;
            break;
        case STN_DISABLE: //PUSHED
            MSG_LOG(TEXT("STN_DISABLE"));
            if (nullptr != OnDisableEvent) {
                OnDisableEvent(this, { uMsg, wParam, lParam });
            }
            tResult = 1;
            break;
        case STN_ENABLE: //UNPUSHED
            MSG_LOG(TEXT("STN_ENABLE"));
            if (nullptr != OnEnableEvent) {
                OnEnableEvent(this, { uMsg, wParam, lParam });
            }
            tResult = 1;
            break;
        default:
            MSG_LOG(TEXT("Not supported %X"), uMsg);
            break;
        }
    }
    else { //search through children
        KLabel *  pFoundChildren = reinterpret_cast<KLabel*>(FindChildrenByID(LOWORD(wParam)));

        if (pFoundChildren) {
            tResult = pFoundChildren->ProcessCommandMessage(hWnd, uMsg, wParam, lParam);
        }
    }

    return tResult;
}

LRESULT KLabel::ProcessNotifyMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT tResult = 0;
    NMHDR * pData = reinterpret_cast<NMHDR*>(lParam);

    if (pData) {
        MSG_ERROR(TEXT("WM_NOTIFY: hwndFrom=0x%X, idFrom=%d, code=0x%X"), pData->hwndFrom, pData->idFrom, pData->code);
    }

    return tResult;
}