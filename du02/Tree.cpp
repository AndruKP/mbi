//
// Created by andru_kfgnofp on 11/22/2025.
//

#include "Tree.h"

Base get_base(const char c) {
    if (!CHAR_TO_BASE.contains(c)) {
        throw std::logic_error("Wrong base character");
    }
    return CHAR_TO_BASE.at(c);
}

void Node::add_child(Node *child, const branchLength distance) {
    if (left == nullptr) {
        left = child;
        left_child_distance = distance;
    } else if (right == nullptr) {
        right = child;
        right_child_distance = distance;
    } else {
        throw NodeException("Both children are already defined");
    }
}

bool Node::isLeaf() const {
    return left == nullptr && right == nullptr;
}

std::vector<Node *> Node::get_leaves() {
    if (isLeaf()) {
        return {this};
    }
    std::vector<Node *> l, r;
    if (left != nullptr) {
        l = left->get_leaves();
    }
    if (right != nullptr) {
        r = right->get_leaves();
    }
    l.insert(l.end(), r.begin(), r.end());
    return l;
}

std::vector<Node *> Node::get_postorder() {
    std::vector<Node *> l, r;
    if (left != nullptr) {
        l = left->get_postorder();
    }
    if (right != nullptr) {
        r = right->get_postorder();
    }
    l.insert(l.end(), r.begin(), r.end());
    l.insert(l.end(), {this});
    return l;
}

void Node::precalculate_jd69_matrix(const probability alpha) {
    if (isLeaf()) return;
    for (unsigned int i = 0; i < NUM_BASES; i++) {
        for (unsigned int j = 0; i < NUM_BASES; i++) {
            left_child_matrix[i][j] = Tree::jukes_cantor_probability(BASES[i], BASES[j], alpha, left_child_distance);
            right_child_matrix[i][j] = Tree::jukes_cantor_probability(BASES[i], BASES[j], alpha, right_child_distance);
        }
    }
    if (left != nullptr) {
        left->precalculate_jd69_matrix(alpha);
    }
    if (right != nullptr) {
        right->precalculate_jd69_matrix(alpha);
    }
}

void Tree::precalculate_jd69_matrix(const probability alpha) const {
    root->precalculate_jd69_matrix(alpha);
}

probability Tree::jukes_cantor_probability(const Base from, const Base to,
                                           const probability alpha, const branchLength t) {
    if (from == to) {
        return (1 + 3 * std::exp(-4.0 / 3.0 * alpha * t)) / 4.0;
    }
    return (1 - std::exp(-4.0 / 3.0 * alpha * t)) / 4.0;
}

void Tree::set_leaves_bases(std::map<std::string, Base> alignment_col) const {
    const auto leaves = get_leaves();
    for (const auto leaf: leaves) {
        leaf->base = alignment_col.at(leaf->name);
        alignment_col.erase(leaf->name);
    }
    if (!alignment_col.empty()) {
        throw std::logic_error("Some alignment's columns aren't used");
    }
}

std::vector<Node *> Tree::get_leaves() const {
    return root->get_leaves();
}

std::vector<Node *> Tree::get_postorder() const {
    return root->get_postorder();
}

std::istream &operator>>(std::istream &is, Tree &tree) {
    std::string line;

    std::map<std::string, Node *> nodes;

    while (std::getline(is, line)) {
        if (line.empty()) {
            break;
        }

        std::istringstream iss(line);
        std::string child_name, parent_name;
        branchLength dist;
        iss >> child_name >> parent_name >> dist;

        if (!nodes.contains(parent_name)) {
            auto node = new Node(parent_name);
            nodes.insert({parent_name, node});
        }
        if (!nodes.contains(child_name)) {
            auto node = new Node(child_name);
            nodes.insert({child_name, node});
        }
        nodes[parent_name]->add_child(nodes[child_name], dist);
    }
    if (!nodes.contains(ROOT_NAME)) {
        throw std::logic_error("Root isn't found in file");
    }
    const auto root = nodes[ROOT_NAME];
    tree.set_root(root);

    return is;
}
