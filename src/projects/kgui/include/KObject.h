//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KTypes.h"
#include "KConfig.h"

#include <functional>

// TODO: dont use windows specyfic types in output API!
typedef struct
{
    UINT    uMsg;
    WPARAM  wParam;
    LPARAM  lParam;
} KEventParams_t;

class KObject;

typedef std::function<LRESULT(KObject* const sender, KEventParams_t info)> KEventCallbackFunc_t;

class KObject
{
protected:
    DWORD   m_ID;   // windows limit this value to WORD size, but DWORD is used for easy limit check
    KString m_Name; // set in c-tor of last class in derivating chain

private:
    static DWORD    m_IdPool;

public:
    KObject();
    virtual ~KObject();
};

