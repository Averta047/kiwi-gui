//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KTypes.h"
#include "KFloat2.h"

enum kRoundFlags_
{
    kRoundFlags_None = 0,
    kRoundFlags_RoundCornersTopLeft = 1 << 4, 
    kRoundFlags_RoundCornersTopRight = 1 << 5,
    kRoundFlags_RoundCornersBottomLeft = 1 << 6,
    kRoundFlags_RoundCornersBottomRight = 1 << 7,
    kRoundFlags_RoundCornersNone = 1 << 8,
    kRoundFlags_RoundCornersTop = kRoundFlags_RoundCornersTopLeft | kRoundFlags_RoundCornersTopRight,
    kRoundFlags_RoundCornersBottom = kRoundFlags_RoundCornersBottomLeft | kRoundFlags_RoundCornersBottomRight,
    kRoundFlags_RoundCornersLeft = kRoundFlags_RoundCornersBottomLeft | kRoundFlags_RoundCornersTopLeft,
    kRoundFlags_RoundCornersRight = kRoundFlags_RoundCornersBottomRight | kRoundFlags_RoundCornersTopRight,
    kRoundFlags_RoundCornersAll = kRoundFlags_RoundCornersTopLeft | kRoundFlags_RoundCornersTopRight | kRoundFlags_RoundCornersBottomLeft | kRoundFlags_RoundCornersBottomRight
};

class KFont;
class KWindow;

class KDraw
{
public:
    KDraw(KWindow& window);
    ~KDraw();

    // Shape drawing
    void AddRect(const KFloat2& start, const KFloat2& end, const KColor& color, float rounding = 0.0f, float thickness = 1.0f, kRoundFlags_ flags = kRoundFlags_RoundCornersAll);
    void AddRectFilled(const KFloat2& start, const KFloat2& end, const KColor& color, float rounding = 0.0f, kRoundFlags_ flags = kRoundFlags_RoundCornersAll);

    void AddCircle(const KFloat2& center, float radius, const KColor& color, float thickness = 1.0f);
    void AddCircleFilled(const KFloat2& center, float radius, const KColor& color);

    void AddLine(const KFloat2& start, const KFloat2& end, const KColor& color, float thickness = 1.0f);

    void AddTriangle(const KFloat2& p1, const KFloat2& p2, const KFloat2& p3,
        const KColor& color, float thickness = 1.0f);

    void AddTriangleFilled(const KFloat2& p1, const KFloat2& p2, const KFloat2& p3,
        const KColor& color);

    // Text drawing
#if 0
    void AddText(const KFont& font, const KFloat2& pos, const KColor& color, const KString& text);
    void AddTextGradient(const KFont& font, const KFloat2& pos, const KFloat2& size,
        const KColor& color1, const KColor& color2,
        const KString& text, bool horizontal = true);
#endif 

private:
    struct Impl;
    Impl* m_pImpl;
};