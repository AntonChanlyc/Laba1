#pragma once
#include "Complex.h"

class Quaternion : public Complex {
protected:
    double j_;
    double k_;

public:
    
    Quaternion();
    Quaternion(double a, double b, double c, double d);
    Quaternion(const Quaternion& other);

    
    double getj() const;
    double getk() const;
    void setJ(double j);
    void setK(double k);

    
    Quaternion operator+(const Quaternion& other) const;
    Quaternion operator-(const Quaternion& other) const;
    Quaternion operator*(const Quaternion& other) const;
    bool operator==(const Quaternion& other) const;

    double norm2() const;

    
    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);
};
