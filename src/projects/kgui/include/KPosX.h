//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KParam.h"

class KPosX : public KParam<LONG>
{
protected:
private:
    const LONG  GetValue() const;
    void        SetValue(LONG NewX);

public:
    KPosX() = delete;
    KPosX(KWindow& Parent);
    ~KPosX() = default;

    KPosX&      operator =(const LONG& arg);
    KPosX&      operator =(const KPosX& arg);
    LONG        operator +(const LONG& arg);
    KPosX&      operator+=(const LONG& arg);
    KPosX&      operator-=(const LONG& arg);
    operator    LONG() const;
};

