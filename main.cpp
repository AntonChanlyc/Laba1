<<<<<<< HEAD
#include "Quaternion.h"
#include "Complex.h"
#include <iostream>
using namespace std;

// Вспомогательная функция для печати типа числа
string typeToString(NumberType t) {
    switch(t) {
        case COMPLEX: return "Complex";
        case QUATERNION: return "Quaternion";
        default: return "Unknown";
    }
}

int main() {
    NumberType typeChoice;
    cout << "Выберите тип числа:\n1 - Complex\n2 - Quaternion\nВведите цифру: ";
    int choice;
    cin >> choice;
    if (choice == 1)
        typeChoice = COMPLEX;
    else
        typeChoice = QUATERNION;

    Complex c1, c2;
    Quaternion q1, q2;
    char op;

    if (typeChoice == COMPLEX) {
        double a, b;
        cout << "Введите комплексное число 1 (a b): ";
        cin >> a >> b;
        c1 = Complex(a, b);

        cout << "Введите комплексное число 2 (a b): ";
        cin >> a >> b;
        c2 = Complex(a, b);
    } else {
        double a, b, c, d;
        cout << "Введите кватернион 1 (a b c d): ";
        cin >> a >> b >> c >> d;
        q1 = Quaternion(a, b, c, d);

        cout << "Введите кватернион 2 (a b c d): ";
        cin >> a >> b >> c >> d;
        q2 = Quaternion(a, b, c, d);
    }

    cout << "\nВыберите операцию:\n"
         << " +  — сложение\n"
         << " -  — вычитание\n"
         << " *  — умножение\n"
         << " =  — проверка равенства\n"
         << " n  — норма первого числа\n"
         << " m  — норма второго числа\n"
         << " t  — показать тип переменной\n"
         << " q  — выход\n"
         << "Введите символ: ";

    while (cin >> op) {
        if (op == 'q' || op == 'Q') break;

        if (typeChoice == COMPLEX) {
            Complex result;
            switch(op) {
                case '+': result = c1 + c2; cout << "Результат: " << result << endl; break;
                case '-': result = c1 - c2; cout << "Результат: " << result << endl; break;
                case '*': result = c1 * c2; cout << "Результат: " << result << endl; break;
                case '=': cout << ((c1 == c2) ? "Равны" : "Не равны") << endl; break;
                case 'n':
                case 'N': cout << "Норма c1 = " << c1.norm2() << endl; break;
                case 'm':
                case 'M': cout << "Норма c2 = " << c2.norm2() << endl; break;
                case 't':
                case 'T': 
                    cout << "Тип c1: " << typeToString(c1.getType()) << endl;
                    cout << "Тип c2: " << typeToString(c2.getType()) << endl;
                    break;
                default: cout << "Неизвестная операция" << endl;
            }
        } else {
            Quaternion result;
            switch(op) {
                case '+': result = q1 + q2; cout << "Результат: " << result << endl; break;
                case '-': result = q1 - q2; cout << "Результат: " << result << endl; break;
                case '*': result = q1 * q2; cout << "Результат: " << result << endl; break;
                case '=': cout << ((q1 == q2) ? "Равны" : "Не равны") << endl; break;
                case 'n':
                case 'N': cout << "Норма q1 = " << q1.norm2() << endl; break;
                case 'm':
                case 'M': cout << "Норма q2 = " << q2.norm2() << endl; break;
                case 't':
                case 'T': 
                    cout << "Тип q1: " << typeToString(q1.getType()) << endl;
                    cout << "Тип q2: " << typeToString(q2.getType()) << endl;
                    break;
                default: cout << "Неизвестная операция" << endl;
            }
        }

        cout << "\nВведите следующую операцию (+, -, *, =, n, m, t, q): ";
    }

    cout << "\nРабота завершена.\n";
    return 0;
}
=======
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
>>>>>>> 19e82b9671605cf18ce56235b6900b9ccb318e45
