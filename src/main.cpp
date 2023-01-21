#include <iostream>
#include "foo.h"
#include "bar.h"


int main(int argc, const char** argv) {
    std::cout << "Hello World from main" << std::endl;
    foo();
    bar();
    return 0;
}