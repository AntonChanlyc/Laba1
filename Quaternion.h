#pragma once
#include "Number.h"

class Quaternion : public Number {
public:
    double w, x, y, z;
    Quaternion();
    Quaternion(double w_, double x_, double y_, double z_);
    Quaternion(const Quaternion& q);

    NumberType getType() const override { return QUATERNION; }
    Number* clone() const override { return new Quaternion(*this); }

    Number* add(const Number& other) const override;
    Number* sub(const Number& other) const override;
    Number* mul(const Number& other) const override;
    Number* div(const Number& other) const override;

    void print(std::ostream& os) const override;
};
