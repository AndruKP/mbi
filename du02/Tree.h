//
// Created by andru_kfgnofp on 11/22/2025.
//
#pragma once


#include <stdexcept>
#include <string>
#include <utility>
#include <iostream>
#include <sstream>
#include <cmath>
#include <map>
#include <vector>

const std::string ROOT_NAME = "Root";

typedef long double probability;
typedef long double branchLength;

// None is used for non-leaf nodes as NONE state, N used as an unknown base
enum Base { A, C, G, T, N, NONE };

class NodeException : std::logic_error {
public:
    NodeException(const std::string &what) : std::logic_error(what) {
    }
};

class Node {
public:
    Node *parent = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;

    //Data
    branchLength left_child_distance = 0;
    branchLength right_child_distance = 0;
    std::string name;
    Base base = NONE;

    Node(Node *par, Node *l, Node *r,
         const branchLength lcd, const branchLength rcd, std::string name, const Base b) : parent(par), left(l),
        right(r),
        left_child_distance(lcd),
        right_child_distance(rcd), name(std::move(name)),
        base(b) {
    }

    explicit Node(std::string name) : name(std::move(name)) {
    }

    Node() = default;

    void add_child(Node *child, branchLength distance);

    bool isLeaf() const;

    std::vector<Node *> get_leaves();

    std::vector<Node *> get_postorder();
};

class Tree {
    Node *root = nullptr;

public:
    void set_root(Node *new_root) {
        root = new_root;
    }

    [[nodiscard]] Node *get_root() const {
        return root;
    }

    static probability jukes_cantor_probability(Base from, Base to, probability alpha, branchLength t);

    void set_leaves_bases(std::map<std::string, Base> alignment_col) const;

    std::vector<Node *> get_leaves() const;

    std::vector<Node *> get_postorder() const;
};


std::istream &operator>>(std::istream &is, Tree &tree);
