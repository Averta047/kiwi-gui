//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KMDIChild.h"

/* Defines */
#define CLASS_NAME          TEXT("KMDIChild")
#define WIN32_CLASS_NAME    CLASS_NAME
#define DEFAULT_TEXT        TEXT("KMDIChild")

#define DEFAULT_WIDTH  200
#define DEFAULT_HEIGHT 200

/* Local Memory */
int KMDIChild::m_IndexPool = 1;

void KMDIChild::Initialize()
{
    this->m_Name = CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =      WS_EX_MDICHILD; //http://msdn.microsoft.com/en-us/library/windows/desktop/ff700543%28v=vs.85%29.aspx
    this->m_sWindowArgs.lpClassName =    WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =   DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =        WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN; //http://msdn.microsoft.com/en-us/library/windows/desktop/ms632600%28v=vs.85%29.aspx
    this->m_sWindowArgs.nWidth =         DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =        DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =          0;
    this->m_sWindowArgs.lpParam =        this;
}

void KMDIChild::Register_Class()
{
    static bool once = true; // Register this class once in a lifetime of application. Lazy implementation.

    if ( true == once)
    {
        WNDCLASSEX wClass = { 0 };

        wClass.cbSize           = sizeof(WNDCLASSEX);
        wClass.style            = 0; // CS_HREDRAW | CS_VREDRAW;
        wClass.lpfnWndProc      = static_cast<WNDPROC>(MDIChildProc);
        wClass.cbClsExtra       = NULL;
        wClass.cbWndExtra       = sizeof(KMDIChild*); // 4 bytes for 'this' pointer
        wClass.hInstance        = this->m_sWindowArgs.hInstance;
        wClass.hIcon            = NULL;
        wClass.hCursor          = LoadCursor(NULL, IDC_ARROW);
        wClass.hbrBackground    = (HBRUSH)COLOR_WINDOW;
        wClass.lpszMenuName     = NULL;
        wClass.lpszClassName    = this->m_sWindowArgs.lpClassName;
        wClass.hIconSm          = NULL;
    
        if (0 == RegisterClassEx(&wClass)) {
            MSG_WARNING(TEXT("RegisterClassEx error: 0x%X"), GetLastError());
            //todo: add UnregisterClass 
        }
        else {
            MSG_SUCCESS(TEXT("Successfully registerd class with RegisterClassEx"));
            once = false;
            //fMainApplicationWindow = true; //firstly created form is the main form
        }
    }
}

KMDIChild::KMDIChild(KMDIClient * pParent, int PosX, int PosY)
    :
    m_pMenu(NULL),
    KWindow(pParent, PosX, PosY)
{
    this->Initialize();
    this->Register_Class();
    this->Create();
}

KMDIChild::~KMDIChild()
{
}

LRESULT KMDIChild::OnCreate()
{
    LRESULT tResult = 0;

    return tResult;
}

// NCCreate must return 1! If not, CreateWindowEx won't create window and will return NULL
LRESULT KMDIChild::OnNCCreate()
{
    LRESULT tResult = 1;

    return tResult;
}

LRESULT KMDIChild::OnClose()
{
    MSG_LOG(TEXT("%s::onClose() - Enter"), this->m_Name.c_str());
    LRESULT tResult = 0;

    if (OnCloseEvent) {
        tResult = OnCloseEvent(this,{});
    }

    MSG_LOG(TEXT("%s::onClose() - Leave"), this->m_Name.c_str());
    return tResult;
}

#if 0
LRESULT KMDIChild::OnResize(HWND hWnd)
{
    LRESULT tResult = 1;

    return tResult;
}
#endif

#if 0
LRESULT KMDIChild::ProcessCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT     tResult = 0;
    
//    tResult = SendMessage((HWND)a_lParam, WM_COMMAND, a_wParam, a_lParam);

    return tResult;
}
#endif

