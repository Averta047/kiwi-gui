#include "KForm.h"

// Defines
#define CLASS_NAME          TEXT("KForm")
#define WIN32_CLASS_NAME    CLASS_NAME
#define DEFAULT_TEXT        TEXT("Caption")

#define DEFAULT_WIDTH  200
#define DEFAULT_HEIGHT 200

// Local Memory
int KForm::m_IndexPool = 1;

// Prototypes
LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// TODO: fix this.
// Main window pointer workaround.
std::vector<KForm*> g_forms;

// keep dx and dy after KForm resize
typedef struct
{
    int Width;
    int Height;
} thDifference_t;

void KForm::Initialize()
{
    this->m_Name = CLASS_NAME;

    this->m_sWindowArgs.dwExStyle       = 0; // CS_HREDRAW | CS_VREDRAW instead manualy call repaint - http://msdn.microsoft.com/en-us/library/windows/desktop/ff700543%28v=vs.85%29.aspx
    this->m_sWindowArgs.lpClassName     = WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName    = DEFAULT_TEXT;
    // (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX); for non resizing instead of WS_OVERLAPPEDWINDOW (remove WS_THICKFRAME - xor)
    this->m_sWindowArgs.dwStyle          = WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW; // WS_POPUP - http://msdn.microsoft.com/en-us/library/windows/desktop/ms632600%28v=vs.85%29.aspx
    this->m_sWindowArgs.nWidth          = DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight         = DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu           = 0;
    this->m_sWindowArgs.lpParam         = this;
}

void KForm::Register_Class()
{
    static bool once = true; // Register this class once in a lifetime of application. Lazy implementation.

    if (true == once)
    {
        WNDCLASSEX window_class_ex = { 0 };

        window_class_ex.cbSize          = sizeof(window_class_ex);
        window_class_ex.style           = CS_HREDRAW | CS_VREDRAW;
        window_class_ex.lpfnWndProc     = static_cast<WNDPROC>(WinProc);
        window_class_ex.cbClsExtra      = NULL;
        window_class_ex.cbWndExtra      = sizeof(KForm*); // 4 bytes for 'this' pointer
        window_class_ex.hInstance       = this->m_sWindowArgs.hInstance;
        window_class_ex.hIcon           = NULL;
        window_class_ex.hCursor         = LoadCursor(NULL, IDC_ARROW);
        window_class_ex.hbrBackground   = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
        window_class_ex.lpszMenuName    = NULL;
        window_class_ex.lpszClassName   = this->m_sWindowArgs.lpClassName;
        window_class_ex.hIconSm         = NULL;
    
        if (0 != RegisterClassEx(&window_class_ex))
        {
            once = false;
        }
        else
        {
            MSG_WARNING(TEXT("RegisterClassEx error: 0x%X"), GetLastError());
        }
    }
}

KForm::KForm(KWindow* pParent, int PosX, int PosY)
    :
    KWindow(pParent, PosX, PosY),
    m_pMenu(nullptr),
    m_hMDIClient(nullptr),
    Resizable(*this)
{
    
    this->Initialize();
    this->Register_Class();

    g_forms.push_back( this);

    this->Create();
}

KForm::~KForm()
{
    auto position = 0;

    for (const auto i: g_forms)
    {
        if ( i == this)
        {
            g_forms.erase(g_forms.begin() + position);
            break;
        }

        position++;
    }
}

LRESULT KForm::ProcessMenuCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT     tResult = 0;

    if (m_pMenu)
    {
        tResult = m_pMenu->ProcessCommandMessage(hWnd, uMsg, wParam, lParam);
    }

    if ( FALSE == tResult)
    {
        if (pPopupMenu)
        {
            tResult = pPopupMenu->ProcessCommandMessage(hWnd, uMsg, wParam, lParam);
        }
    }

    return tResult;
}

int KForm::GetDebugIndex()
{
    int dReturn = this->m_IndexPool;
    this->m_IndexPool++;

    return dReturn;
}

void KForm::SetMenu(KMenu* const pMenu)
{
    if (NULL == pMenu)
    {
        MSG_ERROR(TEXT("Empty input pointer!"));
        return;
    }

    if ( pMenu != m_pMenu)
    {
        if ( NULL != pMenu->m_hMenuHandle)
        {
            m_pMenu = pMenu;

            if ( FALSE == ::SetMenu( this->m_hWinHandle, pMenu->m_hMenuHandle))
            {
                MSG_ERROR(TEXT("SetMenu failed with error =  0x%X"), GetLastError());
            }
            else
            {
                m_pMenu->m_hParentWindow = this->m_hWinHandle;
                DrawMenuBar( this->m_hWinHandle);
            }
        }
        else
        {
            MSG_ERROR(TEXT("Provided thMenu is invalid (wrong handle)"));
        }
    }
}

void KForm::ClearMenu()
{
    if (FALSE == ::SetMenu(this->m_hWinHandle, NULL))
    {
        MSG_ERROR( TEXT( "SetMenu failed with error =  0x%X"), GetLastError());
    }
    else
    {
        this->m_pMenu->m_hParentWindow = NULL;
        m_pMenu = NULL;
    }
}

BOOL KForm::IsMenuEnabled()
{
    BOOL result = false;

    if ( GetMenu( this->m_hWinHandle))
    {
        result = true;
    }

    return result;
}