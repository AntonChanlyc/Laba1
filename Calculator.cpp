#include "Calculator.h"
#include <stdexcept>

void Calculator::push(std::shared_ptr<Number> num) {
    st.push(num);
}

void Calculator::pop() {
    if (st.empty()) throw std::runtime_error("Stack is empty");
    st.pop();
}

bool Calculator::empty() const {
    return st.empty();
}

std::shared_ptr<Number> Calculator::top() const {
    if (st.empty()) throw std::runtime_error("Stack is empty");
    return st.top();
}

std::stack<std::shared_ptr<Number>> Calculator::snapshot() const {
    return st; 
}

void Calculator::clear() {
    while (!st.empty()) st.pop();
}

void Calculator::performOperation(char op) {
    if (st.size() < 2) throw std::runtime_error("Not enough numbers on stack");
    auto b = st.top(); st.pop();
    auto a = st.top(); st.pop();
    Number* result = nullptr;
    switch (op) {
        case '+': result = a->add(*b); break;
        case '-': result = a->sub(*b); break;
        case '*': result = a->mul(*b); break;
        case '/': result = a->div(*b); break;
        default: throw std::runtime_error("Unknown operation");
    }
    st.push(std::shared_ptr<Number>(result));
}
