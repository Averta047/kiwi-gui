//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#pragma once

#include "KSetGetStyle.h"

/*
        KForm window styles
*/

/*
 KResizable can be used to check if Window is resizable and to turn on/off resizablity.

 Example:
 if (KForm->Resizable) {}
 KForm->Resizable = true; KForm->Resizable = false;
*/

class KResizable : public KSetGetStyle < GWL_STYLE, WS_THICKFRAME >
{
public:
    KResizable() = delete;

    KResizable(KWindow& Parent) : KSetGetStyle(Parent)
    {}

    ~KResizable() {}
    KResizable & operator =(const bool & arg)
    {
        KSetGetStyle < GWL_STYLE, WS_THICKFRAME >::operator=(arg);

        return *this;
    }
    operator bool() const
    {
        bool fResult = KSetGetStyle < GWL_STYLE, WS_THICKFRAME >::operator bool();

        return fResult;
    }
};

/*
        KButton window styles (BS prefix - Button Style)
*/

// Change push button style to DefaultPushButton
class KBS_DefaultPush : public KSetGetStyle < GWL_STYLE, BS_DEFPUSHBUTTON >
{
public:
    KBS_DefaultPush() = delete;

    KBS_DefaultPush(KWindow& Parent) :
        KSetGetStyle(Parent)
    {}

    ~KBS_DefaultPush() {}
    KBS_DefaultPush & operator =(const bool & arg)
    {
        KSetGetStyle < GWL_STYLE, BS_DEFPUSHBUTTON >::operator=(arg);

        return *this;
    }
    operator bool() const
    {
        bool fResult = KSetGetStyle < GWL_STYLE, BS_DEFPUSHBUTTON >::operator bool();

        return fResult;
    }
};

#if 0
// Places text at the bottom of the button rectangle.
class KBS_Bottom : public KSetGetStyle < GWL_STYLE, BS_BOTTOM >
{
public:
    KBS_Bottom() {}
    ~KBS_Bottom() {}
    KBS_Bottom& operator =(const bool & a_arg)
    {
        TH_ENTER_FUNCTION;
        KSetGetStyle < GWL_STYLE, BS_BOTTOM >::operator=(a_arg);
        TH_LEAVE_FUNCTION;
        return *this;
    }
    operator bool() const
    {
        bool fResult = KSetGetStyle < GWL_STYLE, BS_BOTTOM >::operator bool();
        TH_ENTER_FUNCTION;
        TH_LEAVE_FUNCTION;
        return fResult;
    }
};

// Centers text horizontally in the button rectangle.
class KBS_Center : public KSetGetStyle < GWL_STYLE, BS_CENTER >
{
public:
    KBS_Center() {}
    ~KBS_Center() {}
    KBS_Center& operator =(const bool & a_arg)
    {
        TH_ENTER_FUNCTION;
        KSetGetStyle < GWL_STYLE, BS_CENTER >::operator=(a_arg);
        TH_LEAVE_FUNCTION;
        return *this;
    }
    operator bool() const
    {
        bool fResult = KSetGetStyle < GWL_STYLE, BS_CENTER >::operator bool();
        TH_ENTER_FUNCTION;
        TH_LEAVE_FUNCTION;
        return fResult;
    }
};

// Specifies that the button is two-dimensional; it does not use the default shading to create a 3-D image. 
class KBS_Flat : public KSetGetStyle < GWL_STYLE, BS_FLAT >
{
public:
    KBS_Flat() {}
    ~KBS_Flat() {}
    KBS_Flat& operator =(const bool & a_arg)
    {
        TH_ENTER_FUNCTION;
        KSetGetStyle < GWL_STYLE, BS_FLAT >::operator=(a_arg);
        TH_LEAVE_FUNCTION;
        return *this;
    }
    operator bool() const
    {
        bool fResult = KSetGetStyle < GWL_STYLE, BS_FLAT >::operator bool();
        TH_ENTER_FUNCTION;
        TH_LEAVE_FUNCTION;
        return fResult;
    }
};

// Wraps the button text to multiple lines if the text string is too long to fit on a single line in the button rectangle.
class KBS_Multiline : public KSetGetStyle < GWL_STYLE, BS_MULTILINE >
{
public:
    KBS_Multiline() {}
    ~KBS_Multiline() {}
    KBS_Multiline& operator =(const bool & a_arg)
    {
        TH_ENTER_FUNCTION;
        KSetGetStyle < GWL_STYLE, BS_MULTILINE >::operator=(a_arg);
        TH_LEAVE_FUNCTION;
        return *this;
    }
    operator bool() const
    {
        bool fResult = KSetGetStyle < GWL_STYLE, BS_MULTILINE >::operator bool();
        TH_ENTER_FUNCTION;
        TH_LEAVE_FUNCTION;
        return fResult;
    }
};
#endif