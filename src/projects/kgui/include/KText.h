//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KParam.h"

class KText : public KParam<KString>
{
private:
protected:
    const KString   GetValue() const;
    void            SetValue(KString NewValue);
public:
    KText() = delete;
    KText(KWindow & a_pParent);
    ~KText();

    KText&      operator =  (const KString& arg);
    KText&      operator =  (const KText& arg);
    KString     operator +  (const KString& arg);
    KText&      operator += (const KString& arg);
    operator    KString () const;
};

