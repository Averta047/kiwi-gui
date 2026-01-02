#include "KMDIClient.h"
#include "KForm.h"

/* Defines */
#define CLASS_NAME          TEXT("KMDIClient")
#define WIN32_CLASS_NAME    TEXT("MDICLIENT")
#define DEFAULT_TEXT        TEXT("Caption")

#define DEFAULT_WIDTH  200
#define DEFAULT_HEIGHT 200

/* Local Memory */
int KMDIClient::m_IndexPool = 1;

// TODO: fix this workaound, because Win32 message dispatcher was designed for only 1 MDI Client.
KMDIClient * g_pMDI_client = nullptr;

/* Prototypes */
LRESULT CALLBACK ChildWindProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

// TODO: Creating protection for creating only 1 MDI client.

KMDIClient::KMDIClient(KForm* pParent, int PosX, int PosY)
    : KWindow(
        pParent,
        PosX, 
        PosY
    )
{
    BOOL fResult = FALSE;
    this->m_Name = CLASS_NAME;

    //this->Events = { 0 };
    CLIENTCREATESTRUCT cs = { 0 };
    cs.idFirstChild = KGUI_ID_MDI_FIRSTCHILD;

    this->m_sWindowArgs.dwExStyle =     WS_EX_CLIENTEDGE; //WS_EX_CLIENTEDGE
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL;
    this->m_sWindowArgs.nWidth =        DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =       DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =         0;//this->m_ID;
    this->m_sWindowArgs.lpParam =       &cs;

    this->Create();

    if (pParent)
    {
        pParent->m_hMDIClient = this->m_hWinHandle;
    }

    g_pMDI_client = this;

    fResult = SetWindowSubclass(this->m_hWinHandle, ChildWindProc, 0, (DWORD_PTR)this);

    if ( FALSE == fResult)
    {
        MSG_ERROR(TEXT("SetWindowSubclass failed with error = 0x%X"), GetLastError());
    }
}

KMDIClient::~KMDIClient()
{
#if 0
    // when parent change this should be done
    KForm* pParent = NULL;

    pParent = dynamic_cast<KForm*>(m_pParent);
    if (pParent) {
        pParent->m_hMDIClient = NULL;
    }
#endif
}

int KMDIClient::GetDebugIndex()
{
    int dReturn = this->m_IndexPool;
    this->m_IndexPool++;

    return dReturn;
}

void KMDIClient::Cascade()
{
    SendMessage(this->m_hWinHandle, WM_MDICASCADE, 0, 0);
}

void KMDIClient::TileHorizontal()
{
    SendMessage(this->m_hWinHandle, WM_MDITILE, MDITILE_HORIZONTAL, 0);
}

void KMDIClient::TileVertical()
{
    SendMessage(this->m_hWinHandle, WM_MDITILE, MDITILE_VERTICAL, 0);
}

void KMDIClient::ArrangeIcons()
{
    SendMessage(this->m_hWinHandle, WM_MDIICONARRANGE, 0, 0);
}