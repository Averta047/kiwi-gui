//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KMenu.h"

// Defines
#define CLASS_NAME                      TEXT("KMenu")
#define MAX_NUMBER_OF_DIGITS_FOR_INDEX  6

// Static elements
uint32_t KMenu::u32Index = 1;

KMenu::KMenu()
{
    this->m_hMenuHandle = CreateMenu();

    if (NULL != m_hMenuHandle) {

        this->m_Name = CLASS_NAME;

    // 1st: set MNS_NOTIFYBYPOS style so menu commands are processed by WM_MENUCOMMAND instead of WM_COMMAND
#if 0
        MENUINFO menuInfo = { 0 };
        BOOL fResult = FALSE;
        menuInfo.cbSize = sizeof(menuInfo);
        fResult = GetMenuInfo(m_hMenuHandle, &menuInfo);

        if (fResult) {
            menuInfo.fMask = MIM_STYLE;
            menuInfo.dwStyle |= MNS_NOTIFYBYPOS;

            fResult = SetMenuInfo(m_hMenuHandle, &menuInfo);

            if (FALSE == fResult) {
                MSG_ERROR(TEXT("SetMenuInfo failed with error = 0x%X"), GetLastError());
            }
        }
        else {
            MSG_ERROR(TEXT("GetMenuInfo failed with error = 0x%X"), GetLastError());
        }
#endif
        // Create Default Menu Name
        TCHAR szNameIndex[MAX_NUMBER_OF_DIGITS_FOR_INDEX] = { 0 }; // up to 5 digtits

        if (-1 == _stprintf_s(szNameIndex, MAX_NUMBER_OF_DIGITS_FOR_INDEX, TEXT("%d"), u32Index)) {
            MSG_ERROR(TEXT("CRITICAL_ERROR - window index buffer overflow"));
        }
        else {
            this->m_Name += KString(szNameIndex);
        }
        u32Index++;
        MSG_SUCCESS(TEXT("Created new menu: %s with ID=%d"), this->m_Name.c_str(), this->m_ID);
    }
    else {
        MSG_ERROR(TEXT("Failed to create menu with CreateMenu error = 0x%X"), GetLastError());
    }
}

KMenu::~KMenu()
{
}
