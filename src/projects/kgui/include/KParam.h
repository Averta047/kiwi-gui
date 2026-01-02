//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
// 
// Thx : http://stackoverflow.com/questions/4122214/why-operator-doesnt-get-inherited-from-a-template-class
//
//================================================================//

#pragma once

#include "KTypes.h"
#include "KConfig.h"

class KWindow;

template <typename T>
class KParam
{
    // Share SetParent
    friend class    KWindow;
    friend class    KForm;
    friend class    KButton;

protected:
    KWindow&       m_Parent;

    inline virtual const T GetValue(void) const = 0;
    inline virtual void    SetValue(T) = 0;

public:
    KParam()                    = delete;
    KParam(KWindow& Parent)    : m_Parent(Parent) { }
    virtual ~KParam()           = default;

    inline KParam & operator=(const T& arg)
    {
        SetValue(arg);

        return *this;
    }

    inline KParam & operator=(const KParam & arg)
    {
        T NewValue = arg.GetValue();
        SetValue(NewValue);
 
        return *this;
    }
 
    inline T operator+(const T & arg)
    {
        T val = GetValue() + arg;

        return  val;
    }

    inline KParam & operator+=(const T & arg)
    {
        T NewValue = GetValue() + arg;
        SetValue(NewValue);
 
        return *this;
    }
 
    inline KParam & operator-=(const T & arg)
    {
        T NewValue = GetValue() - arg;
        SetValue(NewValue);
 
        return *this;
    }
 
    inline operator T() const
    {
        T returnValue = GetValue();

        return returnValue;
    }
};
