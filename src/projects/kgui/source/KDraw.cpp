//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "KDraw.h"
#include "KColor.h"
#include "KFont.h"
#include "KWindow.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

struct KDraw::Impl
{
    KWindow& m_Parent;

    HDC m_hDC;
    HDC m_hMemDC;
    HBITMAP m_hMemBitmap;
    HBITMAP m_hOldBitmap;
    RECT m_ClientRect;
    Gdiplus::Graphics* m_pGraphics;
    ULONG_PTR m_GDI_PlusToken;

    Impl(KWindow& window);
    ~Impl();

    void CreateRoundedRectPath(Gdiplus::GraphicsPath& path, const KFloat2& start, const KFloat2& end, float rounding, kRoundFlags_ flags);
};

KDraw::Impl::Impl(KWindow& Parent) :
    m_Parent(Parent),
    m_hDC(nullptr),
    m_hMemDC(nullptr),
    m_hMemBitmap(nullptr),
    m_hOldBitmap(nullptr),
    m_pGraphics(nullptr)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_GDI_PlusToken, &gdiplusStartupInput, NULL);

    // Get window DC
    m_hDC = GetDC(m_Parent.GetHandle());
    GetClientRect(m_Parent.GetHandle(), &m_ClientRect);

    // Create memory DC for double buffering
    m_hMemDC = CreateCompatibleDC(m_hDC);
    m_hMemBitmap = CreateCompatibleBitmap(m_hDC, m_ClientRect.right, m_ClientRect.bottom);
    m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hMemBitmap);

    // Create graphics object
    m_pGraphics = new Gdiplus::Graphics(m_hMemDC);

    // Important !
    m_pGraphics->SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);                  // Sharp edges
    m_pGraphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);                 // Smooth curves
    m_pGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);  // ImGui-style text
    m_pGraphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQuality);       // Clean gradients

    // Transparent background
    m_pGraphics->Clear(Gdiplus::Color(0, 0, 0, 0)); 

    /*
    // Clear background
    HBRUSH hbrBkgnd = CreateSolidBrush(RGB(255, 0, 0));
    FillRect(m_hMemDC, &m_ClientRect, hbrBkgnd);
    DeleteObject(hbrBkgnd);
    */
}

KDraw::Impl::~Impl()
{
    if (m_pGraphics)
    {
        delete m_pGraphics;
        m_pGraphics = nullptr;
    }

    if (m_hMemDC) 
    {
        // Copy to screen
        BitBlt(m_hDC, 0, 0, m_ClientRect.right, m_ClientRect.bottom, m_hMemDC, 0, 0, SRCCOPY);

        // Clean up
        SelectObject(m_hMemDC, m_hOldBitmap);
        DeleteObject(m_hMemBitmap);
        DeleteDC(m_hMemDC);
    }

    if (m_hDC) 
    {
        ReleaseDC(m_Parent.GetHandle(), m_hDC);
    }

    // Shutdown GDI+
    Gdiplus::GdiplusShutdown(m_GDI_PlusToken);
}

// Public class implementation
KDraw::KDraw(KWindow& window) :
    m_pImpl(new Impl(window))
{
}

KDraw::~KDraw()
{
    delete m_pImpl;
}

void KDraw::AddRect(const KFloat2& start, const KFloat2& end, const KColor& color, float rounding, float thickness, kRoundFlags_ flags) 
{
    if (!m_pImpl->m_pGraphics || color.a == 0) return;

    // ImGui-style coordinate adjustments
    Gdiplus::REAL offset = 0.5f;
    Gdiplus::RectF rect(
        std::floor(start.x) + offset,
        std::floor(start.y) + offset,
        std::floor(end.x - start.x) - offset * 2.0f,
        std::floor(end.y - start.y) - offset * 2.0f
    );

    Gdiplus::Pen pen(Gdiplus::Color(color.a, color.r, color.g, color.b), thickness);

    if (rounding < 0.5f || flags == kRoundFlags_RoundCornersNone) {
        m_pImpl->m_pGraphics->DrawRectangle(&pen, rect);
    }
    else {
        Gdiplus::GraphicsPath path;
        m_pImpl->CreateRoundedRectPath(path,
            { rect.X, rect.Y },
            { rect.X + rect.Width, rect.Y + rect.Height },
            rounding, flags);
        m_pImpl->m_pGraphics->DrawPath(&pen, &path);
    }
}

