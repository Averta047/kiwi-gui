//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KPosX.h"
#include "KWindow.h"
#include "KForm.h"

KPosX::KPosX(KWindow & Parent) : KParam(Parent) {}

const LONG KPosX::GetValue() const
{
    RECT rcClient = { 0 };

    GetWindowRect(this->m_Parent.GetHandle(), &rcClient);
    MapWindowPoints(HWND_DESKTOP, GetParent(this->m_Parent.GetHandle()), (LPPOINT)&rcClient, 2);

    return rcClient.left;
}

void KPosX::SetValue(LONG NewX)
{
    BOOL        fResult = FALSE;
    KForm *    pForm = NULL;

    pForm = dynamic_cast<KForm*>(&m_Parent);

    fResult = SetWindowPos(this->m_Parent.GetHandle(), NULL,
        NewX,           // x
        m_Parent.Y,     // y
        0,              // cx - ignored with SWP_NOSIZE
        0,              // cy - ignored with SWP_NOSIZE
        SWP_NOZORDER | SWP_NOSIZE); // SWP_NOMOVE ignore x,y | SWP_NOSIZE ignore cx, cy

    if (fResult) {
        if (NULL == pForm) {
            this->m_Parent.StoreCurrentRect(); // store new RECT
        }
    }
    else {
        MSG_ERROR(TEXT("SetWindowPos failed with error = 0x%X"), GetLastError());
    }
}

KPosX& KPosX::operator=(const LONG& arg)
{
    KParam<LONG>::operator=(arg);

    return *this;
}

KPosX& KPosX::operator=(const KPosX& arg)
{
    KParam<LONG>::operator=(arg);

    return *this;
}

LONG KPosX::operator+(const LONG& arg)
{
    LONG val = KParam<LONG>::operator+(arg);

    return val;
}

KPosX& KPosX::operator+=(const LONG& arg)
{
    KParam<LONG>::operator+=(arg);

    return *this;
}

KPosX& KPosX::operator-=(const LONG& arg)
{
    KParam<LONG>::operator-=(arg);

    return *this;
}

KPosX::operator LONG() const
{
    LONG val = KParam<LONG>::operator LONG();

    return val;
}