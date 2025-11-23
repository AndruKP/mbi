//
// Created by andru_kfgnofp on 11/23/2025.
//

#include "felsenstein.h"

constexpr probability PROBABILITY_UNKNOWN_LEAF = 1;
const std::map<const Base, probability> BASE_FREQUENCIES = {
    {A, 1.0 / 4.0}, {C, 1.0 / 4.0}, {G, 1.0 / 4.0}, {T, 1.0 / 4.0}
};


probability felsenstein(const Tree t, const probability alpha, const std::map<std::string, Base> &alignment_col) {
    t.set_leaves_bases(alignment_col);
    std::map<std::pair<const Node *, Base>, probability> A;

    const auto order = t.get_postorder();
    const std::vector bases = {Base::A, Base::C, Base::G, Base::T};

    for (const auto &vertex: order) {
        for (auto base: bases) {
            if (vertex->isLeaf()) {
                if (vertex->base == N) {
                    A[{vertex, base}] = PROBABILITY_UNKNOWN_LEAF;
                } else {
                    A[{vertex, base}] = base == vertex->base;
                }
                continue;
            }
            probability prob_left = 0, prob_right = 0;
            auto left_node = vertex->left;
            auto right_node = vertex->right;
            for (const auto &sum_base: bases) {
                prob_left += A[{left_node, sum_base}] * Tree::jukes_cantor_probability(
                    base, sum_base, alpha, vertex->left_child_distance);
                prob_right += A[{right_node, sum_base}] * Tree::jukes_cantor_probability(
                    base, sum_base, alpha, vertex->right_child_distance);;
            }
            A[{vertex, base}] = prob_left * prob_right;
        }
    }
    probability result = 0;
    const Node *root = t.get_root();
    for (auto &base: bases) {
        result += BASE_FREQUENCIES.at(base) * A.at({root, base});
    }
    return result;
}
