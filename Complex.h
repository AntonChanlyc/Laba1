#pragma once
#include "Number.h"
//dfd
class Complex : public Number {
protected:
    double re_;
    double im_;

public:
    Complex();
    Complex(double re, double im);
    Complex(const Complex& other);

    double getReal() const;
    double getImag() const;

    // Реализация виртуальных методов
    NumberType getType() const override { return COMPLEX; }
    Number* clone() const override { return new Complex(*this); }

    Number* add(const Number& other) const override;
    Number* sub(const Number& other) const override;
    Number* mul(const Number& other) const override;
    Number* div(const Number& other) const override;

    void print(std::ostream& os) const override;
};
