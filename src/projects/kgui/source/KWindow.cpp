//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KWindow.h"

#include "KForm.h"
#include "KMDIClient.h"
#include "KMDIChild.h"

// Defines
#define MAX_NUMBER_OF_DIGITS_FOR_INDEX 6

#define CUSTOM_TITLEBAR_HEIGHT 30
#define CUSTOM_BORDER_WIDTH 0
#define CUSTOM_CORNER_SIZE 0

// Prototypes
LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

KWindow::KWindow(KWindow* pParent, int PosX, int PosY)
    :
    m_pParent(nullptr),
    m_hWinHandle(NULL),
    pPopupMenu(nullptr),
    Text(*this),
    Width(*this),
    Height(*this),
    X(*this),
    Y(*this),
    Font(*this),
    Theme(*this)
{
    m_sWindowArgs = { 0 };
    m_sWindowArgs.hMenu = reinterpret_cast<HMENU>(this->m_ID);;
    m_sWindowArgs.nX = PosX;
    m_sWindowArgs.nY = PosY;
    m_sWindowArgs.nWidth = CW_USEDEFAULT;
    m_sWindowArgs.nHeight = CW_USEDEFAULT;
//    m_sWindowArgs.dwExStyle = 0;

    if (pParent)
    {
        m_pParent = pParent;
        m_sWindowArgs.hWndParent = m_pParent->m_hWinHandle;
        m_sWindowArgs.hInstance = m_pParent->m_sWindowArgs.hInstance;
        
        pParent->AddChildrenWindow(this);
    }
    else
    {
        m_sWindowArgs.hInstance = GetModuleHandle(NULL);
        MSG_WARNING(TEXT("Empty input pointer! Using module handle as parent instance"));
    }
}

KWindow::~KWindow()
{
}

void KWindow::Create()
{
    CreateDebugName();

    if (NULL == m_hWinHandle)
    {
        // Set the size, but not the position.
        RECT rcAdjWidth = {
                0,
                0,
                m_sWindowArgs.nWidth,
                m_sWindowArgs.nHeight
        };
        
        // Calculate window client area (no borders) and adjust the size.
        BOOL result = AdjustWindowRectEx(
            &rcAdjWidth,
            m_sWindowArgs.dwStyle,
            FALSE,
            m_sWindowArgs.dwExStyle
        );

        if (FALSE == result)
        {
            MSG_ERROR(TEXT( "AdjustWindowRectEx failed with error = 0x%X"), GetLastError());
        }

        m_hWinHandle = CreateWindowEx(
            m_sWindowArgs.dwExStyle,
            m_sWindowArgs.lpClassName,
            m_sWindowArgs.lpWindowName,
            m_sWindowArgs.dwStyle,
            m_sWindowArgs.nX,
            m_sWindowArgs.nY,
            rcAdjWidth.right,
            rcAdjWidth.bottom,
            m_sWindowArgs.hWndParent,
            m_sWindowArgs.hMenu,
            m_sWindowArgs.hInstance,
            m_sWindowArgs.lpParam
        );

        if (m_hWinHandle)
        {
            MSG_SUCCESS(TEXT("Created new window: %s with ID=%d"), this->m_Name.c_str(), this->m_ID);

            //SetLayeredWindowAttributes(m_hWinHandle, 0, 0, LWA_ALPHA);

            //SetWindowTheme(m_hWinHandle, L"", L"");

            this->OnCreate();

            // set defualt fond name and size. It must be done after successfull window creation
            this->Font.SetName(KGUI_DEF_APP_FONT_NAME);
            this->Font.SetSize(KGUI_DEF_APP_FONT_SIZE);

            this->Theme.SetTheme(kTheme_Dark);

            const LONG_PTR pUserData = GetWindowLongPtr(m_hWinHandle, GWLP_USERDATA);

            if (NULL == pUserData)
            {
                // Set 'this' pointer to user data in Window system object so it can be accessed by WinProc.
                SetWindowLongPtr(m_hWinHandle, GWLP_USERDATA, reinterpret_cast< LONG_PTR>( this));
            }

            this->StoreCurrentRect();
        }
        else
        {
            MSG_ERROR(TEXT("Failed to create window: %s, CreateWindowEx returned error: 0x%X"), this->m_Name.c_str(), GetLastError());
        }
    }
}

