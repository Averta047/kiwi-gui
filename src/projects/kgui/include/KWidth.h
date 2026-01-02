//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KParam.h"

class KWidth : public KParam<int>
{
private:
protected:
    const int   GetValue() const;
    void        SetValue(int NewValue);
public:
    KWidth() = delete;
    KWidth(KWindow & a_pParent);
    ~KWidth();

    KWidth&     operator= (const int& arg);
    KWidth&     operator= (const KWidth& arg);
    int         operator+ (const int& arg);
    KWidth&     operator+=(const int& arg);
    KWidth&     operator-=(const int& arg);
    operator    int() const;
};

