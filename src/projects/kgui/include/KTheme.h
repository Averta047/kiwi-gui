//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KTypes.h"
#include "KConfig.h"

enum KColors_
{
    kCol_Text,
    kCol_TextDisabled,
    kCol_WindowBg,

    kCol_Border,
    kCol_Separator,

    kCol_FrameBg,
    kCol_FrameBgHovered,
    kCol_FrameBgActive,
    kCol_FrameBgDisabled,

    kCol_TitleBg,
    kCol_TitleBgActive,

    kCol_MenuBarBg,

    kCol_ScrollBarBg,
    kCol_ScrollBarGrab,
    kCol_ScrollBarGrabHovered,
    kCol_ScrollBarGrabActive,

    kCol_COUNT
};

struct KStyle
{
public:
    int         Alpha;

    KFloat2     WindowPadding;
    float       WindowRounding;
    float       WindowBorderSize;
    KFloat2     WindowTitleAlign;

    KFloat2     FramePadding;
    float       FrameRounding;
    float       FrameBorderSize;
    KFloat2     FrameTextAlign;

    KFloat2     ItemInnerSpacing;

    KFloat2     CheckBoxPadding;
    KFloat2     CheckMarkPadding;

    bool        AntiAliasing;

    KColor      Colors[kCol_COUNT];

public:
    KStyle()
    {
        ApplyDefaultStyles();
    }

public:
    void ApplyDefaultStyles()
    {
        Alpha               = 255;

        WindowPadding       = KFloat2(16.0f, 16.0f);
        WindowRounding      = 11.0f;
        WindowBorderSize    = 1.0f;
        WindowTitleAlign    = KFloat2(0.0f, 0.5f);

        FramePadding        = KFloat2(10.0f, 10.0f);
        FrameRounding       = 6.0f;
        FrameBorderSize     = 1.0f;
        FrameTextAlign      = KFloat2(0.0f, 0.5f);

        ItemInnerSpacing    = KFloat2(10.0f, 7.0f);

        CheckBoxPadding     = KFloat2(2.0f, 2.0f);
        CheckMarkPadding    = KFloat2(3.0f, 3.0f);

        AntiAliasing        = true;
    }
};

struct KWindow;

enum KThemes_ : int
{
    kTheme_Dark = 0,
    kTheme_Light,

    kThemes_COUNT
};

class KTheme
{
protected:
    KStyle      m_Style;
    KThemes_    m_CurrentTheme;

private:
    KWindow&    m_Parent;

    void        CallRepaint();
    void        SetDarkTheme();
    void        SetLightTheme();

public:
    KTheme() = delete;
    KTheme(KWindow& Parent);
    virtual     ~KTheme();

    void        SetTheme(KThemes_ theme = kTheme_Dark);
    KThemes_    GetCurrentTheme();

public:
    void        SetStyle(KStyle& style);
    KStyle      GetStyle() const;

public:
    KTheme& operator =(const KTheme& arg);
};