LRESULT KWindow::OnReceiveCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT tResult = 0;

    if (OnReceiveCommandMessageEvent)
    {
        tResult = OnReceiveCommandMessageEvent(this, { uMsg, wParam, lParam });
    }

    return tResult;
}

LRESULT KWindow::OnCreate()
{
    LRESULT tResult = 0;

    return tResult;
}

// NCCreate must return 1! If not, CreateWindowEx won't create window and will return NULL
LRESULT KWindow::OnNCCreate()
{
    return 1;
}

LRESULT KWindow::OnClose()
{
    return 0;
}

LRESULT KWindow::OnDestroy()
{
    LRESULT tResult = 0;

    if (OnDestroyEvent)
    {
        tResult = OnDestroyEvent(this, {});

        if (0 != tResult)
        {
            PostQuitMessage(0);
        }
    }

    return tResult;
}

LRESULT KWindow::OnFocus()
{
    return 0;
}

LRESULT KWindow::OnKillFocus()
{
    return 0;
}

KWindow * KWindow::GetParent() const
{
    return this->m_pParent;
}

// a_lParam is pointer to char* ended with \0
LRESULT KWindow::OnSetText(LPARAM lParam)
{
    return 0;
}

// If an application processes this message, it should return zero.
// a_wParam - low-order word is new width
// a_lParam - high-order word is new height
LRESULT KWindow::OnResize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    // loop through this window children
    for (const auto & i: m_Children)
    {
        // skip KForm and KMDIChild windows
        if (nullptr == dynamic_cast< KForm*>(i) && nullptr == dynamic_cast< KMDIChild*>(i))
        {
            RECT rcCurPos = { 0 };

            i->GetRect(rcCurPos);

            LONG NewX = rcCurPos.left;
            LONG NewY = rcCurPos.top;
            LONG NewW = i->Width;
            LONG NewH = i->Height;

            if (false == i->Anchors.Left)
            {
                if (rcCurPos.right > i->m_rcOldPosition.right)
                {
                    NewX += (rcCurPos.right - i->m_rcOldPosition.right);
                }
                else
                {
                    NewX -= (i->m_rcOldPosition.right - rcCurPos.right);
                }

                if (i->m_pParent)
                {
                    rcCurPos.right = i->m_pParent->Width - (NewX + static_cast< LONG>(i->Width));
                }
            }

            if (false == i->Anchors.Top)
            {
                if (rcCurPos.bottom > i->m_rcOldPosition.bottom)
                {
                    NewY += (rcCurPos.bottom - i->m_rcOldPosition.bottom);
                }
                else
                {
                    NewY -= (i->m_rcOldPosition.bottom - rcCurPos.bottom);
                }

                if (i->m_pParent)
                {
                    rcCurPos.bottom = i->m_pParent->Height - (NewX + static_cast< LONG>(i->Height));
                }
            }

            if (true == i->Anchors.Right)
            {
                if ( rcCurPos.right > i->m_rcOldPosition.right)
                {
                    NewW += (rcCurPos.right - i->m_rcOldPosition.right);
                }
                else
                {
                    NewW -= (i->m_rcOldPosition.right - rcCurPos.right);
                }
            }

            if (true == i->Anchors.Bottom)
            {
                if (rcCurPos.bottom > i->m_rcOldPosition.bottom)
                {
                    NewH += (rcCurPos.bottom - i->m_rcOldPosition.bottom);
                }
                else
                {
                    NewH -= (i->m_rcOldPosition.bottom - rcCurPos.bottom);
                }
            }

            BOOL fResult = SetWindowPos(
                i->m_hWinHandle,
                NULL,
                NewX,
                NewY,
                NewW,
                NewH,
                SWP_NOZORDER
            );

            if (FALSE == fResult)
            {
                MSG_ERROR(TEXT("SetWindowPos failed with error = 0x%X"), GetLastError());
            }

            result = 1; // this will resize all children manually, so dont send WM_SIZE to children
            
            KMDIClient* mdi = dynamic_cast< KMDIClient*>(i);

            if (nullptr != mdi)
            { 
                return 1;
            }
        }
        else
        {
            result = 0;
        }
    }

    return result;
}


