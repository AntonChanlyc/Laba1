#include <iostream>
#include <string>
#include <memory>
#include <ctime>
#include <sstream>
#include <stack>

#include "Calculator.h"
#include "Complex.h"
#include "Quaternion.h"
#include "HistorySet.h"

#ifdef _WIN32
extern int start_gui_mode();
#endif

using History = HistorySet<std::string, std::stack<std::shared_ptr<Number>>>;

static std::string now_string() {
    std::time_t t = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return std::string(buf);
}

void print_help() {
    std::cout << "Команды:\n";
    std::cout << " c re im\n";
    std::cout << " q w x y z\n";
    std::cout << " + - * /\n";
    std::cout << " s  — сохранить стек в историю\n";
    std::cout << " hs — список ключей\n";
    std::cout << " hv key — показать\n";
    std::cout << " hd key — удалить\n";
    std::cout << " hsave file\n";
    std::cout << " hload file\n";
    std::cout << " e — выход\n";
}

int console_mode() {
    Calculator calc;
    History hist;

    print_help();

    std::string cmd;
    while (true) {
        std::cout << "> ";
        std::cin >> cmd;

        if (cmd == "c") {
            double r, i; std::cin >> r >> i;
            calc.push(std::make_shared<Complex>(r,i));
        }
        else if (cmd == "q") {
            double w,x,y,z; std::cin >> w >> x >> y >> z;
            calc.push(std::make_shared<Quaternion>(w,x,y,z));
        }
        else if (cmd == "+" || cmd == "-" || cmd == "*" || cmd == "/") {
            calc.performOperation(cmd[0]);
        }
        else if (cmd == "p") {
            auto t = calc.top();
            std::cout << *t << "\n";
        }
        else if (cmd == "s") {
            hist.add(now_string(), calc.snapshot());
        }
        else if (cmd == "hs") {
            for (auto &k : hist.keys()) std::cout << k << "\n";
        }
        else if (cmd == "hv") {
            std::string key; std::cin >> key;
            auto st = hist[key];
            while (!st.empty()) { std::cout << *st.top() << "\n"; st.pop(); }
        }
        else if (cmd == "hd") {
            std::string key; std::cin >> key;
            hist.remove(key);
        }
        else if (cmd == "hsave") {
            std::string f; std::cin >> f;
            hist.save(f);
        }
        else if (cmd == "hload") {
            std::string f; std::cin >> f;
            hist.load(f);
        }
        else if (cmd == "e") break;
        else std::cout << "Неизвестная команда\n";
    }
    return 0;
}

int main(int argc, char** argv) {

    bool gui = false;
    for (int i = 1; i < argc; ++i)
        if (std::string(argv[i]) == "--gui")
            gui = true;

#ifdef _WIN32
    if (gui) return start_gui_mode();
#endif

    return console_mode();
}
