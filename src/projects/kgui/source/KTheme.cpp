//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KTheme.h"
#include "KWindow.h"

KTheme::KTheme(KWindow& Parent) : m_Parent(Parent) { m_CurrentTheme = kTheme_Dark; }
KTheme::~KTheme() {}

void KTheme::SetDarkTheme()
{
    m_CurrentTheme                              = kTheme_Dark;

	m_Style.Colors[kCol_Text]                   = KColor(0xFFFFFFFF);
    m_Style.Colors[kCol_TextDisabled]           = KColor(0x878787FF);
    m_Style.Colors[kCol_WindowBg]               = KColor(0x151515FF);

    m_Style.Colors[kCol_Border]                 = KColor(0x363636FF);
    m_Style.Colors[kCol_Separator]              = KColor(0x333333FF);

    m_Style.Colors[kCol_FrameBg]                = KColor(0x2F2F2FFF);
    m_Style.Colors[kCol_FrameBgHovered]         = KColor(0x343434FF);
    m_Style.Colors[kCol_FrameBgActive]          = KColor(0x2B2B2BFF);
    m_Style.Colors[kCol_FrameBgDisabled]        = KColor(0x1F1F1FFF);

    m_Style.Colors[kCol_TitleBg]                = KColor(0x191919FF);
    m_Style.Colors[kCol_TitleBgActive]          = KColor(0x151515FF);

    m_Style.Colors[kCol_MenuBarBg]              = KColor(0x151515FF);

    m_Style.Colors[kCol_ScrollBarBg]            = KColor(0x1A1A1AFF);
    m_Style.Colors[kCol_ScrollBarGrab]          = KColor(0x575757FF);
    m_Style.Colors[kCol_ScrollBarGrabHovered]   = KColor(0x5C5C5CFF);
    m_Style.Colors[kCol_ScrollBarGrabActive]    = KColor(0x515151FF);
}

void KTheme::SetLightTheme()
{
    m_CurrentTheme = kTheme_Light;

	m_Style.Colors[kCol_Text]                   = KColor(0x0D0D0DFF);
    m_Style.Colors[kCol_TextDisabled]           = KColor(0xA6A6A6FF);
    m_Style.Colors[kCol_WindowBg]               = KColor(0xEDEDEDFF);

    m_Style.Colors[kCol_Border]                 = KColor(0x7D7D7DFF);
    m_Style.Colors[kCol_Separator]              = KColor(0x959595FF);

    m_Style.Colors[kCol_FrameBg]                = KColor(0xA7A7A7FF);
    m_Style.Colors[kCol_FrameBgHovered]         = KColor(0xB3B3B3FF);
    m_Style.Colors[kCol_FrameBgActive]          = KColor(0xA4A4A4FF);
    m_Style.Colors[kCol_FrameBgDisabled]        = KColor(0xA1A1A1FF);

    m_Style.Colors[kCol_TitleBg]                = KColor(0xE2E2E2FF);
    m_Style.Colors[kCol_TitleBgActive]          = KColor(0xD1D1D1FF);

    m_Style.Colors[kCol_MenuBarBg]              = KColor(0xEDEDEDFF);

    m_Style.Colors[kCol_ScrollBarBg]            = KColor(0xBCBCBCFF);
    m_Style.Colors[kCol_ScrollBarGrab]          = KColor(0xEDEDEDFF);
    m_Style.Colors[kCol_ScrollBarGrabHovered]   = KColor(0xF7F7F7FF);
    m_Style.Colors[kCol_ScrollBarGrabActive]    = KColor(0xDBDBDBFF);
}

void KTheme::SetTheme(KThemes_ theme)
{
    switch (theme)
    {
    case kTheme_Dark:
        SetDarkTheme();
        break;
    case kTheme_Light:
        SetLightTheme();
        break;
    default:
        SetDarkTheme();
        break;
    }
    
    this->CallRepaint();
}

KThemes_ KTheme::GetCurrentTheme()
{
    return m_CurrentTheme;
}

void KTheme::SetStyle(KStyle& style)
{
    m_Style = style;
}
KStyle KTheme::GetStyle() const
{
    return m_Style;
}

KTheme& KTheme::operator =(const KTheme& arg)
{
    *this = arg;
}

// TODO : do not use windows related functions in here, instead make a setters in the KWindow class, make sure we don't apply dark titlebar for widgets
void KTheme::CallRepaint()
{
    RedrawWindow(m_Parent.GetHandle(), NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);

    BOOL use_dark = TRUE;
    if (m_CurrentTheme == kTheme_Light)
        use_dark = FALSE;

    ::DwmSetWindowAttribute(m_Parent.GetHandle(), DWMWA_USE_IMMERSIVE_DARK_MODE, &use_dark, sizeof(use_dark));
}