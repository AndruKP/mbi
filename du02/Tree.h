//
// Created by andru_kfgnofp on 11/22/2025.
//

#ifndef DU02_TREE_H
#define DU02_TREE_H

enum base { A, C, G, T };

class Node {
    Node *parent = nullptr;
    Node *left = nullptr;
    long double left_distance = 0;
    Node *right = nullptr;
    long double right_distance = 0;

    //TODO: data
};

class Tree {
    Node *root = nullptr;

public:
    static long double jukes_cantor_probability(base from, base to, long double alpha, long double t);
};


#endif //DU02_TREE_H
