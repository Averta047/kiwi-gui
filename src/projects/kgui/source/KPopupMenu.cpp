//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KPopupMenu.h"

// Defines
#define CLASS_NAME                      TEXT("KPopupMenu")
#define MAX_NUMBER_OF_DIGITS_FOR_INDEX  6

// Static elements
uint32_t KPopupMenu::u32Index = 1;

KPopupMenu::KPopupMenu()
{
    this->m_hMenuHandle = 0;

    this->m_hMenuHandle = CreatePopupMenu();

    if (NULL != m_hMenuHandle) {
        BOOL fResult = FALSE;

        this->m_Name = CLASS_NAME;

        /* 1st: set MNS_NOTIFYBYPOS style so menu commands are processed by WM_MENUCOMMAND instead of WM_COMMAND */
#if 0
        MENUINFO menuInfo = { 0 };

        menuInfo.cbSize = sizeof(menuInfo);
        fResult = GetMenuInfo(m_hMenuHandle, &menuInfo);

        if (FALSE == fResult) {
            MSG_ERROR(TEXT("GetMenuInfo failed with error = 0x%X"), GetLastError());
        }
        else {
            menuInfo.fMask = MIM_STYLE;
            menuInfo.dwStyle |= MNS_NOTIFYBYPOS;

            fResult = SetMenuInfo(m_hMenuHandle, &menuInfo);

            if (FALSE == fResult) {
                MSG_ERROR(TEXT("SetMenuInfo failed with error = 0x%X"), GetLastError());
            }
        }
#endif
        /* Create Default Menu Name */
        TCHAR szNameIndex[MAX_NUMBER_OF_DIGITS_FOR_INDEX] = { 0 }; // up to 5 digtits

        if (-1 == _stprintf_s(szNameIndex, MAX_NUMBER_OF_DIGITS_FOR_INDEX, TEXT("%d"), u32Index)) {
            MSG_ERROR(TEXT("CRITICAL_ERROR - window index buffer overflow"));
        }
        else {
            this->m_Name += KString(szNameIndex);
        }
        u32Index++;
        MSG_SUCCESS(TEXT("Created new popup menu: %s with ID=%d"), this->m_Name.c_str(), this->m_ID);
    }
    else {
        MSG_ERROR(TEXT("Failed to create popup menu with CreateMenu error = 0x%X"), GetLastError());
    }
}

KPopupMenu::~KPopupMenu()
{
}

void KPopupMenu::Show(HWND Parent, POINT & Point)
{
    BOOL fResult = false;

    //fResult = SetForegroundWindow(Parent->m_hWinHandle);

    //if (false == fResult) {
    //    MSG_ERROR(TEXT("SetForegroundWindow failed with error = 0x%X"), GetLastError());
    //}

    //fResult = false;
    fResult = TrackPopupMenu(
        this->m_hMenuHandle,
        TPM_TOPALIGN | TPM_LEFTALIGN,
        Point.x,
        Point.y,
        0,
        Parent,
        NULL
    );

    if (false == fResult) {
        MSG_ERROR(TEXT("TrackPopupMenu failed with error = 0x%X"), GetLastError());
    }
}

void KPopupMenu::Show(KWindow* Parent, POINT& Point)
{
    BOOL fResult = false;

    if (Parent) {
        //fResult = SetForegroundWindow(a_parent->m_hWinHandle);

        //if (false == fResult) {
        //    MSG_ERROR(TEXT("SetForegroundWindow failed with error = 0x%X"), GetLastError());
        //}

        //fResult = false;
        fResult = TrackPopupMenu(
            this->m_hMenuHandle,
            TPM_TOPALIGN | TPM_LEFTALIGN,
            Point.x,
            Point.y,
            0,
            Parent->GetHandle(),
            NULL
        );

        if (false == fResult) {
            MSG_ERROR(TEXT("TrackPopupMenu failed with error = 0x%X"), GetLastError());
        }
    }
    else {
        MSG_ERROR(TEXT("Empty pointer as argument!"));
    }
}