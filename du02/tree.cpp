//
// Created by andru_kfgnofp on 11/22/2025.
//

#include "tree.h"

Base get_base(const char c) {
    if (!CHAR_TO_BASE.contains(c)) {
        throw std::logic_error("Wrong base character");
    }
    return CHAR_TO_BASE.at(c);
}

void node::add_child(node *child, const branch_length distance) {
    if (left == nullptr) {
        left = child;
        left_child_distance = distance;
    } else if (right == nullptr) {
        right = child;
        right_child_distance = distance;
    } else {
        throw node_exception("Both children are already defined");
    }
}

bool node::is_leaf() const {
    return left == nullptr && right == nullptr;
}

void node::get_leaves(std::vector<node *> &result) {
    if (is_leaf()) {
        result.push_back({this});
        return;
    }
    if (left != nullptr) {
        left->get_leaves(result);
    }
    if (right != nullptr) {
        right->get_leaves(result);
    }
}

void node::get_postorder(std::vector<node *> &result) {
    if (left != nullptr) {
        left->get_postorder(result);
    }
    if (right != nullptr) {
        right->get_postorder(result);
    }
    idx = result.size();
    result.push_back(this);
}

void node::precalculate_jd69_matrix(const probability alpha) {
    if (is_leaf()) return;

    for (unsigned int i = 0; i < NUM_BASES; i++) {
        for (unsigned int j = 0; j < NUM_BASES; j++) {
            left_child_matrix[i][j] = tree::jukes_cantor_probability(BASES[i], BASES[j], alpha, left_child_distance);
            right_child_matrix[i][j] = tree::jukes_cantor_probability(BASES[i], BASES[j], alpha, right_child_distance);
        }
    }

    if (left != nullptr) {
        left->precalculate_jd69_matrix(alpha);
    }
    if (right != nullptr) {
        right->precalculate_jd69_matrix(alpha);
    }
}

void tree::precalculate_jd69_matrix(const probability alpha) const {
    root->precalculate_jd69_matrix(alpha);
}

probability tree::jukes_cantor_probability(const Base from, const Base to,
                                           const probability alpha, const branch_length t) {
    if (from == to) {
        return (1 + 3 * std::exp(-4.0 / 3.0 * alpha * t)) / 4.0;
    }
    return (1 - std::exp(-4.0 / 3.0 * alpha * t)) / 4.0;
}

void tree::set_leaves_bases(std::map<std::string, Base> alignment_col) {
    for (const auto leaf: get_leaves()) {
        leaf->base = alignment_col.at(leaf->name);
        alignment_col.erase(leaf->name);
    }

    if (!alignment_col.empty()) {
        throw std::logic_error("Some alignment's columns aren't used");
    }
}

std::vector<node *> tree::get_leaves() {
    if (leaves.empty()) {
        root->get_leaves(leaves);
    }
    return leaves;
}

std::vector<node *> tree::get_postorder() const {
    std::vector<node *> result;
    root->get_postorder(result);
    return result;
}

std::istream &operator>>(std::istream &is, tree &tree) {
    std::string line;

    std::map<std::string, node *> nodes;

    while (std::getline(is, line)) {
        if (line.empty()) {
            break;
        }

        std::istringstream iss(line);
        std::string child_name, parent_name;
        branch_length dist;
        iss >> child_name >> parent_name >> dist;

        if (!nodes.contains(parent_name)) {
            auto parent_node = new node(parent_name);
            nodes.insert({parent_name, parent_node});
        }
        if (!nodes.contains(child_name)) {
            auto child_node = new node(child_name);
            nodes.insert({child_name, child_node});
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
