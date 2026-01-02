//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

//http://www.functionx.com/win32/Lesson07.htm

#include "KMenuCommon.h"

/* KMenuCommon */
KMenuCommon::KMenuCommon() :
    Items(*this),
    m_hMenuHandle(NULL),
    m_hParentWindow(NULL)
{
}

KMenuCommon::~KMenuCommon()
{
    // if menu has no parent it need to be destroyed manually
    DestroyMenu(this->m_hMenuHandle);
}

// wParam - the zero - based index of the item selected
// lParam - a handle to the menu for the item selected
LRESULT KMenuCommon::ProcessCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT         tResult = 0;
    UINT            uItemID = 0xFFFFFFFFul;
    KMenuItem*      pFoundChildren = NULL;

    uItemID = LOWORD(wParam);

    pFoundChildren = this->Items.FindItemById(uItemID);

    if (pFoundChildren) {
        tResult = 1; // always return 1 if children has been found (since all windows has unique Id)
        pFoundChildren->ProcessCommandMessage(uMsg, wParam, lParam);
    }
#if 0
    uItemID = GetMenuItemID(
        (HMENU)lParam,
        (UINT)wParam
        );

    if (uItemID != -1 && uItemID != NULL) {
        KMenuItem * pFoundChildren = NULL;

        pFoundChildren = this->Items.findItemById(uItemID);

        if (pFoundChildren) {
            tResult = 1; // always return 1 if children has been found (since all windows has unique Id)
            pFoundChildren->processCommandMessage(uMsg, wParam, lParam);
        }
    }
#endif

    return tResult;
}

/* KMenuText */

KMenuText::KMenuText(KMenuItem & Parent) : m_Parent(Parent) {}

KMenuText::~KMenuText()
{
}

void KMenuText::operator=(KString NewText) {
    BOOL fResult = FALSE;
    UINT uFlags = MF_STRING | MF_BYCOMMAND;

    if (NewText.compare(TEXT("-")) == 0) {
        uFlags |= MF_SEPARATOR;
    }

    fResult = ModifyMenu(
        this->m_Parent.m_Parent.m_hMenuHandle,
        (UINT)this->m_Parent.m_ID,
        uFlags,
        (UINT_PTR)this->m_Parent.m_ID,
        NewText.c_str()
        );

    if (FALSE == fResult) {
        MSG_ERROR(TEXT("ModifyMenu failed with error = 0x%X"), GetLastError());
    }
}

/* KMenuSubMenu */

KMenuSubMenu::KMenuSubMenu(KMenuItem& Parent) : m_Parent(Parent)
{
}

KMenuSubMenu::~KMenuSubMenu()
{
}

void KMenuSubMenu::operator=(KMenuCommon* pSubMenu) {
    if (pSubMenu && this->m_Parent.m_Parent.m_ID != pSubMenu->m_ID)
    {
        BOOL            fResult = FALSE;
        MENUITEMINFO    itemInfo = { 0 };
        TCHAR*          pBuffer = NULL;

        // Retrieving menu item TEXT according to Remarks section in
        // http://msdn.microsoft.com/en-us/library/windows/desktop/ms647980%28v=vs.85%29.aspx
        itemInfo.cbSize = sizeof(MENUITEMINFO);
        itemInfo.fMask = MIIM_STRING;

        // get buffer size
        fResult = GetMenuItemInfo(
            this->m_Parent.m_Parent.m_hMenuHandle,
            (UINT)this->m_Parent.m_ID,
            FALSE,
            &itemInfo
            );

        if (itemInfo.cch) {
            itemInfo.cch++;
            pBuffer = new TCHAR[itemInfo.cch];
        }

        itemInfo.dwTypeData = pBuffer;

        if (fResult) {
            fResult = GetMenuItemInfo(
                this->m_Parent.m_Parent.m_hMenuHandle,
                (UINT)this->m_Parent.m_ID,
                FALSE,
                &itemInfo
                );

            if (fResult) {
                // modify menu item
                UINT     uFlags = MF_BYCOMMAND | MF_ENABLED;
                UINT_PTR pPopupMenu = NULL;

                if (pSubMenu) {
                    uFlags |= MF_POPUP;
                    pPopupMenu = (UINT_PTR)pSubMenu->m_hMenuHandle;
                }

                fResult = ModifyMenu(
                    this->m_Parent.m_Parent.m_hMenuHandle,
                    (UINT)this->m_Parent.m_ID,
                    uFlags,
                    pPopupMenu,
                    itemInfo.dwTypeData
                    );

                if (fResult) {
                    this->m_Parent.m_pPopupMenu = pSubMenu;

                    DrawMenuBar(this->m_Parent.m_Parent.m_hParentWindow);
                }
                else {
                    MSG_ERROR(TEXT("ModifyMenu failed with error = 0x%X"), GetLastError());
                }
            }
            else {
                MSG_ERROR(TEXT("GetMenuItemInfo failed with error = 0x%X"), GetLastError());
            }
        }
        else {
            MSG_ERROR(TEXT("GetMenuItemInfo failed with error = 0x%X"), GetLastError());
        }

        if (pBuffer) {
            delete [] pBuffer;
        }
    }
}