void KDraw::AddRectFilled(const KFloat2& start, const KFloat2& end, const KColor& color, float rounding, kRoundFlags_ flags) {
    if (!m_pImpl->m_pGraphics) return;

    Gdiplus::SolidBrush brush(Gdiplus::Color(color.a, color.r, color.g, color.b));

    // Adjust for pixel-perfect filling (ImGui doesn't blur filled shapes)
    Gdiplus::REAL x = std::floor(start.x);
    Gdiplus::REAL y = std::floor(start.y);
    Gdiplus::REAL width = std::floor(end.x - start.x);
    Gdiplus::REAL height = std::floor(end.y - start.y);

    if (rounding <= 0.0f || flags == kRoundFlags_RoundCornersNone) {
        m_pImpl->m_pGraphics->FillRectangle(&brush, x, y, width, height);
    }
    else {
        Gdiplus::GraphicsPath path;
        KFloat2 adjStart(x, y);
        KFloat2 adjEnd(x + width, y + height);
        m_pImpl->CreateRoundedRectPath(path, adjStart, adjEnd, rounding, flags);
        m_pImpl->m_pGraphics->FillPath(&brush, &path);
    }
}

void KDraw::AddCircle(const KFloat2& center, float radius, const KColor& color, float thickness)
{
    if (!m_pImpl->m_pGraphics) return;

    Gdiplus::Pen pen(Gdiplus::Color(color.a, color.r, color.g, color.b), thickness);
    m_pImpl->m_pGraphics->DrawEllipse(&pen, center.x - radius, center.y - radius, radius * 2, radius * 2);
}

void KDraw::AddCircleFilled(const KFloat2& center, float radius, const KColor& color)
{
    if (!m_pImpl->m_pGraphics) return;

    Gdiplus::SolidBrush brush(Gdiplus::Color(color.a, color.r, color.g, color.b));
    m_pImpl->m_pGraphics->FillEllipse(&brush, center.x - radius, center.y - radius, radius * 2, radius * 2);
}

void KDraw::AddLine(const KFloat2& start, const KFloat2& end, const KColor& color, float thickness)
{
    if (!m_pImpl->m_pGraphics) return;

    Gdiplus::Pen pen(Gdiplus::Color(color.a, color.r, color.g, color.b), thickness);
    m_pImpl->m_pGraphics->DrawLine(&pen, start.x, start.y, end.x, end.y);
}

void KDraw::AddTriangle(const KFloat2& p1, const KFloat2& p2, const KFloat2& p3, const KColor& color, float thickness)
{
    if (!m_pImpl->m_pGraphics) return;

    Gdiplus::Pen pen(Gdiplus::Color(color.a, color.r, color.g, color.b), thickness);
    Gdiplus::PointF points[3] = { Gdiplus::PointF(p1.x, p1.y), Gdiplus::PointF(p2.x, p2.y), Gdiplus::PointF(p3.x, p3.y) };
    m_pImpl->m_pGraphics->DrawPolygon(&pen, points, 3);
}

void KDraw::AddTriangleFilled(const KFloat2& p1, const KFloat2& p2, const KFloat2& p3, const KColor& color)
{
    if (!m_pImpl->m_pGraphics) return;

    Gdiplus::SolidBrush brush(Gdiplus::Color(color.a, color.r, color.g, color.b));
    Gdiplus::PointF points[3] = { Gdiplus::PointF(p1.x, p1.y), Gdiplus::PointF(p2.x, p2.y), Gdiplus::PointF(p3.x, p3.y) };
    m_pImpl->m_pGraphics->FillPolygon(&brush, points, 3);
}

void KDraw::Impl::CreateRoundedRectPath(Gdiplus::GraphicsPath& path, const KFloat2& start, const KFloat2& end, float rounding, kRoundFlags_ flags)
{
    float diameter = rounding * 2.0f;
    float width = end.x - start.x;
    float height = end.y - start.y;

    // Top-left
    if (flags & kRoundFlags_RoundCornersTopLeft) {
        path.AddArc(start.x, start.y, diameter, diameter, 180, 90);
    }
    else {
        path.AddLine(start.x, start.y + rounding, start.x, start.y);
        path.AddLine(start.x, start.y, start.x + rounding, start.y);
    }

    // Top-right
    if (flags & kRoundFlags_RoundCornersTopRight) {
        path.AddArc(start.x + width - diameter, start.y, diameter, diameter, 270, 90);
    }
    else {
        path.AddLine(start.x + width - rounding, start.y, start.x + width, start.y);
        path.AddLine(start.x + width, start.y, start.x + width, start.y + rounding);
    }

    // Bottom-right
    if (flags & kRoundFlags_RoundCornersBottomRight) {
        path.AddArc(start.x + width - diameter, start.y + height - diameter,
            diameter, diameter, 0, 90);
    }
    else {
        path.AddLine(start.x + width, start.y + height - rounding,
            start.x + width, start.y + height);
        path.AddLine(start.x + width, start.y + height,
            start.x + width - rounding, start.y + height);
    }

    // Bottom-left
    if (flags & kRoundFlags_RoundCornersBottomLeft) {
        path.AddArc(start.x, start.y + height - diameter, diameter, diameter, 90, 90);
    }
    else {
        path.AddLine(start.x + rounding, start.y + height, start.x, start.y + height);
        path.AddLine(start.x, start.y + height, start.x, start.y + height - rounding);
    }

    path.CloseFigure();
}
