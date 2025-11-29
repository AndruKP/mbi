//
// Created by andru_kfgnofp on 11/23/2025.
//

#include "felsenstein.h"

#include <unordered_map>

constexpr probability PROBABILITY_UNKNOWN_LEAF = 1;
const std::map<const Base, probability> BASE_FREQUENCIES = {
    {A, 1.0 / 4.0}, {C, 1.0 / 4.0}, {G, 1.0 / 4.0}, {T, 1.0 / 4.0}
};

struct felsHash {
    std::size_t operator()(const std::pair<const Node*, const Base>& key) const noexcept {
        const std::size_t h1 = std::hash<const Node*>{}(key.first);
        const std::size_t h2 = std::hash<std::underlying_type_t<Base>>{}(key.second);

        return h1 ^ h2; //+ 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2);
    }
};



probability felsenstein(const Tree &t, const std::map<std::string, Base> &alignment_col) {
    t.set_leaves_bases(alignment_col);
    std::unordered_map<std::pair<const Node *, Base>, probability, felsHash> A;

    const auto order = t.get_postorder();

    for (const auto &vertex: order) {
        for (unsigned int first_base_idx = 0; first_base_idx < NUM_BASES; first_base_idx++) {
            auto base = BASES[first_base_idx];
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
            for (unsigned int second_base_idx = 0; second_base_idx < NUM_BASES; second_base_idx++) {
                auto sum_base = BASES[second_base_idx];
                prob_left += A[{left_node, sum_base}] * vertex->left_child_matrix[first_base_idx][second_base_idx];
                prob_right += A[{right_node, sum_base}] * vertex->right_child_matrix[first_base_idx][second_base_idx];
            }
            A[{vertex, base}] = prob_left * prob_right;
        }
    }
    probability result = 0;
    const Node *root = t.get_root();
    for (auto &base: BASES) {
        result += BASE_FREQUENCIES.at(base) * A[{root, base}];
    }
    return result;
}

probability single_col_felsenstein(const Tree &t, const probability alpha,
                                   const std::map<std::string, Base> &alignment_col) {
    t.precalculate_jd69_matrix(alpha);
    return felsenstein(t, alignment_col);
}

log_prob sequence_alignment_felsenstein(const Tree &t, const alignment &a) {
    log_prob result = 0;

    auto m = a.get_map();
    for (unsigned long i = 0; i < a.seq_length(); i++) {
        if (DEBUG && i % 1000 == 0) {
            std::cout << i << std::endl;
        }
        std::map<std::string, Base> one_base_alignment;
        for (auto &[name, seq]: m) {
            one_base_alignment[name] = get_base(seq[i]);
        }
        result += std::log(felsenstein(t, one_base_alignment));
    }
    return result;
}

probability get_optimal_alpha(const Tree &t, const alignment &a) {
    log_prob max_log_prob = 0;
    probability max_arg_alpha = 0;
    // TODO: change to 20
    constexpr int NUM_ITER = 1;
    for (int iter = 1; iter <= NUM_ITER; iter++) {
        std::cout << iter << std::endl;

        const probability alpha = iter / 10.0;
        t.precalculate_jd69_matrix(alpha);
        const log_prob curr_lp = sequence_alignment_felsenstein(t, a);

        if (max_arg_alpha == 0 || max_log_prob < curr_lp) {
            max_log_prob = curr_lp;
            max_arg_alpha = alpha;
        }
    }
    return max_arg_alpha;
}
