//
// Created by andru_kfgnofp on 11/22/2025.
//

#include "Tree.h"

void Node::add_child(Node *child, const long double distance) {
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

long double Tree::jukes_cantor_probability(const Base from, const Base to,
                                           const long double alpha, const long double t) {
    if (from == to) {
        return (1 + 3 * std::exp(-4.0 / 3.0 * alpha * t)) / 4.0;
    }
    return 1 - std::exp(-4.0 / 3.0 * alpha * t);
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
        long double dist;
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
