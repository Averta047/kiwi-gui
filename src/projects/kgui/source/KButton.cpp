//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KButton.h"
#include "KDraw.h"

/* Defines */
#define CLASS_NAME          TEXT("KButton")
#define WIN32_CLASS_NAME    TEXT("BUTTON")
#define DEFAULT_TEXT        TEXT("Caption")

#define DEFAULT_WIDTH  75
#define DEFAULT_HEIGHT 25

/* Local Memory */
int KButton::m_IndexPool = 1;

/* Prototypes */
LRESULT CALLBACK ChildWindProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

KButton::KButton(KWindow* pParent, int PosX, int PosY)
    : 
    KWindow(
        pParent, 
        PosX, 
        PosY
    ),
    DefaultPushStyle(*this)
{
    this->m_Name = CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =     0;
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY/* | BS_OWNERDRAW*/;
    this->m_sWindowArgs.nWidth =        DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =       DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =         reinterpret_cast<HMENU>(this->m_ID);
    this->m_sWindowArgs.lpParam =       this;

    this->Create();

    if (FALSE == SetWindowSubclass(this->m_hWinHandle, ChildWindProc, 0, reinterpret_cast< DWORD_PTR>(this)))
    {
        MSG_ERROR(TEXT("SetWindowSubclass failed with error = 0x%X"), GetLastError());
    }
}

KButton::~KButton()
{
}

int KButton::GetDebugIndex()
{
    int index = this->m_IndexPool;
    this->m_IndexPool++;

    return index;
}

LRESULT KButton::ProcessCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{   
    constexpr auto callback_called = 1;
    constexpr auto callback_not_found = 0;

    LRESULT result = callback_not_found;

    if (LOWORD(wParam) == static_cast<WORD>(this->m_ID))
    {
        switch (HIWORD(wParam))
        {
        case BN_CLICKED:
        {
            MSG_LOG(TEXT("BN_CLICKED"));

            if (OnClickEvent)
            {
                OnClickEvent(this, { uMsg, wParam, lParam });
            }

            result = callback_called;
        }
        break;
        case BN_PAINT:
        {
            MSG_LOG(TEXT("BN_PAINT"));

            if (OnPaintEvent)
            {
                OnPaintEvent(this, { uMsg, wParam, lParam });
            }

            result = callback_called;
        }
        break;
        case BN_HILITE: //PUSHED
        {
            MSG_LOG(TEXT("PUSHED"));

            if (OnPushedEvent)
            {
                OnPushedEvent(this, { uMsg, wParam, lParam });
            }

            result = callback_called;
        }
        break;
        case BN_UNHILITE: //UNPUSHED
        {
            MSG_LOG(TEXT("UNPUSHED"));

            if (OnUnpushedEvent)
            {
                OnUnpushedEvent(this, { uMsg, wParam, lParam });
            }

            result = callback_called;
        }
        break;
        case BN_DISABLE:
        {
            MSG_LOG(TEXT("BN_DISABLE"));

            if (OnDisableEvent)
            {
                OnDisableEvent(this, { uMsg, wParam, lParam });
            }

            result = callback_called;
        }
        break;
        case BN_DOUBLECLICKED:
        {
            MSG_LOG(TEXT("BN_DOUBLECLICKED"));

            if (OnDoubleClickedEvent)
            {
                OnDoubleClickedEvent(this, { uMsg, wParam, lParam });
            }

            result = callback_called;
        }
        break;
        case BN_SETFOCUS:
        {
            MSG_LOG(TEXT("BN_SETFOCUS"));

            if (OnSetFocusEvent)
            {
                OnSetFocusEvent(this, { uMsg, wParam, lParam });
            }

            result = callback_called;
        }
        break;
        case BN_KILLFOCUS:
        {
            MSG_LOG(TEXT("BN_KILLFOCUS"));

            if (OnKillFocusEvent)
            {
                OnKillFocusEvent(this, { uMsg, wParam, lParam });
            }

            result = callback_called;
        }
        break;
        default:
        {
            MSG_LOG(TEXT("Not supported %X"), uMsg);
        }
        break;
        }
    }
#if 0
    else
    {
        //search through children
        KButton* pFoundChildren = reinterpret_cast<KButton*>(findChildrenByID(LOWORD wParam)));

        if (pFoundChildren)
        {
            tResult = pFoundChildren->processCommandMessage(hWnd, uMsg, wParam, lParam);
        }
    }
#endif

    return result;
}

LRESULT KButton::ProcessNotifyMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT tResult = 0;

    NMHDR* pData = 0;
    pData = reinterpret_cast<NMHDR*>(lParam);

#if 0
    if (pData)
    {
        switch (pData->code)
        {
        case BCN_DROPDOWN:
            MSG_LOG(TEXT("BCN_DROPDOWN - %s"), this->m_name.c_str());
            tResult = 1;
            break;
        case BCN_HOTITEMCHANGE: //mouse is enetring button window
            MSG_LOG(TEXT("BCN_HOTITEMCHANGE - %s"), this->m_name.c_str());
            tResult = 1;
            break;
        case NM_CUSTOMDRAW:
            MSG_LOG(TEXT("LNM_CUSTOMDRAW - %s"), this->m_name.c_str());
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
