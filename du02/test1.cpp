//
// Created by andru_kfgnofp on 11/23/2025.
//

#include <iostream>

#include "Tree.h"
#include "felsenstein.h"

int main() {
    Tree t;
    std::map<std::string, base> m = {{"human", A}};
    felsenstein(t, 0, m);
    std::cout << "hello world" << std::endl;
    return 0;
}