// a_wParam - handle to the window user right clicked. this can be a child
// a_lParam - low-order word is X, high-order word is Y
// return 0 will process this message further in Z-axis
// return 1 will stop processing this msg
LRESULT KWindow::OnContextMenu( WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 1;
    POINT   point = { 0 };
    HWND    hwnd = 0;

    hwnd = reinterpret_cast< HWND>(wParam);
    point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

    if (pPopupMenu)
    {
        pPopupMenu->Show(hwnd, point);
    }

    return result;
}

// KForm need specialized version
LRESULT KWindow::OnGetMinMax(LPARAM lParam)
{
    constexpr LRESULT message_not_proccessed{0U};
    constexpr LRESULT message_proccessed{1U};

    LRESULT     result = message_proccessed;
    MINMAXINFO* sInfo = reinterpret_cast< MINMAXINFO*>(lParam);

    if (sInfo)
    {
        if (this->Constraints.MaxWidth)
        {
            sInfo->ptMaxTrackSize.x = this->Constraints.MaxWidth;
            result = message_not_proccessed;
        }

        if (this->Constraints.MaxHeight)
        {
            sInfo->ptMaxTrackSize.y = this->Constraints.MaxHeight;
            result = message_not_proccessed;
        }

        if (this->Constraints.MinWidth)
        {
            sInfo->ptMinTrackSize.x = this->Constraints.MinWidth;
            result = message_not_proccessed;
        }

        if (this->Constraints.MinHeight)
        {
            sInfo->ptMinTrackSize.y = this->Constraints.MinHeight;
            result = message_not_proccessed;
        }
    }

    return result;
}

LRESULT KWindow::ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    constexpr LRESULT message_not_proccessed{0U};

    LRESULT result = message_not_proccessed;

    if (OnReceiveCommandMessage(hWnd, uMsg, wParam, lParam))
    {
        return TRUE;
    }

    switch (uMsg)
    {
#if 0
    case WM_PAINT:
    {
        if (!m_pParent) // check if we are the base window
        {
            KStyle style = Theme.GetStyle();
            KDraw draw(*this);

            draw.AddRectFilled(KFloat2(0, 0), KFloat2(Width, Height), style.Colors[kCol_WindowBg], style.WindowRounding);
        }
        break;
    }

    case WM_NCHITTEST:
    {
        if (!m_pParent)
        {
            // let the default handler handle most cases
            LRESULT hit = DefWindowProc(hWnd, uMsg, wParam, lParam);

            // if the mouse is in the client area, treat it as caption
            if (hit == HTCLIENT)
            {
                return HTCAPTION;
            }
            result = hit;
        }
        break;
    }
#endif
    case WM_NOTIFY:
    {
        result = this->ProcessNotifyMessage(hWnd, uMsg, wParam, lParam);
        break;
    }
    case WM_COMMAND:
    {
        result = this->ProcessCommandMessage(hWnd, uMsg, wParam, lParam);

        if ( message_not_proccessed == result)
        {
            result = this->ProcessMenuCommandMessage(hWnd, uMsg, wParam, lParam);
        }
        break;
    }
#if 0
    case WM_MENUCOMMAND:
        tResult = this->processMenuCommandMessage(hWnd, uMsg, wParam, lParam);
        break;
#endif
    case WM_CONTEXTMENU:
    {
        result = this->OnContextMenu(wParam, lParam);
        break;
    }
    case WM_GETMINMAXINFO:
    {
        result = OnGetMinMax(lParam);
        break;
    }
    case WM_NCCREATE:
    {
        result = this->OnNCCreate();
        break;
    }
    case WM_NCDESTROY:
    {
        if (m_pParent)
        {
            m_pParent->RemoveChildrenWindow(this);
        }

        result = this->OnDestroy();
        break;
    }
#if 0 
    // does not work, instead called in Create
    case WM_CREATE:
        result = this->OnCreate();
        break;
#endif
    case WM_CLOSE:
    {
        result = this->OnClose();
        break;
    }
#if 0
    // not needed
    case WM_DESTROY:
        tResult = this->OnDestroy();
        break;
#endif
    case WM_SIZE:
    {
        InvalidateRect(hWnd, NULL, TRUE);
        result = this->OnResize(hWnd, wParam, lParam);
        break;
    }
    case WM_SETFOCUS:
    {
        result = this->OnFocus();
        break;
    }
    case WM_KILLFOCUS:
    {
        result = this->OnKillFocus();
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        //MSG_ERROR(TEXT("WM_CTLCOLORSTATIC - not supported atm"));
        break;
    }
    case WM_SETTEXT:
    {
        result = this->OnSetText(lParam);
        break;
    }
    }

    return result;
}

