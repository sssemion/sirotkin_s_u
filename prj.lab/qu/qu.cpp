#include<iostream>

class Vec2d {
public:
    Vec2d(double x1, double y1, double x2, double y2) : x1_(x1), y1_(y1), x2_(x2), y2_(y2) {};
    Vec2d(const Vec2d &copy) = default;
    Vec2d& operator=(const Vec2d &copy) = default;
    ~Vec2d() = default;

    double length() const { return sqrt(pow(x1_ - x2_, 2) + pow(y1_ - y2_, 2)); };
    bool operator==(const Vec2d &rhs) const;
    bool operator!=(const Vec2d &rhs) const;
    Vec2d operator-() const;
    Vec2d& operator+=(const Vec2d &rhs);
    Vec2d& operator-=(const Vec2d &rhs);


    double x1() const { return x1_; };
    double y1() const { return y1_; };
    double x2() const { return x2_; };
    double y2() const { return y2_; };

private:
    double x1_ = 0, y1_ = 0;
    double x2_ = 0, y2_ = 0;
};

Vec2d Vec2d::operator-() const {
    return {x2_, y2_, x1_, y1_};
}

Vec2d& Vec2d::operator+=(const Vec2d &rhs) {
    x2_ += (rhs.x2_ - rhs.x1_);
    y2_ += (rhs.y2_ - rhs.y1_);
    return *this;
}

Vec2d &Vec2d::operator-=(const Vec2d &rhs) {
    return operator+=(-rhs);
}


int test_passed = 0, test_failed = 0;
void check(bool test) {
    if (test) {
        ++test_passed;
        return;
    }
    ++test_failed;
    std::cerr << "Test " << test_failed + test_passed << " failed" << std::endl;
}

int main() {
    Vec2d v(0, 0, 4, 3);
    check(v.x1() == 0);
    check(v.y1() == 0);
    check(v.x2() == 4);
    check(v.y2() == 3);
    check(v.length() == 5);
    v = -v;
    check(v.x2() == 0);
    check(v.y2() == 0);
    check(v.x1() == 4);
    check(v.y1() == 3);
    check(v.length() == 5);

    Vec2d v1(0, 0, 1, 1);
    Vec2d v2(3, 3, 4, 2);
    v1 += v2;
    check(v1.x1() == 0);
    check(v1.y1() == 0);
    check(v1.x2() == 2);
    check(v1.y2() == 0);

    std::cout << test_passed << " tests passed successfully, " << test_failed << " failed." << std::endl;
}
