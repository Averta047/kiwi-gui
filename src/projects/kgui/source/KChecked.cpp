//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KChecked.h"
#include "KWindow.h"
#include "KCheckBox.h"

KChecked::KChecked(KWindow& Parent) : KParam(Parent) {}

const bool KChecked::GetValue() const
{
    if (!m_Parent.GetHandle())
        return false;

    return BST_CHECKED == SendMessage(m_Parent.GetHandle(), BM_GETCHECK, 0, 0);
}

void KChecked::SetValue(bool bChecked)
{
    if (!m_Parent.GetHandle())
        return;

    SendMessage(m_Parent.GetHandle(), BM_SETCHECK, bChecked ? BST_CHECKED : BST_UNCHECKED, 0);
}

KChecked& KChecked::operator=(const bool& arg)
{
    KParam<bool>::operator=(arg);

    return *this;
}

KChecked& KChecked::operator=(const KChecked& arg)
{
    KParam<bool>::operator=(arg);

    return *this;
}

KChecked::operator bool() const
{
    bool val = KParam<bool>::operator bool();

    return val;
}