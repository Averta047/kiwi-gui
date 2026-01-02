//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

class KColor
{
public:
    constexpr KColor() : r(0), g(0), b(0), a(0) {}
    constexpr KColor(int _r, int _g, int _b, int _a) : r(_r), g(_g), b(_b), a(_a) {}
    constexpr KColor(const KColor& color) : r(color.r), g(color.g), b(color.b), a(color.a) {}
    constexpr KColor(unsigned int rgba_hex) :
        r((rgba_hex >> 24) & 0xFF),
        g((rgba_hex >> 16) & 0xFF),
        b((rgba_hex >> 8) & 0xFF),
        a(rgba_hex & 0xFF) 
    {
    }

public:
    int r, g, b, a;
};