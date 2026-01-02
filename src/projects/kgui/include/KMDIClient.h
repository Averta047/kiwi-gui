//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KWindow.h"

class KForm;

class KMDIClient : public KWindow
{
    friend LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //friend LRESULT CALLBACK MDIClientWindProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);
protected:
    virtual int         GetDebugIndex();
private:
    static int          m_IndexPool;

public:
                        KMDIClient() = delete;
                        KMDIClient(KForm* pParent, int PosX = CW_USEDEFAULT, int PosY = CW_USEDEFAULT);
                        ~KMDIClient();

    void                Cascade();              // arrange childen in cascade format
    void                TileHorizontal();       // arrange childen in horizontal tile format
    void                TileVertical();         // arrange childen in vertical tile format
    void                ArrangeIcons();         // arrange minimazed icons
};

