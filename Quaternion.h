#pragma once
#include "Number.h"

class Quaternion : public Number {
private:
    double w, x, y, z;

public:
    Quaternion();
    Quaternion(double w, double x, double y, double z);
    Quaternion(const Quaternion& q);

    double getW() const;
    double getX() const;
    double getY() const;
    double getZ() const;

    NumberType getType() const override { return QUATERNION; }
    Number* clone() const override { return new Quaternion(*this); }

    Number* add(const Number& other) const override;
    Number* sub(const Number& other) const override;
    Number* mul(const Number& other) const override;
    Number* div(const Number& other) const override;

    void print(std::ostream& os) const override;
};
