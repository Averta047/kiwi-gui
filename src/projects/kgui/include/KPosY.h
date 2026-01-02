//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KParam.h"

class KPosY : public KParam<LONG>
{
private:
    const LONG  GetValue() const;
    void        SetValue(LONG NewY);

public:
    KPosY() = delete;
    KPosY(KWindow & Parent);
                ~KPosY() = default;

    KPosY&      operator =(const LONG& arg);
    KPosY&      operator =(const KPosY& arg);
    LONG        operator +(const LONG& arg);
    KPosY&      operator+=(const LONG& arg);
    KPosY&      operator-=(const LONG& arg);
    operator    LONG() const;
};