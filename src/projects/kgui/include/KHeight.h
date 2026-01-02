//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KParam.h"

class KHeight : public KParam<int>
{
private:
protected:
    const int   GetValue() const;
    void        SetValue(int NewValue);
public:
    KHeight() = delete;
    KHeight(KWindow& Parent);
    ~KHeight();
    
    KHeight&    operator= (const int& arg);
    KHeight&    operator= (const KHeight& arg);
    int         operator+ (const int& arg);
    KHeight&    operator+=(const int& arg);
    KHeight&    operator-=(const int& arg);
    operator    int() const;
};

