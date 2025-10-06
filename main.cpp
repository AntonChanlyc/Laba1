#include "Complex.h"
#include <iostream>
#include <cassert>
#include <cmath>

using namespace std;
#define TESTS   

#ifdef TESTS
int main() {

    Complex z0;  
    assert(z0.getReal() == 0 && z0.getImag() == 0);

    Complex z1(1, 2);
    Complex z2(3, -1);

    
    Complex z3(z1);  
    assert(z3 == z1);

    
    assert(z1.getReal() == 1);
    assert(z1.getImag() == 2);

    
    z3.setReal(5);
    z3.setImag(-7);
    assert(z3.getReal() == 5);
    assert(z3.getImag() == -7);

    assert((z1 + z2) == Complex(4, 1));
    assert((z1 - z2) == Complex(-2, 3));
    assert((z1 * z2) == Complex(5, 5));


    Complex div = z1 / z2;
    Complex back = div * z2;
    assert(std::abs(back.getReal() - z1.getReal()) < 1e-9);
    assert(std::abs(back.getImag() - z1.getImag()) < 1e-9);

    Complex c_conj = z1.conj();
    assert(c_conj == Complex(1, -2));


    cout << "все тесты пройдены!\n";
    return 0;
}
#else
int main() {
    Complex z(3, 4);
    cout << "z = " << z << endl;
    return 0;
}
#endif