// Pass command message to children.
LRESULT KWindow::ProcessCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = SendMessage(reinterpret_cast< HWND>(lParam), WM_COMMAND, wParam, lParam);

    return result;
}

// Pass command message to menu.
LRESULT KWindow::ProcessMenuCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    constexpr LRESULT message_not_proccessed{0U};
    LRESULT result = message_not_proccessed;

    if (pPopupMenu)
    {
        result = pPopupMenu->ProcessCommandMessage(hWnd, uMsg, wParam, lParam);
    }

    return result;
}

LRESULT KWindow::ProcessNotifyMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    constexpr LRESULT message_not_proccessed{0U};

    LRESULT result = message_not_proccessed;

    const NMHDR * const pData = reinterpret_cast<NMHDR*>(lParam);

    if (pData)
    {
        result = SendMessage(pData->hwndFrom, WM_NOTIFY, wParam, lParam);
    }

    return result;
}

// pre-condition: m_name should be already set by class deriving from KWindow
void KWindow::CreateDebugName()
{
    TCHAR   szNameIndex[MAX_NUMBER_OF_DIGITS_FOR_INDEX] = { 0 }; // up to 5 digtits
    int     index = this->GetDebugIndex();

    if (-1 == _stprintf_s(szNameIndex, MAX_NUMBER_OF_DIGITS_FOR_INDEX, TEXT("%d"), index))
    {
        MSG_ERROR(TEXT("Window index buffer overflow"));
    }
    else
    {
        this->m_Name += KString(szNameIndex);
    }
}

void KWindow::AddChildrenWindow(KWindow* pChildren)
{
    if (pChildren)
    {
        bool isDublicateFound = false;

        // check for dublicates
        for ( const auto & i: m_Children)
        {
            if (pChildren == i)
            {
                isDublicateFound = true;
                break;
            }
        }

        if ( false == isDublicateFound)
        {
            this->m_Children.push_back(pChildren);
        }
    }
}

void KWindow::RemoveChildrenWindow(KWindow* pChildren)
{
    if (pChildren)
    {
        int position = 0;

        for (const auto & i: m_Children)
        {
            if (pChildren == i)
            {
                m_Children.erase(m_Children.begin() + position);
                break;
            }

            ++position;
        }
    }
}

KWindow * KWindow::FindChildrenByID(const WORD SearchedId)
{
    KWindow * pFoundChildren = nullptr;

    for ( const auto & i : m_Children)
    {
        if (SearchedId == static_cast< WORD>(i->m_ID))
        {
            pFoundChildren = i;
            break;
        }
    }

    return pFoundChildren;
}

// developer is responsible for de-allocating memory
#if 0
void KWindow::DestroyChildren()
{
    std::vector<KWindow*>::iterator i;

    i = this->m_children.begin();

    for (; i != m_children.end(); i++)
    {
        if (NULL != (*i))
        {
            delete (*i);
            (*i) = NULL;
        }
    }
}
#endif

void KWindow::Show()
{
    ShowWindow(this->m_hWinHandle, SW_SHOWNORMAL);
}

void KWindow::Hide()
{
    ShowWindow(this->m_hWinHandle, SW_HIDE);
}

