#ifndef RECTANGLE_CLASS
#define RECTANGLE_CLASS

#include <iostream>
#include "point.hpp"
#include "shape.hpp"

using std::cout;
using std::endl;

class Rectangle: public Shape {
private:
    Point left_up;
    Point right_low;
public:
    // Initialize a rectangle with the information of left-upper point and
    // right-lower point. Assume that valid coordinates are given.
    Rectangle(int left_up_x, int left_up_y, int right_low_x, int right_low_y)
        : left_up(left_up_x, left_up_y), right_low(right_low_x, right_low_y)
    { }

    // Print out the four points that compose this rectangle.
    void Print() {
        int left_up_x = left_up.GetX();
        int left_up_y = left_up.GetY();
        int right_low_x = right_low.GetX();
        int right_low_y = right_low.GetY();
        cout << "Left-upper: (" << left_up_x << ", " << left_up_y << ")\n";
        cout << "Left-lower: (" << left_up_x << ", " << right_low_y << ")\n";
        cout << "Right-upper: (" << right_low_x << ", " << left_up_y << ")\n";
        cout << "Right-lower: (" << right_low_x << ", " << right_low_y << ")\n";
    }

    // Shift the position of this rectangle. Increase its x coordinate by
    // 'delta_x' and y coordinate by 'delta_y'. Negative values can be given as
    // arguments. If X or Y coordinate of any of the four points becomes
    // negative after the shift, don't shift it and just return false.
    bool Shift(int delta_x, int delta_y);

    // Increase the height and width of the rectangle by twice, while fixing the
    // position of its left-upper point. If X or Y coordinate of any of the four
    // points becomes negative after the resizing, don't increase the size and
    // just return false.
    bool DoubleSize();

    // Rotate this rectangle counterclockwise by 90 degree. Its left-upper point
    // must be the pivot of the rotation. If X or Y coordinate of any of the
    // four points becomes negative after the rotation, don't rotate it and just
    // return false.
    bool Rotate();
};

#endif
