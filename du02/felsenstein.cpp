//
// Created by andru_kfgnofp on 11/23/2025.
//

#include "felsenstein.h"

#include <unordered_map>

constexpr probability PROBABILITY_UNKNOWN_LEAF = 1;
const std::map<const Base, probability> BASE_FREQUENCIES = {
    {A, 1.0 / 4.0}, {C, 1.0 / 4.0}, {G, 1.0 / 4.0}, {T, 1.0 / 4.0}
};

probability felsenstein(tree &t, const std::map<std::string, Base> &alignment_col) {
    t.set_leaves_bases(alignment_col);

    const auto order = t.get_postorder();
    std::vector<std::array<probability, NUM_BASES> > dp_table;
    dp_table.reserve(order.size());

    for (auto vertex_idx = 0; vertex_idx < order.size(); vertex_idx++) {
        for (const auto &base: BASES) {
            const auto vertex = order[vertex_idx];

            if (vertex->is_leaf()) {
                if (vertex->base == N) {
                    dp_table[vertex_idx][base] = PROBABILITY_UNKNOWN_LEAF;
                } else {
                    dp_table[vertex_idx][base] = base == vertex->base;
                }
                continue;
            }

            probability prob_left = 0, prob_right = 0;

            const auto left_node = vertex->left;
            const auto right_node = vertex->right;

            for (const auto &second_base: BASES) {
                prob_left += dp_table[left_node->idx][second_base] * vertex->left_child_matrix[base][second_base];
                prob_right += dp_table[right_node->idx][second_base] * vertex->right_child_matrix[base][second_base];
            }
            dp_table[vertex_idx][base] = prob_left * prob_right;
        }
    }

    probability result = 0;

    const node *root = t.get_root();
    for (auto &base: BASES) {
        result += BASE_FREQUENCIES.at(base) * dp_table[root->idx][base];
    }
    return result;
}

probability single_col_felsenstein(tree &t, const probability alpha,
                                   const std::map<std::string, Base> &alignment_col) {
    t.precalculate_jd69_matrix(alpha);
    return felsenstein(t, alignment_col);
}

log_prob sequence_alignment_felsenstein(tree &t, const alignment &a, const probability alpha) {
    t.precalculate_jd69_matrix(alpha);
    log_prob result = 0;

    auto m = a.get_map();
    for (unsigned long i = 0; i < a.seq_length(); i++) {
        std::map<std::string, Base> one_base_alignment;

        for (auto &[name, seq]: m) {
            one_base_alignment[name] = get_base(seq[i]);
        }
        result += std::log(felsenstein(t, one_base_alignment));
    }
    return result;
}

std::pair<log_prob, probability> get_optimal_alpha(tree &t, const alignment &a) {
    constexpr int NUM_ITER = 20;

    log_prob max_log_prob = 0;
    probability max_arg_alpha = 0;

    for (int iter = 1; iter <= NUM_ITER; iter++) {
        const probability alpha = iter / 10.0;
        const log_prob curr_lp = sequence_alignment_felsenstein(t, a, alpha);

        if (max_arg_alpha == 0 || max_log_prob < curr_lp) {
            max_log_prob = curr_lp;
            max_arg_alpha = alpha;
        }
    }
    return {max_log_prob, max_arg_alpha};
}

std::vector<std::pair<log_prob, probability> > get_intervals_alphas(tree &t, const alignment &a,
                                                                    const std::size_t interval_length) {
    const auto alignments = a.split_alignment(interval_length);
    std::vector<std::pair<log_prob, probability> > results;

    for (const auto &sub_alignment: alignments) {
        results.push_back(get_optimal_alpha(t, sub_alignment));
    }
    return results;
}
