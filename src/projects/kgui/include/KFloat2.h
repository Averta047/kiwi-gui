//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

class KFloat2
{
public:
    constexpr KFloat2() : x(0.0f), y(0.0f) {}
    constexpr KFloat2(float _x, float _y) : x(_x), y(_y) {}

public:
    // Operations

public:
    float x, y;
};