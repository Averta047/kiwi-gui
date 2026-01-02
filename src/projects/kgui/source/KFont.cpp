//========= CopKThemeyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KFont.h"
#include "KWindow.h"

KFont::KFont(KWindow& Parent) : m_Parent(Parent) {}

KFont::~KFont()
{
    if (m_pFont)
    {
        BOOL fResult = DeleteObject(m_pFont);

        if (fResult != TRUE)
        {
            MSG_ERROR(TEXT("DeleteObject failed with error = 0x%X"), GetLastError());
        }
    }
}

void KFont::Set()
{
    // delete current font (if exist)
    if (m_pFont)
    {
        BOOL fResult = DeleteObject(m_pFont);

        if (TRUE != fResult)
        {
            MSG_ERROR(TEXT("DeleteObject failed with error = 0x%X"), GetLastError());
        }
        else
        {
            m_pFont = NULL;
        }
    }

    if (NULL == m_pFont)
    {
        // Create Default Font
        m_pFont = CreateFontIndirect(&this->m_LogicalFont);

        if (m_pFont)
        {
            // set new font
            SendMessage(m_Parent.GetHandle(), WM_SETFONT, reinterpret_cast<WPARAM>(m_pFont), static_cast<LPARAM>(TRUE));
        }
        else
        {
            MSG_ERROR(TEXT("CreateFontIndirect failed with error = 0x%X"), GetLastError());
        }
    }
    else
    {
        MSG_ERROR(TEXT("Failed to set new font!"));
    }
}

void KFont::SetName(const KString& FontName)
{
    SecureZeroMemory( this->m_LogicalFont.lfFaceName, LF_FACESIZE);
    _tcscpy_s( this->m_LogicalFont.lfFaceName, LF_FACESIZE, FontName.c_str());

    this->Set();
}

// set size in point
void KFont::SetSize(uint32_t NewSize)
{
    HDC hDevContext = GetDC(NULL);

    // Calcuate font size to point format
    this->m_LogicalFont.lfHeight = -MulDiv(NewSize, GetDeviceCaps(hDevContext, LOGPIXELSY), 72);
    // If you want constant font size in pixels for all DPI then you should use this:
    // nHeight = MulDiv(PointSize, 96, GetDeviceCaps(hDC, LOGPIXELSY));
    // 96 is current dpi
    ReleaseDC(NULL, hDevContext);

    this->Set();
}

HFONT KFont::GetHandle()
{
    return m_pFont;
}