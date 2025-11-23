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

const std::string ROOT_NAME = "Root";

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
    long double left_child_distance = 0;
    long double right_child_distance = 0;
    std::string name;
    Base base = NONE;

    Node(Node *par, Node *l, Node *r,
         const long double lcd, const long double rcd, std::string name, const Base b) : parent(par), left(l), right(r),
        left_child_distance(lcd),
        right_child_distance(rcd), name(std::move(name)),
        base(b) {
    }

    explicit Node(std::string name) : name(std::move(name)) {
    }

    Node() = default;

    void add_child(Node *child, long double distance);
};

class Tree {
    Node *root = nullptr;

public:
    void set_root(Node *new_root) {
        root = new_root;
    }

    static long double jukes_cantor_probability(Base from, Base to, long double alpha, long double t);
};


std::istream &operator>>(std::istream &is, Tree &tree);
