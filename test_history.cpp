#include <iostream>
#include "HistorySet.h"
#include "Calculator.h"
#include "Complex.h"

using History = HistorySet<std::string, std::stack<std::shared_ptr<Number>>>;

int main() {
    History h;
    std::stack<std::shared_ptr<Number>> s;
    s.push(std::make_shared<Complex>(1,2));
    s.push(std::make_shared<Complex>(3,4));
    std::string k = "2025-01-01 00:00:00";
    h.add(k, s);

    if (!h.contains(k)) { std::cout << "FAIL contains\n"; return 1; }
    auto& fetched = h[k];
    if (fetched.size() != 2) { std::cout << "FAIL size\n"; return 1; }

    h.save("history_test.txt");
    h.clear();
    if (h.size() != 0) { std::cout << "FAIL clear\n"; return 1; }
    h.load("history_test.txt");
    if (!h.contains(k)) { std::cout << "FAIL load\n"; return 1; }

    std::cout << "All history tests passed\n";
    return 0;
}
