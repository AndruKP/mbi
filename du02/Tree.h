//
// Created by andru_kfgnofp on 11/22/2025.
//
#pragma once


#include <array>
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
typedef long double log_prob;
typedef long double branchLength;

// NONE is used for non-leaf nodes as NONE state, N used as an unknown base
enum Base { A, C, G, T, N, NONE };

const std::vector BASES = {A, C, G, T};
constexpr unsigned int NUM_BASES = 4;

const std::map<char, Base> CHAR_TO_BASE = {{'A', A}, {'C', C}, {'T', T}, {'G', G}, {'N', N}, {'-', N}};

Base get_base(char c);

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

    size_t idx = 0;

    std::array<std::array<probability, 4>, 4> left_child_matrix;
    std::array<std::array<probability, 4>, 4> right_child_matrix;

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

    [[nodiscard]] bool isLeaf() const;

    void get_leaves(std::vector<Node *> &result);

    void get_postorder(std::vector<Node *> &result);

    void precalculate_jd69_matrix(probability alpha);
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

    void precalculate_jd69_matrix(probability alpha) const;

    static probability jukes_cantor_probability(Base from, Base to, probability alpha, branchLength t);

    void set_leaves_bases(std::map<std::string, Base> alignment_col) const;

    [[nodiscard]] std::vector<Node *> get_leaves() const;

    [[nodiscard]] std::vector<Node *> get_postorder() const;
};


std::istream &operator>>(std::istream &is, Tree &tree);
