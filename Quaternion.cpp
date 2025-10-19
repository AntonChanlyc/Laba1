#include "Quaternion.h"
#include <cmath>


Quaternion::Quaternion() : Complex(0, 0), j_(0), k_(0) {type_ = QUATERNION;}
Quaternion::Quaternion(double a, double b, double c, double d)
    : Complex(a, b), j_(c), k_(d) { type_ = QUATERNION; }
Quaternion::Quaternion(const Quaternion& other)
    : Complex(other.re_, other.im_), j_(other.j_), k_(other.k_) {type_ = QUATERNION;}

// Методы 
double Quaternion::getj() const { return j_; }
double Quaternion::getk() const { return k_; }
void Quaternion::setJ(double j) { j_ = j; }
void Quaternion::setK(double k) { k_ = k; }

// операторы
Quaternion Quaternion::operator+(const Quaternion& other) const {
    return Quaternion(re_ + other.re_,
                      im_ + other.im_,
                      j_ + other.j_,
                      k_ + other.k_);
}
Quaternion Quaternion::operator-(const Quaternion& other) const {
    return Quaternion(re_ - other.re_,
                      im_ - other.im_,
                      j_ - other.j_,
                      k_ - other.k_);
}
Quaternion Quaternion::operator*(const Quaternion& other) const {
    double a1 = re_, b1 = im_, c1 = j_, d1 = k_;
    double a2 = other.re_, b2 = other.im_, c2 = other.j_, d2 = other.k_;

    return Quaternion(
        a1*a2 - b1*b2 - c1*c2 - d1*d2,
        a1*b2 + b1*a2 + c1*d2 - d1*c2,
        a1*c2 - b1*d2 + c1*a2 + d1*b2,
        a1*d2 + b1*c2 - c1*b2 + d1*a2
    );
}
bool Quaternion::operator==(const Quaternion& other) const {
    return std::abs(re_ - other.re_) < 1e-9 &&
           std::abs(im_ - other.im_) < 1e-9 &&
           std::abs(j_ - other.j_) < 1e-9 &&
           std::abs(k_ - other.k_) < 1e-9;
}

double Quaternion::norm2() const {
    return re_*re_ + im_*im_ + j_*j_ + k_*k_;
}

std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
    os << q.re_;
    if (q.im_ >= 0) os << "+";
    os << q.im_ << "i";
    if (q.j_ >= 0) os << "+";
    os << q.j_ << "j";
    if (q.k_ >= 0) os << "+";
    os << q.k_ << "k";
    return os;
}
