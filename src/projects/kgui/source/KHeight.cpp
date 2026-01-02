#include "KHeight.h"

#include "KWindow.h"
#include "KForm.h"
#include "KMDIChild.h"

const int KHeight::GetValue() const
{
    RECT        rcClient = { 0 };
    int         iReturnValue = 0;
    KForm*     pForm = NULL;
    KMDIChild* pMdiChild = NULL;
    
    pForm = dynamic_cast<KForm*>(&m_Parent);
    pMdiChild = dynamic_cast<KMDIChild*>(&m_Parent);

    // KForm and MDIChild are exceptions due to the fact
    // that their border need to be taken into account
    if (pForm || pMdiChild) {
        //right and bottom members contain the width and height of the window
        GetClientRect(this->m_Parent.GetHandle(), &rcClient);

        iReturnValue = rcClient.bottom;// +rcClient.top;
    }
    else
    {
        GetWindowRect(this->m_Parent.GetHandle(), &rcClient);

        //hMenu = GetMenu(this->m_Parent->m_hWinHandle);
        //AdjustWindowRectEx(&rcClient, 0, FALSE, 0);
        iReturnValue = rcClient.bottom - rcClient.top;
    }

    return iReturnValue;
}
void KHeight::SetValue(int NewValue)
{
    RECT    rcClient = { 0 };
    DWORD   dwStyle = 0;
    DWORD   dwExStyle = 0;
    HMENU   hMenu = 0;
    BOOL    fResult = FALSE;
    KForm* pForm = 0;

    pForm = dynamic_cast<KForm*>(&m_Parent);

    dwStyle = GetWindowLongPtr(this->m_Parent.GetHandle(), GWL_STYLE);

    if (pForm) {
        hMenu = GetMenu(this->m_Parent.GetHandle());
    }

#if 0
    // dwExStyle cause wrong calculations
    dwExStyle = GetWindowLongPtr(this->m_Parent->GetHandle(), GWL_EXSTYLE);
#endif

    rcClient = { 0, 0, this->m_Parent.Width, NewValue };

    fResult = AdjustWindowRectEx(&rcClient, dwStyle, hMenu ? TRUE : FALSE, dwExStyle);

    if (FALSE == fResult) {
        MSG_ERROR(TEXT("AdjustWindowRectEx failed with error = 0x%X"), GetLastError());
    }

    if (NewValue > 0) {
        fResult = FALSE;

        fResult = SetWindowPos(
            this->m_Parent.GetHandle(),
            NULL,
            0,
            0,
            rcClient.right - rcClient.left,
            rcClient.bottom - rcClient.top,
            SWP_NOZORDER | SWP_NOMOVE);

        if (FALSE == fResult) {
            MSG_ERROR(TEXT("SetWindowPos failed with error = 0x%X"), GetLastError());
        }

        if (fResult) {
            if (NULL == pForm) {
                this->m_Parent.StoreCurrentRect();
            }
        } else {
            MSG_ERROR(TEXT("SetWindowPos failed with error = 0x%X"), GetLastError());
        }
    }
}

KHeight::KHeight(KWindow& Parent) : KParam(Parent)
{
}

KHeight::~KHeight()
{
}

KHeight & KHeight::operator=(const int& arg)
{
    KParam<int>::operator=(arg);

    return *this;
}

KHeight & KHeight::operator=(const KHeight& arg)
{
    KParam<int>::operator=(arg);

    return *this;
}

int KHeight::operator+(const int& arg)
{
    int val = KParam<int>::operator+(arg);

    return val;
}

KHeight & KHeight::operator+=(const int& arg)
{
    KParam<int>::operator+=(arg);

    return *this;
}

KHeight & KHeight::operator-=(const int& arg)
{
    KParam<int>::operator-=(arg);

    return *this;
}

KHeight::operator int() const
{
    int val = KParam<int>::operator int();

    return val;
}
