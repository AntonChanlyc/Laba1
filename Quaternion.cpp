#include "Quaternion.h"
#include <stdexcept>
#include <cmath>

Quaternion::Quaternion() : w(0), x(0), y(0), z(0) {}
Quaternion::Quaternion(double w, double x, double y, double z)
    : w(w), x(x), y(y), z(z) {}
Quaternion::Quaternion(const Quaternion& q)
    : w(q.w), x(q.x), y(q.y), z(q.z) {}

double Quaternion::getW() const { return w; }
double Quaternion::getX() const { return x; }
double Quaternion::getY() const { return y; }
double Quaternion::getZ() const { return z; }

// ---------- ¿–»‘Ã≈“» ¿ -------------
Number* Quaternion::add(const Number& other) const {
    if (other.getType() != QUATERNION)
        throw std::runtime_error("Quaternion can operate only with Quaternion");

    const Quaternion& q = dynamic_cast<const Quaternion&>(other);
    return new Quaternion(
        w + q.w, x + q.x, y + q.y, z + q.z
    );
}

Number* Quaternion::sub(const Number& other) const {
    if (other.getType() != QUATERNION)
        throw std::runtime_error("Quaternion can operate only with Quaternion");

    const Quaternion& q = dynamic_cast<const Quaternion&>(other);
    return new Quaternion(
        w - q.w, x - q.x, y - q.y, z - q.z
    );
}

Number* Quaternion::mul(const Number& other) const {
    if (other.getType() != QUATERNION)
        throw std::runtime_error("Quaternion can operate only with Quaternion");

    const Quaternion& q = dynamic_cast<const Quaternion&>(other);

    return new Quaternion(
        w*q.w - x*q.x - y*q.y - z*q.z,
        w*q.x + x*q.w + y*q.z - z*q.y,
        w*q.y - x*q.z + y*q.w + z*q.x,
        w*q.z + x*q.y - y*q.x + z*q.w
    );
}

Number* Quaternion::div(const Number& other) const {
    if (other.getType() != QUATERNION)
        throw std::runtime_error("Quaternion can operate only with Quaternion");

    const Quaternion& q = dynamic_cast<const Quaternion&>(other);

    double norm2 = q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z;
    if (norm2 == 0) throw std::runtime_error("Division by zero quaternion!");

    // conj(q)
    Quaternion conj(q.w, -q.x, -q.y, -q.z);

    // multiply this * conj(q)
    Number* temp = this->mul(conj);

    // temp is Number*, convert to Quaternion*
    Quaternion* resQ = dynamic_cast<Quaternion*>(temp);
    if (!resQ) {
        delete temp;
        throw std::runtime_error("Internal error: mul() returned wrong type");
    }

    // finish division
    resQ->w /= norm2;
    resQ->x /= norm2;
    resQ->y /= norm2;
    resQ->z /= norm2;

    return resQ;  // already a Quaternion*
}



void Quaternion::print(std::ostream& os) const {
    os << w << " + "
       << x << "i + "
       << y << "j + "
       << z << "k";
}
