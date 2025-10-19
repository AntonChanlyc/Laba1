#include "Complex.h"
#include <stdexcept>

// Конструкторы
<<<<<<< HEAD
Complex::Complex() : re_(0), im_(0), type_(COMPLEX) {}
Complex::Complex(double re, double im) : re_(re), im_(im), type_(COMPLEX) {}
Complex::Complex(const Complex& other)
    : re_(other.re_), im_(other.im_), type_(other.type_) {}

NumberType Complex::getType() const {
    return type_;
}
=======
Complex::Complex() : re_(0), im_(0) {}
Complex::Complex(double re, double im) : re_(re), im_(im) {}
Complex::Complex(const Complex& other) : re_(other.re_), im_(other.im_) {}
>>>>>>> 19e82b9671605cf18ce56235b6900b9ccb318e45

// Гетеры и сетеры
double Complex::getReal() const { return re_; }
double Complex::getImag() const { return im_; }
void Complex::setReal(double re) { re_ = re; }
void Complex::setImag(double im) { im_ = im; }


Complex Complex::conj() const { return Complex(re_, -im_); }
double Complex::norm2() const { return re_ * re_ + im_ * im_; }

// Операторы
Complex Complex::operator+(const Complex& other) const {
    return Complex(re_ + other.re_, im_ + other.im_);
}
Complex Complex::operator-(const Complex& other) const {
    return Complex(re_ - other.re_, im_ - other.im_);
}
Complex Complex::operator*(const Complex& other) const {
    return Complex(re_ * other.re_ - im_ * other.im_,
                   re_ * other.im_ + im_ * other.re_);
}
Complex Complex::operator/(const Complex& other) const {
    double denom = other.re_ * other.re_ + other.im_ * other.im_;
    if (denom == 0) throw std::runtime_error("Division by zero!");
    return Complex((re_ * other.re_ + im_ * other.im_) / denom,
                   (im_ * other.re_ - re_ * other.im_) / denom);
}


bool Complex::operator==(const Complex& other) const {
    return re_ == other.re_ && im_ == other.im_;
}

// Вывод
std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.re_;
    if (c.im_ >= 0) os << "+";
    os << c.im_ << "i";
    return os;
}
