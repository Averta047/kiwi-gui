//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KPosY.h"
#include "KWindow.h"
#include "KForm.h"

KPosY::KPosY(KWindow& Parent) : KParam(Parent) {}

const LONG KPosY::GetValue() const
{
    RECT rcClient = { 0 };

    GetWindowRect(this->m_Parent.GetHandle(), &rcClient);

    const HWND parent = GetParent(this->m_Parent.GetHandle());

    MapWindowPoints(HWND_DESKTOP, parent, (LPPOINT) &rcClient, 2);

    return rcClient.top;
}
void KPosY::SetValue(LONG NewY)
{
    const KForm * const pForm = dynamic_cast< KForm*>(&m_Parent);

    //GetWindowRect(m_pParent->getHandle(), &rcClient);

    BOOL result = SetWindowPos(
        this->m_Parent.GetHandle(),
        NULL,
        m_Parent.X, //x
        NewY, //y
        0, //cx - new width
        0, //cy - no change
        SWP_NOZORDER | SWP_NOSIZE
    ); // SWP_NOMOVE ignore x,y | SWP_NOSIZE ignore cx, cy

    if ( result)
    {
        if ( nullptr == pForm)
        {
            this->m_Parent.StoreCurrentRect();
        }
    }
    else
    {
        MSG_ERROR(TEXT("SetWindowPos failed with error = 0x%X"), GetLastError());
    }
}

KPosY& KPosY::operator=(const LONG& arg)
{
    KParam<LONG>::operator=(arg);

    return *this;
}

KPosY& KPosY::operator=(const KPosY& arg)
{
    KParam<LONG>::operator=(arg);

    return *this;
}

LONG KPosY::operator+(const LONG& arg)
{
    LONG val = KParam<LONG>::operator+(arg);

    return val;
}

KPosY& KPosY::operator+=(const LONG& arg)
{
    KParam<LONG>::operator+=(arg);

    return *this;
}

KPosY& KPosY::operator-=(const LONG& arg)
{
    KParam<LONG>::operator-=(arg);

    return *this;
}

KPosY::operator LONG() const
{
    LONG val = KParam<LONG>::operator LONG();

    return val;
}