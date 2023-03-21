#pragma once
#include <iostream>
#include <random>
#include <cassert>
#include <chrono>
#include <cstring>

#define TEST_CALL(f)    do {\
    std::cout << "calc "<< #f << " start" << std::endl;\
    auto b = std::chrono::system_clock::now();\
    f();\
    auto e = std::chrono::system_clock::now();\
    auto diff  = std::chrono::duration_cast<std::chrono::milliseconds>(e-b).count();\
    std::cout << "calc "<< #f << "end  cost:" << diff << "ms" << std::endl;\
}while(0)

