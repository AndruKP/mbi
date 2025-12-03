//
// Created by andru_kfgnofp on 11/22/2025.
//
#pragma once


#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

const std::string ROOT_NAME = "Root";

typedef long double probability;
typedef long double log_prob;
typedef long double branch_length;

// NONE is used for non-leaf nodes as NONE state, N used as an unknown base
// TODO: Base to base
enum Base { A, C, G, T, N, NONE };

const std::vector BASES = {A, C, G, T};
constexpr unsigned int NUM_BASES = 4;

const std::map<char, Base> CHAR_TO_BASE = {{'A', A}, {'C', C}, {'T', T}, {'G', G}, {'N', N}, {'-', N}};

Base get_base(char c);

class node_exception : std::logic_error {
public:
    node_exception(const std::string &what) : std::logic_error(what) {
    }
};

class node {
public:
    node *parent = nullptr;
    node *left = nullptr;
    node *right = nullptr;

    //Data
    branch_length left_child_distance = 0;
    branch_length right_child_distance = 0;
    std::string name;
    Base base = NONE;

    size_t idx = 0;

    std::array<std::array<probability, NUM_BASES>, NUM_BASES> left_child_matrix = {};
    std::array<std::array<probability, NUM_BASES>, NUM_BASES> right_child_matrix = {};

    node(node *par, node *l, node *r,
         const branch_length lcd, const branch_length rcd, std::string name, const Base b) : parent(par), left(l),
        right(r),
        left_child_distance(lcd),
        right_child_distance(rcd), name(std::move(name)),
        base(b) {
    }

    explicit node(std::string name) : name(std::move(name)) {
    }

    node() = default;

    void add_child(node *child, branch_length distance);

    [[nodiscard]] bool is_leaf() const;

    void get_leaves(std::vector<node *> &result);

    void get_postorder(std::vector<node *> &result);

    void precalculate_jc69_matrix(probability alpha);
};

class tree {
    node *root = nullptr;
    std::vector<node *> leaves;

public:
    void set_root(node *new_root) {
        root = new_root;
    }

    [[nodiscard]] node *get_root() const {
        return root;
    }

    void precalculate_jd69_matrix(probability alpha) const;

    static probability jukes_cantor_probability(Base from, Base to, probability alpha, branch_length t);

    void set_leaves_bases(std::map<std::string, Base> alignment_col);

    [[nodiscard]] std::vector<node *> get_leaves();

    [[nodiscard]] std::vector<node *> get_postorder() const;
};


std::istream &operator>>(std::istream &is, tree &tree);
