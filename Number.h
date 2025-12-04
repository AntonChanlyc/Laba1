#pragma once
#include <iostream>

enum NumberType {
    COMPLEX,
    QUATERNION
};

class Number {
public:
    virtual ~Number() = default;

    virtual NumberType getType() const = 0;

    virtual Number* clone() const = 0;

    // јрифметические операции Ч виртуальные
    virtual Number* add(const Number& other) const = 0;
    virtual Number* sub(const Number& other) const = 0;
    virtual Number* mul(const Number& other) const = 0;
    virtual Number* div(const Number& other) const = 0;

    virtual void print(std::ostream& os) const = 0;
};

inline std::ostream& operator<<(std::ostream& os, const Number& n) {
    n.print(os);
    return os;
}
