#pragma once
#include <iostream>
#include <cmath>

enum NumberType {
    COMPLEX,
    QUATERNION
};

class Complex {
protected:
    double re_; // действ
    double im_; // мним 
    NumberType type_;

public:
    Complex();
    Complex(double re, double im);
    Complex(const Complex& other);

    double getReal() const;
    double getImag() const;
    void setReal(double re);
    void setImag(double im);
    NumberType getType() const;

    Complex conj() const;
    double norm2() const;

    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(const Complex& other) const;
    Complex operator/(const Complex& other) const;

    bool operator==(const Complex& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Complex& c);




    
};
