#include <iostream>
#include <functional>
#include <vector>

std::function<int(int)> create_func_from_rpn(std::string exp) {
    std::vector<std::function<int(int)>> rpn_stack;
    bool in_num = false;
    int num = 0;
    for (char c : exp) {
        if (isdigit(c)) {
            in_num = true;
            num *= 10;
            num += c - '0';
        }
        if (c == ' ') {
            if (in_num) {
                rpn_stack.push_back([=](int x){ return num; });
                num = 0;
                in_num = false;
            }
        }
        if (c == 'x') {
            rpn_stack.push_back([](int x){ return x; });
        }
        if (c == '+') {
            auto right = rpn_stack.back();
            rpn_stack.pop_back();
            auto left = rpn_stack.back();
            rpn_stack.pop_back();
            rpn_stack.push_back([=](int x){ return left(x) + right(x); });
        }
        if (c == '*') {
            auto right = rpn_stack.back();
            rpn_stack.pop_back();
            auto left = rpn_stack.back();
            rpn_stack.pop_back();
            rpn_stack.push_back([=](int x){ return left(x) * right(x); });
        }
    }
    return rpn_stack.back();
}

int main()
{
    auto f = create_func_from_rpn("21 34 x * +");   // f = 21+34*x

    std::cout << f(3) << std::endl;                 // 21+34*3 értéke
    std::cout << f(4) << std::endl;                 // 21+34*4 értéke
    std::cout << f(5) << std::endl;
    return 0;
}
