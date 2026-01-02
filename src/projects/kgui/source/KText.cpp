//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KText.h"
#include "KWindow.h"

const KString KText::GetValue() const
{
    KString ReturnValue;
    TCHAR*  pBuffer = NULL;
    int     nLen = 0;

    nLen = GetWindowTextLength(this->m_Parent.GetHandle());

    if (nLen > 0) {
        pBuffer = new TCHAR[nLen + 1]; //+1 for '\0'

        if (pBuffer) {
            BOOL fResult = FALSE;

            fResult = GetWindowText(this->m_Parent.GetHandle(), pBuffer, nLen + 1);

            if (fResult) {
                ReturnValue = KString(pBuffer, nLen);
            }

            delete [] pBuffer;
        }
    }

    return ReturnValue;
}

void KText::SetValue(KString NewValue)
{
    LRESULT tResult = 0;
    DWORD   dwLastError = 0;

    SetLastError(dwLastError);

    tResult = SendMessage(
        this->m_Parent.GetHandle(),
        WM_SETTEXT,
        NULL,
        (LPARAM)(NewValue.c_str())
    );

    dwLastError = GetLastError();

    if (TRUE != tResult) {
        if (dwLastError) {
            MSG_ERROR(TEXT("SendMessage WM_SETTEXT failed with error = 0x%X, tResult = %d"), dwLastError, tResult);
        }
    }
}
KText::KText(KWindow& Parent) : KParam(Parent)
{
}
KText::~KText()
{
}

KText & KText::operator=(const KString& arg)
{
    KParam<KString>::operator=(arg);

    return *this;
}
KText & KText::operator=(const KText& arg)
{
    KParam<KString>::operator=(arg);

    return *this;
}
KString KText::operator+(const KString& arg)
{
    KString val = KParam<KString>::operator+(arg);

    return val;
}
KText & KText::operator+=(const KString& arg)
{
    KParam<KString>::operator+=(arg);

    return *this;
}
KText::operator KString () const
{
    KString val = KParam<KString>::operator KString();

    return val;
}
