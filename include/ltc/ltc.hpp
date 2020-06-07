#pragma once
#include <string>
#include <iostream>

class HelloWorld
{
public:
    HelloWorld(const std::string message);
    void Print(std::ostream &os) const;

private:
    std::string m_message;
};