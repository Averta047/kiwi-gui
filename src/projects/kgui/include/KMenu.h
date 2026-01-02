//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KMenuCommon.h"

class KMenu : public KMenuCommon
{
    friend class        KMenuItem;
    friend class        KForm;

private:
    /* 
        index used to numerate this type of Window - it only
        goes uppward, even if some elements are deleted 
    */
    static uint32_t     u32Index;

protected:
public:
    KMenu();
    ~KMenu();
};
