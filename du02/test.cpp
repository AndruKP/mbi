//
// Created by andru_kfgnofp on 11/23/2025.
//

#include <fstream>
#include <iostream>

#include "Tree.h"
#include "felsenstein.h"

//tests to test features that aren't described in a21-en.pdf
void test_0() {
    std::cout << "TEST 0" << std::endl;

    Tree t;
    std::fstream("tree.txt", std::ios::in) >> t;
    auto res = t.get_postorder();
}

//test sanity of Felsenstein algo implementation
void test_a() {
    std::cout << "TEST A" << std::endl;

    Tree t;
    const auto left = new Node(nullptr, nullptr, nullptr, 0, 0, "left", A);
    const auto right = new Node(nullptr, nullptr, nullptr, 0, 0, "right", A);
    const auto root = new Node(nullptr, left, right, 1, 1, "root", NONE);

    t.set_root(root);

    const auto probability = felsenstein(t, 1, {{"left", A}, {"right", A}});
    std::cout << probability << std::endl;
    std::cout << "should be around " << 0.0755281471042753 << std::endl;

    delete left;
    delete right;
    delete root;
}

void test_b() {
    std::cout << "TEST B" << std::endl;

    constexpr Base BASE_TO_TEST_B = A;
    const std::string FILENAME = "tree.txt";
    Tree t;
    std::fstream(FILENAME, std::ios::in) >> t;

    std::map<std::string, Base> m;
    for (const auto leaf: t.get_leaves()) {
        m[leaf->name] = BASE_TO_TEST_B;
    }
    const probability probability_1 = felsenstein(t, 1, m);
    const probability probability_02 = felsenstein(t, 0.2, m);
    std::cout << "value for alpha = 1 is " << probability_1 << std::endl;
    std::cout << "value for alpha = 2 is " << probability_02 << std::endl;
}

void test_c() {
    std::cout << "TEST C" << std::endl;
    const std::string ALIGNMENT_FILENAME = "cftr.txt";
    const std::string TREE_FILENAME = "tree.txt";

    alignment a;
    Tree t;
    std::fstream(ALIGNMENT_FILENAME) >> a;
    std::fstream(TREE_FILENAME) >> t;

    std::cout << get_optimal_alpha(t, a);
}

int main() {
    test_0();
    test_a();
    test_b();
    test_c();
    return 0;
}
