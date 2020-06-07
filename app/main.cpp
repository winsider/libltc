#include <iostream>
#include <ltc/ltc.hpp>

int main(int argc, char *argv[])
{
    HelloWorld hello("Hello World!");
    hello.Print(std::cout);
    return 0;
}
