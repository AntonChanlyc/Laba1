#include "Complex.h"
#include <stdexcept>

Complex::Complex() : re_(0), im_(0) {}
Complex::Complex(double re, double im) : re_(re), im_(im) {}
Complex::Complex(const Complex& other)
    : re_(other.re_), im_(other.im_) {}

double Complex::getReal() const { return re_; }
double Complex::getImag() const { return im_; }

// -------- ¿–»‘Ã≈“» ¿ --------

Number* Complex::add(const Number& other) const {
    if (other.getType() != COMPLEX)
        throw std::runtime_error("Complex can operate only with Complex");

    const Complex& c = dynamic_cast<const Complex&>(other);
    return new Complex(re_ + c.re_, im_ + c.im_);
}

Number* Complex::sub(const Number& other) const {
    if (other.getType() != COMPLEX)
        throw std::runtime_error("Complex can operate only with Complex");

    const Complex& c = dynamic_cast<const Complex&>(other);
    return new Complex(re_ - c.re_, im_ - c.im_);
}

Number* Complex::mul(const Number& other) const {
    if (other.getType() != COMPLEX)
        throw std::runtime_error("Complex can operate only with Complex");

    const Complex& c = dynamic_cast<const Complex&>(other);
    return new Complex(
        re_ * c.re_ - im_ * c.im_,
        re_ * c.im_ + im_ * c.re_
    );
}

Number* Complex::div(const Number& other) const {
    if (other.getType() != COMPLEX)
        throw std::runtime_error("Complex can operate only with Complex");

    const Complex& c = dynamic_cast<const Complex&>(other);
    double d = c.re_*c.re_ + c.im_*c.im_;
    if (d == 0) throw std::runtime_error("Division by zero!");

    return new Complex(
        (re_*c.re_ + im_*c.im_) / d,
        (im_*c.re_ - re_*c.im_) / d
    );
}

void Complex::print(std::ostream& os) const {
    os << re_;
    if (im_ >= 0) os << "+";
    os << im_ << "i";
}
