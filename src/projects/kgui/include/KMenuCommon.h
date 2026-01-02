//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KObject.h"

class KWindow;
class KMenu;
class KMenuItem;
class KMenuCommon;

class KMenuText
{
    friend class KMenuItem;

private:
    KMenuItem&          m_Parent;

public:
                        KMenuText() = delete;
                        KMenuText(KMenuItem& Parent);
                        ~KMenuText();

    void                operator=(KString NewText);
};

class KMenuSubMenu
{
    friend class        KMenuItem;

private:
    KMenuItem&          m_Parent;
public:
                        KMenuSubMenu() = delete;
                        KMenuSubMenu(KMenuItem& Parent);
                        ~KMenuSubMenu();

    void                operator=(KMenuCommon* pSubMenu);
};

//add image 
#if 0
void bitmap(string filename)
{
    HBITMAP bitimage = (HBITMAP)LoadImage(NULL, filename.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (primeiromenu == true)
        SetMenuItemBitmaps(GetMenu(WindowMain), menuposition, MF_BYPOSITION, (HBITMAP)bitimage, (HBITMAP)bitimage);
    else
        SetMenuItemBitmaps(MenuHandle, menuposition, MF_BYPOSITION, (HBITMAP)bitimage, (HBITMAP)bitimage);
}
#endif

class KMenuItem : public KObject
{
    friend class            KMenuItemList;
    friend class            KMenuSubMenu;
    friend class            KMenuText;
    friend class            KMenuCommon;
private:
    KMenuCommon&            m_Parent;
    static int              m_IndexPool;
    KMenuCommon*            m_pPopupMenu;

    //    void GetInfo()
    //    {
    //        this->m_changedata.cbSize = sizeof(m_changedata);
    //        this->m_changedata.dwMask = TBIF_IMAGE | TBIF_TEXT | TBIF_STATE | TBIF_STYLE | TBIF_LPARAM | TBIF_COMMAND | TBIF_SIZE;
    //        SendMessage(this->parent->getHandle(), TB_GETBUTTONINFO, (WPARAM)this->id, (LPARAM)&this->m_changedata);
    //    }

    LRESULT                 ProcessCommandMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT                 ProcessNotifyMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
public:
                            KMenuItem() = delete;
                            KMenuItem(KMenuCommon& Parent);
                            ~KMenuItem();
    //bitmap
    //state
    //style
    //id <- private
    //caption
    KMenuText              Text;
    KMenuSubMenu           SubMenu;

    KEventCallbackFunc_t   OnClickEvent;
};

class KMenuItemList
{
    friend class                KMenuCommon;
private:
    KMenuCommon&                m_pParent;
    std::vector<KMenuItem*>     m_items;

    KMenuItem*                  FindItemById(UINT_PTR SearchedId);
public:
                                KMenuItemList() = delete;
                                KMenuItemList(KMenuCommon& u32ItemIndex);
                                ~KMenuItemList();

    void                        Add(KString);
 // void                        Insert(KMenuItem&);
 // void                        Remove(uint32_t);
    uint32_t                    Size();
    int                         LastIndex;

    KMenuItem * const          operator[](uint32_t);
};

class KMenuCommon : public KObject
{
    friend class        KMenuText;
    friend class        KMenuSubMenu;
    friend class        KMenuItemList;
private:
protected:
    //HMENU               m_hMenuHandle;
    HWND                m_hParentWindow;
    LRESULT             ProcessCommandMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
                        KMenuCommon();
    virtual             ~KMenuCommon();

    KMenuItemList       Items;
    HMENU               m_hMenuHandle;
};
