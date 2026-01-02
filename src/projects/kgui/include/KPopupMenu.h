//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KWindow.h"
#include "KMenuCommon.h"

class KWindow;

class KPopupMenu : public KMenuCommon
{
    friend class KWindow;
    friend class KForm;
private:
    /* 
        index used to numerate this type of Window - it only
        goes uppward, even if some elements are deleted 
    */
    static uint32_t     u32Index;

    void                Show(HWND Parent, POINT& Point);
protected:
public:
                        KPopupMenu();
                        ~KPopupMenu();

    void                Show(KWindow* Parent, POINT& Point);
};

