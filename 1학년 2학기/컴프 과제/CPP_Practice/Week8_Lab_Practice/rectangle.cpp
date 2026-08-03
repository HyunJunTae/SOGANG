#include "rectangle.hpp"

// TODO: Fill in these functions.

bool Rectangle::Shift(int delta_x, int delta_y) {
    // Rectangle 에서는 point 의 private 멤버에는 바로 접근 불가능하니까
    // left_up.x가 아니라 GetX()를 써서 접근해야함.
    int a = left_up.GetX() + delta_x;
    int b = left_up.GetY() + delta_y;
    int c = right_low.GetX() + delta_x;
    int d = right_low.GetY() + delta_y;

    if (a<0 || b<0)
        return false;

    left_up.SetXY(a, b);
    right_low.SetXY(c, d);

    return true;
}

bool Rectangle::DoubleSize() {
    int a = right_low.GetX() * 2;
    int b = right_low.GetY() * 2;

    right_low.SetXY(a, b);
    // right_low(a, b); 생성자니까 여기서 안씀.

    // left_up을 고정시키고 right_low만 두 배로 옮기는거니까 false 발생 안함.

    return true;
}

bool Rectangle::Rotate() {
    int a = left_up.GetX();
    int b = left_up.GetY()-(right_low.GetX()-left_up.GetX()); // new left_up y
    int c = left_up.GetX()+(right_low.GetY()-left_up.GetY()); // new right_low x
    int d = left_up.GetY(); // new right_low y
    
    if (b<0 || c<0 || d<0)
        return false;
    
    left_up.SetXY(a, b);
    right_low.SetXY(c, d);
    return true;

}
