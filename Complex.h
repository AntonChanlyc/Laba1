#pragma once
#include <iostream>

class Complex {
private:
    double re_; // действ
    double im_; // мним 

public:
    Complex();
    Complex(double re, double im);
    Complex(const Complex& other);

    double getReal() const;
    double getImag() const;
    void setReal(double re);
    void setImag(double im);

    Complex conj() const;
    double norm2() const;

    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(const Complex& other) const;
    Complex operator/(const Complex& other) const;

    bool operator==(const Complex& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
};
