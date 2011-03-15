#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "commonHeaders.h"

class Rect
{
private:
    ULONG   m_left;
    ULONG   m_top;
    ULONG   m_right;
    ULONG   m_bottom;
public:
    Rect                        ()          {m_left = m_top = m_right = m_bottom = 0;}
    Rect                        (ULONG left,
                                 ULONG top,
                                 ULONG right,
                                 ULONG bottom)
    {
            m_left  =   left;
            m_top   =   top;
            m_right =   right;
            m_bottom=   bottom;
    }
    long    getWidth()
    {
            return      m_right - m_left;
    }
    long    getHeight()
    {
            return      m_bottom - m_top;
    }
    long    getArea()
    {
            return      getWidth() * getHeight();
    }
    long    getLeft()
    {
            return      m_left;
    }
    long    getRight()
    {
            return      m_right;
    }
    long    getTop()
    {
            return      m_top;
    }
    long    getBottom()
    {
            return      m_bottom;
    }
    Rect    &operator=  (const Rect &r)
    {
        if (this    !=  &r)
        {
            m_left  =   r.m_left;
            m_right =   r.m_right;
            m_top   =   r.m_top;
            m_bottom=   r.m_bottom;

        }
            return      *this;
    }
};


#endif