void KWindow::Maximize()
{
    ShowWindow(this->m_hWinHandle, SW_MAXIMIZE);
}

void KWindow::Minimize()
{
    ShowWindow(this->m_hWinHandle, SW_MINIMIZE);
}

void KWindow::Enable()
{
    EnableWindow(this->m_hWinHandle, TRUE);
}

void KWindow::Disable()
{
    EnableWindow(this->m_hWinHandle, FALSE);
}

void KWindow::Destroy()
{
    if (FALSE == DestroyWindow( this->m_hWinHandle))
    {
        MSG_ERROR(TEXT("DestroyWindow failed with error = 0x%X"), GetLastError());
    }
}

void KWindow::SetFocus()
{
    if (NULL == ::SetFocus(this->m_hWinHandle))
    {
        MSG_ERROR(TEXT("SetFocus failed with error = 0x%X"), GetLastError());
    }
}

// Store current position in m_rcOldPosition. m_rcOldPosition is used when resizing
void KWindow::StoreCurrentRect()
{
    this->GetRect(this->m_rcOldPosition);
}

// store current window position in an argument
void KWindow::GetRect(RECT& rcOutput)
{
    if ( m_pParent)
    {
        rcOutput.left   = this->X;
        rcOutput.top    = this->Y;
        rcOutput.bottom = m_pParent->Height - (this->Y + static_cast<LONG>(this->Height));
        rcOutput.right  = m_pParent->Width - (this->X + static_cast<LONG>(this->Width));
#if 0
        MSG_SUCCESS(TEXT("Window %s position: L=%d, T=%d, B=%d, R=%d"), this->m_name.c_str(),
            rcOutput.left, rcOutput.top, rcOutput.bottom, rcOutput.right);
#endif
    }
}

const HWND KWindow::GetHandle() const
{
     return this->m_hWinHandle; 
}

// Parent Window Procedure
LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    constexpr LRESULT message_not_proccessed{0U};

    LRESULT result = message_not_proccessed;

    // Retrieve 'this' pointer from passed by CreateWindowEx.
    const LONG_PTR      pUserData = GetWindowLongPtr(hWnd, GWLP_USERDATA);
    KWindow * const     pWindow = reinterpret_cast< KWindow *>(pUserData);
    
    if (pWindow)
    {
        result = pWindow->ProcessMessage(hWnd, uMsg, wParam, lParam);

        if (message_not_proccessed == result)
        {
            // Messages passed to MDIClient must be passed to DefFrameProc.
            const KForm * const pForm = dynamic_cast< KForm*>(pWindow);

            if (pForm && pForm->m_hMDIClient)
            {
                result = DefFrameProc(hWnd, pForm->m_hMDIClient, uMsg, wParam, lParam);
            }
            else
            {
                result = DefWindowProc(hWnd, uMsg, wParam, lParam);
            }
        }
    }
    else
    {
        result = DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return result;
}

// Children Window Procedure
LRESULT CALLBACK ChildWindProc(
    HWND        hWnd,
    UINT        uMsg,
    WPARAM      wParam,
    LPARAM      lParam,
    UINT_PTR    uIdSubclass,
    DWORD_PTR   dwRefData
)
{
    constexpr LRESULT message_not_proccessed{ 0U};

    LRESULT result = message_not_proccessed;

    if ( WM_NCDESTROY == uMsg)
    {
        BOOL fResult = RemoveWindowSubclass(hWnd, ChildWindProc, uIdSubclass);

        if ( FALSE == fResult)
        {
            MSG_ERROR( TEXT( "RemoveWindowSubclass failed"));
        }
    }

    KWindow * const pWindow = reinterpret_cast<KWindow*>(dwRefData);

    if (pWindow)
    {
        result = pWindow->ProcessMessage(hWnd, uMsg, wParam, lParam);

        if ( message_not_proccessed == result)
        {
            result = DefSubclassProc(hWnd, uMsg, wParam, lParam);
        }
    }
    else
    {
        result = DefSubclassProc(hWnd, uMsg, wParam, lParam);
    }

    return result;
}