/* KMenuItem */

KMenuItem::KMenuItem(KMenuCommon& Parent)
    :
    Text(*this),
    SubMenu(*this),
    m_Parent(Parent),
    m_pPopupMenu(NULL),
    OnClickEvent(NULL)
{
}

KMenuItem::~KMenuItem() {
}

LRESULT KMenuItem::ProcessCommandMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT tResult = 0; // should return 1 if used by app

    MSG_LOG(TEXT("Menu Item CLICKED, Id=%d"), this->m_ID);

    // for now: only button click event
    if (NULL != OnClickEvent) {
        OnClickEvent(this, { uMsg, wParam, lParam });
    }

    return tResult;
}

LRESULT KMenuItem::ProcessNotifyMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT tResult = 0;
    NMHDR * pData = 0;

    pData = reinterpret_cast<NMHDR*>(lParam);

    //MSG_LOG(TEXT("%s - Not supported %X"), this->m_name.c_str(), pData->code);

    return tResult;
}

/* KMenuItemList */

KMenuItemList::KMenuItemList(KMenuCommon& Parent) :
    m_pParent(Parent),
    LastIndex(-1)
{
}

KMenuItemList::~KMenuItemList() {
    std::vector<KMenuItem*>::iterator i;

    i = this->m_items.begin();

    for (; i != this->m_items.end(); i++) {
        delete *i;
    }
}

KMenuItem * KMenuItemList::FindItemById(UINT_PTR SearchedId) {
    std::vector<KMenuItem*>::iterator i;
    KMenuItem * pFoundItem = NULL;

    i = this->m_items.begin();

    for (; i != m_items.end(); i++) {
        if (SearchedId == static_cast<WORD>((*i)->m_ID)) {
            pFoundItem = (*i);
            break;
        }
        else if ((*i)->m_pPopupMenu) {
            pFoundItem = (*i)->m_pPopupMenu->Items.FindItemById(SearchedId);
            if (pFoundItem) {
                break;
            }
        }
    }

    return pFoundItem;
}

// add new menu item.
// If provided name is "-", new item will be created as separator
void KMenuItemList::Add(KString NewText) {
    KMenuItem * newMenu = NULL;
        
    newMenu = new KMenuItem(m_pParent);

    if (newMenu) {
        BOOL    fResult = FALSE;
        UINT    uFlags = MF_BYCOMMAND | MF_ENABLED;
        LPCTSTR lpNewItem = NULL;

        MSG_SUCCESS(TEXT("Menu item created with id=%d"), newMenu->m_ID);
        m_items.push_back(newMenu);
        LastIndex++;

        if (NewText.compare(TEXT("-")) == 0) {
            uFlags |= MF_SEPARATOR;
        }
        else {
            uFlags |= MF_STRING;
            lpNewItem = NewText.c_str();
        }


        fResult = AppendMenu(
            this->m_pParent.m_hMenuHandle,
            uFlags,
            (UINT)newMenu->m_ID,
            lpNewItem
            );

        if (fResult) {
            // update menu in window
            DrawMenuBar(this->m_pParent.m_hParentWindow);
        }
        else {
            MSG_ERROR(TEXT("AppendMenu failed with error = 0x%X"), GetLastError());
        }
    }
}

#if 0
void KMenuItemList::Remove(uint32_t u32ItemIndex) {
    TH_ENTER_FUNCTION;
    if (u32ItemIndex < m_items.size()) {
        std::vector<KMenuItem *>::iterator i;

        if (m_items.at(u32ItemIndex)) {
            //todo: remove item from menu
            delete m_items.at(u32ItemIndex);
        }

        i = m_items.begin() + u32ItemIndex; //unsigned to iterator workaround
        m_items.erase(i);
        LastIndex--;

        fResult = RemoveMenu(
            this->m_pParent->m_pParent->m_hMenuHandle,
            (UINT)this->m_pParent->m_popupMenu->m_ID,
            MF_BYCOMMAND
            );

        if (FALSE == fResult){
            MSG_ERROR(TEXT("RemoveMenu failed with error = 0x%X"), GetLastError());
        }

        // update menu in window
        if (this->m_pParent->m_hParentWindow) {
            DrawMenuBar(this->m_pParent->m_hParentWindow);
        }
    }
    TH_LEAVE_FUNCTION;
}
#endif

uint32_t KMenuItemList::Size() {
    uint32_t u32Size = 0;

    u32Size = this->m_items.size();

    return u32Size;
}

KMenuItem * const KMenuItemList::operator[](uint32_t u32ItemIndex) {
    KMenuItem * p = 0;
    if (u32ItemIndex < m_items.size()) {
        if (m_items.at(u32ItemIndex)) {
            p = m_items.at(u32ItemIndex);
        }
    }

    return p;
}