#if 0
// wParam - the zero - based index of the item selected
// lParam - a handle to the menu for the item selected
LRESULT KMDIChild::processMenuCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT     tResult = 0;

    if (m_menu) {
        tResult = m_pMenu->processCommandMessage(hWnd, uMsg, wParam, lParam);
    }

    if (FALSE == tResult) {
        if (PopupMenu) {
            tResult = pPopupMenu->processCommandMessage(hWnd, uMsg, wParam, lParam);
        }
    }

    return tResult;
}
#endif

int KMDIChild::GetDebugIndex()
{
    int dReturn = this->m_IndexPool;
    this->m_IndexPool++;

    return dReturn;
}

#if 0
void KMDIChild::SetMenu(thMenu* const pMenu)
{
    if (NULL != pMenu) {
        if (pMenu != m_pMenu) {
            if (NULL != pMenu->m_hMenuHandle) {
                BOOL fResult = false;

                m_pMenu = pMenu;
                fResult = ::SetMenu(this->m_hWinHandle, pMenu->m_hMenuHandle);

                if (false == fResult) {
                    MSG_ERROR(TEXT("SetMenu failed with error =  0x%X"), GetLastError());
                }
                else {
                    //todo:
                    //this->addChildrenWindow(pMenu);
                    //DrawMenuBar(this->hWinHandle);
                    m_pMenu->m_hParentWindow = this->m_hWinHandle;
                    DrawMenuBar(this->m_hWinHandle);
                }
            }
            else {
                MSG_ERROR(TEXT("Provided thMenu is invalid (wrong handle)"));
            }
        }
    }
    else {
        MSG_ERROR(TEXT("Empty input pointer!"));
    }
}

void KMDIChild::ClearMenu()
{
    BOOL fResult = false;

    fResult = ::SetMenu(this->m_hWinHandle, NULL);

    if (false == fResult) {
        MSG_ERROR(TEXT("SetMenu failed with error =  0x%X"), GetLastError());
    }
    else {
        this->m_menu->m_hParentWindow = NULL;
        m_menu = NULL;
    }
}

BOOL KMDIChild::IsMenuEnabled()
{
    BOOL    fResult = false;
    HMENU   hMenu = 0;

    hMenu = GetMenu(this->m_hWinHandle);

    if (hMenu) {
        fResult = true;
    }

    return fResult;
}
#endif

LRESULT CALLBACK MDIChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG_PTR        pUserData = NULL;
    LRESULT         tResult = 0;
    KMDIChild *    pMDIChild = NULL;

    pUserData = GetWindowLongPtr(hWnd, GWLP_USERDATA);
    pMDIChild = reinterpret_cast<KMDIChild *>(pUserData);

    if (WM_NCCREATE == uMsg) {
        LPCREATESTRUCT cs = (LPCREATESTRUCT)lParam;
        if (cs->lpCreateParams) {
            MDICREATESTRUCT * pMDIinfo = NULL;

            pMDIinfo = reinterpret_cast<MDICREATESTRUCT*>(cs->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, pMDIinfo->lParam);
            pMDIChild = reinterpret_cast<KMDIChild*>(pMDIinfo->lParam);
        }
        else {
            MSG_ERROR(TEXT("CRITICAL ERROR - no 'this' pointer in CreateWindowEx Param!"));
        }
    }

    if (pMDIChild) {
        tResult = pMDIChild->ProcessMessage(hWnd, uMsg, wParam, lParam);

        // msgs that must be passed through or msgs not handled
        // https://msdn.microsoft.com/en-us/library/windows/desktop/ms644925%28v=vs.85%29.aspx
        if (0 == tResult || WM_SIZE == uMsg
            || WM_CHILDACTIVATE == uMsg || WM_GETMINMAXINFO == uMsg
            || WM_MENUCHAR == uMsg || WM_MOVE == uMsg
            || WM_SETFOCUS == uMsg || WM_SYSCOMMAND == uMsg
        ) 
        {
            tResult = DefMDIChildProc(hWnd, uMsg, wParam, lParam);
        }
    }
    else {
        tResult = DefMDIChildProc(hWnd, uMsg, wParam, lParam);
    }

    return tResult;
}