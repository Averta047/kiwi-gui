//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KParam.h"

class KChecked : public KParam<bool>
{
protected:
private:
    const bool  GetValue() const;
    void        SetValue(bool bChecked);

public:
    KChecked() = delete;
    KChecked(KWindow& Parent);
    ~KChecked() = default;

    KChecked& operator=(const bool& arg);
    KChecked& operator=(const KChecked& arg);
    operator bool() const;
};