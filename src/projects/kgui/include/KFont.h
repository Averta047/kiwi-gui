//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KTypes.h"
#include "KConfig.h"

class KWindow;

class KFont
{
protected:
private:
    KWindow&    m_Parent;
    LOGFONT     m_LogicalFont   {0};
    HFONT       m_pFont         {NULL};

    void        Set();
public:
    KFont() = delete;
    KFont(KWindow& Parent);
    virtual ~KFont();

// public interface
    void    SetName(const KString& FontName);
    void    SetSize(uint32_t NewSize);

    HFONT   GetHandle();
};

