#pragma once
#include <stack>
#include <memory>
#include "Number.h"

class Calculator {
private:
    std::stack<std::shared_ptr<Number>> st;

public:
    void push(std::shared_ptr<Number> num);
    void pop();
    bool empty() const;

    void performOperation(char op); // '+', '-', '*', '/'

    std::shared_ptr<Number> top() const;
};
