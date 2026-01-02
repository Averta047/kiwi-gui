//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KTypes.h"
#include "KConfig.h"
#include "KParam.h"

#include "KWindow.h"

/*
Generalization of win32 styles.
In this class, each style provided as TMask is of type bool, which can be read, set or clear.
Interacting is supposed to be programmer-friendly thanks to provided operator overloadings.

- TStyleType should be: GWL_EXSTYLE or GWL_STYLE
- TMask is style mask to be used
*/

template <int TStyleType, int TMask>
class KSetGetStyle : public KParam<bool>
{
protected:
private:
    const bool GetValue() const {
        bool        fResult = false;
        LONG_PTR    lpWindowStyle = 0;

        // GetWindowLongPtr return 0 on error
        lpWindowStyle = GetWindowLongPtr(this->m_Parent.GetHandle(), TStyleType);

        if (lpWindowStyle) {
            if (TMask == (lpWindowStyle & TMask)) {
                fResult = true;
            }
        }
        else {
            MSG_WARNING(TEXT("GetWindowLongPtr failed with error: 0x%X"), GetLastError());
        }

        return fResult;
    }
    void SetValue(bool fResizeable)
    {
        BOOL        fResult = FALSE;
        LONG_PTR    lpWindowStyle = 0;
        LONG_PTR    lpResult = 0;

        // GetWindowLongPtr return 0 on error
        lpWindowStyle = GetWindowLongPtr(this->m_Parent.GetHandle(), TStyleType);

        if (lpWindowStyle) {
            if (fResizeable) {
                lpWindowStyle |= TMask;
            }
            else {
                lpWindowStyle &= ~TMask;
            }

            // SetWindowLongPtr return 0 on error
            lpResult = SetWindowLongPtr(this->m_Parent.GetHandle(), TStyleType, lpWindowStyle);

            if (0 == lpResult) {
                MSG_WARNING(TEXT("SetWindowLongPtr failed with error: 0x%X"), GetLastError());
            }
        }
        else {
            MSG_WARNING(TEXT("GetWindowLongPtr failed with error: 0x%X"), GetLastError());
        }

    }
public:
    KSetGetStyle() = delete;

    KSetGetStyle(KWindow& Parent) : KParam(Parent) {}

    ~KSetGetStyle() = default;

    KSetGetStyle & operator =(const bool & arg)
    {
        KParam<bool>::operator=(arg);

        return *this;
    }
    operator bool() const
    {
        bool val = KParam<bool>::operator bool();

        return val;
    }
};
