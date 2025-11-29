//
// Created by andru_kfgnofp on 11/23/2025.
//
#pragma once

#include <map>
#include <string>
#include "tree.h"
#include "alignment.h"


probability felsenstein(tree &t, const std::map<std::string, Base> &alignment_col);

probability single_col_felsenstein(tree &t, probability alpha, const std::map<std::string, Base> &alignment_col);

log_prob sequence_alignment_felsenstein(tree &t, const alignment &a, probability alpha);

std::pair<log_prob, probability> get_optimal_alpha(tree &t, const alignment &a);

std::vector<std::pair<log_prob, probability> > get_intervals_alphas(tree &t, const alignment &a,
                                                                    std::size_t interval_length);
