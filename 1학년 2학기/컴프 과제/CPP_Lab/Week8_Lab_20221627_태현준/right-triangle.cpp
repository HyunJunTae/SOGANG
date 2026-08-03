#include "right-triangle.hpp"

// TODO: Fill in these functions.

bool RightTriangle::Shift(int delta_x, int delta_y) {
    int a = ra_point.GetX() + delta_x;
    int b = ra_point.GetY() + delta_y;

    if(a<0 || b<0)
        return false;

    ra_point.SetXY(a, b);

    return false;
}

bool RightTriangle::DoubleSize() {
    int a = x_offset*2;
    int b = y_offset*2;

    if (ra_point.GetX()+a<0 || ra_point.GetY()+b<0)
        return false;

    x_offset = a;
    y_offset = b;

    return true;
}

bool RightTriangle::Rotate() {
    int a = y_offset; // new x_offset
    int b = -x_offset; // new y_offset

    if (ra_point.GetX()+a<0 || ra_point.GetY()+b<0)
        return false;

    x_offset = a;
    y_offset = b;

    return true;
}
