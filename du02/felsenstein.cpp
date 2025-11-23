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

log_prob sequence_alignment_felsenstein(const Tree t, const probability alpha, const alignment &a) {
    log_prob result = 0;

    for (unsigned long i = 0; i < a.seq_length(); i++) {
        if (i % 1000 == 0) {
            std::cout << i << std::endl;
        }
        std::map<std::string, Base> one_base_alignment;
        for (auto &[name, seq]: a.get_map()) {
            one_base_alignment[name] = get_base(seq[i]);
        }
        result += std::log(felsenstein(t, alpha, one_base_alignment));
    }
    return result;
}

probability get_optimal_alpha(const Tree t, const alignment &a) {
    log_prob max_log_prob = 0;
    probability max_arg_alpha = 0;
    for (int iter = 1; iter <= 20; iter++) {
        std::cout << iter << std::endl;

        const probability alpha = iter / 10.0;
        const log_prob curr_lp = sequence_alignment_felsenstein(t, alpha, a);

        if (max_arg_alpha == 0 || max_log_prob < curr_lp) {
            max_log_prob = curr_lp;
            max_arg_alpha = alpha;
        }
    }
    return max_arg_alpha;
}
