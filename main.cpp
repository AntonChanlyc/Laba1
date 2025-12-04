#include <iostream>
#include "Calculator.h"
#include "Complex.h"
#include "Quaternion.h"

#ifdef INTERFACE

int main() {
    Calculator calc;

    std::cout << "Введите команду:\n";
    std::cout << "c re im — push Complex\n";
    std::cout << "q w x y z — push Quaternion\n";
    std::cout << "+ - * / — операция\n";
    std::cout << "p — print top\n";
    std::cout << "e — exit\n";

    char cmd;
    while (std::cin >> cmd) {
        if (cmd == 'e') break;

        try {
            if (cmd == 'c') {
                double r, i;
                std::cin >> r >> i;
                calc.push(std::make_shared<Complex>(r, i));
            }
            else if (cmd == 'q') {
                double w,x,y,z;
                std::cin >> w >> x >> y >> z;
                calc.push(std::make_shared<Quaternion>(w,x,y,z));
            }
            else if (cmd == 'p') {
                std::cout << *calc.top() << "\n";
            }
            else {
                calc.performOperation(cmd);
            }
        }
        catch (const std::exception& ex) {
            std::cout << "Ошибка: " << ex.what() << "\n";
        }
    }
}

#endif
