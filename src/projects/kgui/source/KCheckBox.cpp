//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KCheckBox.h"

// Defines
#define CLASS_NAME          TEXT("KCheckBox")
#define WIN32_CLASS_NAME    TEXT("BUTTON")
#define DEFAULT_TEXT        TEXT("Caption")

#define DEFAULT_WIDTH  75
#define DEFAULT_HEIGHT 25

// Local Memory
int KCheckBox::m_IndexPool = 1;

// Prototypes
LRESULT CALLBACK ChildWindProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

KCheckBox::KCheckBox(KWindow* pParent, int PosX, int PosY)
    :
    KWindow(
        pParent, 
        PosX, 
        PosY
    ),
    Checked(*this)
{
    this->m_Name = CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =     0;
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_NOTIFY;
    this->m_sWindowArgs.nWidth =        DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =       DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =         reinterpret_cast<HMENU>(this->m_ID);
    this->m_sWindowArgs.lpParam =       this;

    this->Create();

    BOOL fResult = SetWindowSubclass(this->m_hWinHandle, ChildWindProc, 0, (DWORD_PTR)this);

    if (FALSE == fResult) {
        MSG_ERROR(TEXT("SetWindowSubclass failed with error = 0x%X"), GetLastError());
    }
}

KCheckBox::~KCheckBox()
{
}

int KCheckBox::GetDebugIndex()
{
    int dReturn = this->m_IndexPool;
    this->m_IndexPool++;
    return dReturn;
}

LRESULT KCheckBox::ProcessCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT tResult = 0; // should return 1 if not used (no CB registered)

    if (LOWORD(wParam) == static_cast<WORD>(this->m_ID)) {
        switch (HIWORD(wParam)) {
        case BN_CLICKED:
            MSG_LOG(TEXT("BN_CLICKED"));
            if (nullptr != OnClickEvent) {
                OnClickEvent(this, { uMsg, wParam, lParam });
            }
            tResult = 1;
            break;
#if 0
        case BN_PAINT:
            MSG_LOG(TEXT("BN_PAINT"));
            if (nullptr != OnPaintEvent) {
                OnPaintEvent(this, { uMsg, wParam, lParam });
            }
            tResult = 1;
            break;
        case BN_HILITE: //PUSHED
            MSG_LOG(TEXT("PUSHED"));
            if (nullptr != OnPushedEvent) {
                OnPushedEvent(this, { uMsg, wParam, lParam });
            }
            tResult = 1;
            break;
        case BN_UNHILITE: //UNPUSHED
            MSG_LOG(TEXT("UNPUSHED"));
            if (nullptr != OnUnpushedEvent) {
                OnUnpushedEvent(this, { uMsg, wParam, lParam });
            }
            tResult = 1;
            break;
#endif
        case BN_DISABLE:
            MSG_LOG(TEXT("BN_DISABLE"));
            if (nullptr != OnDisableEvent) {
                OnDisableEvent(this, { uMsg, wParam, lParam });
            }
            tResult = 1;
            break;
        case BN_DOUBLECLICKED:
            MSG_LOG(TEXT("BN_DOUBLECLICKED"));
            if (nullptr != OnDoubleClickedEvent) {
                OnDoubleClickedEvent(this, { uMsg, wParam, lParam });
            }
            tResult = 1;
            break;
        case BN_SETFOCUS:
            MSG_LOG(TEXT("BN_SETFOCUS"));
            if (nullptr != OnSetFocusEvent) {
                OnSetFocusEvent(this, { uMsg, wParam, lParam });
            }
            tResult = 1;
            break;
        case BN_KILLFOCUS:
            MSG_LOG(TEXT("BN_KILLFOCUS"));
            if (nullptr != OnKillFocusEvent) {
                OnKillFocusEvent(this, { uMsg, wParam, lParam });
            }
            tResult = 1;
            break;
        default:
            MSG_ERROR(TEXT("Not supported %X"), uMsg);
            break;
        }
    }
    else { //search through children
        KCheckBox *  pFoundChildren = nullptr;

        pFoundChildren = reinterpret_cast<KCheckBox*>(FindChildrenByID(LOWORD(wParam)));

        if (pFoundChildren) {
            //tResult = pFoundChildren->ProcessCommandMessage(hWnd, uMsg, wParam, lParam);
        }
    }

    return tResult;
}

LRESULT KCheckBox::ProcessNotifyMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT tResult = 0;
    NMHDR*  pData = 0;

    pData = reinterpret_cast<NMHDR*>(lParam);

#if 0
    if (pData) {
        switch (pData->code) {
        case BCN_DROPDOWN:
            MSG_LOG(TEXT("BCN_DROPDOWN - %s"), this->m_Name.c_str());
            break;

        case BCN_HOTITEMCHANGE: //mouse is enetring button window
            MSG_LOG(TEXT("BCN_HOTITEMCHANGE - %s"), this->m_Name.c_str());
            tResult = 1;
            break;
        case NM_CUSTOMDRAW:
            MSG_LOG(TEXT("LNM_CUSTOMDRAW - %s"), this->m_Name.c_str());
            tResult = 1;
            break;
        default:
            MSG_ERROR(TEXT("WM_NOTIFY: hwndFrom=0x%X, idFrom=%d, code=0x%X"), pData->hwndFrom, pData->idFrom, pData->code);
            break;
        }
    }
#endif

    return tResult;
}


LRESULT KCheckBox::OnReceiveCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    constexpr auto callback_called = 1;
    constexpr auto callback_not_found = 0;

    LRESULT result = callback_not_found;

    switch (uMsg)
    {
    case WM_NCCREATE:
    {
        SetProp(hWnd, TEXT("CHECKSTATE"), (HANDLE)BST_UNCHECKED);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        LRESULT current = (LRESULT)GetProp(hWnd, TEXT("CHECKSTATE"));
        LRESULT newState = (current == BST_CHECKED) ? BST_UNCHECKED : BST_CHECKED;
        SetProp(hWnd, TEXT("CHECKSTATE"), (HANDLE)newState);

        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }

    case BM_GETCHECK:
    {
        result = (LRESULT)GetProp(hWnd, TEXT("CHECKSTATE"));
        break;
    }
    case BM_SETCHECK:
    {
        SetProp(hWnd, TEXT("CHECKSTATE"), (HANDLE)wParam);
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }
    case WM_NCDESTROY:
    {
        RemoveProp(hWnd, TEXT("CHECKSTATE"));
        break;
    }
    };

    return result;
}