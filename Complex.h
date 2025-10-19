#pragma once
#include <iostream>
<<<<<<< HEAD
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
=======

class Complex {
private:
    double re_; // действ
    double im_; // мним 
>>>>>>> 19e82b9671605cf18ce56235b6900b9ccb318e45

public:
    Complex();
    Complex(double re, double im);
    Complex(const Complex& other);

    double getReal() const;
    double getImag() const;
    void setReal(double re);
    void setImag(double im);
<<<<<<< HEAD
    NumberType getType() const;
=======
>>>>>>> 19e82b9671605cf18ce56235b6900b9ccb318e45

    Complex conj() const;
    double norm2() const;

    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(const Complex& other) const;
    Complex operator/(const Complex& other) const;

    bool operator==(const Complex& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
<<<<<<< HEAD




    
=======
>>>>>>> 19e82b9671605cf18ce56235b6900b9ccb318e45
};
