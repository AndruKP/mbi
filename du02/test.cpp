//
// Created by andru_kfgnofp on 11/23/2025.
//

#include <fstream>
#include <iostream>

#include "Tree.h"
#include "felsenstein.h"

void test_a() {
}

void test_b() {
    Tree t;
    std::fstream("tree.txt", std::ios::in) >> t;

    std::map<std::string, Base> m = {{"human", A}};
    long double probability = felsenstein(t, 0, m);
    std::cout << probability << std::endl;
}

int main() {
    std::cout << "hello world" << std::endl;
    test_a();
    test_b();
    return 0;
}
