//
// Created by andru_kfgnofp on 11/23/2025.
//
#pragma once

#include <map>
#include <string>
#include "Tree.h"
#include "alignment.h"

// TODO: change to false
constexpr bool DEBUG = true;

probability felsenstein(Tree &t, const std::map<std::string, Base> &alignment_col);

probability single_col_felsenstein(Tree &t, probability alpha, const std::map<std::string, Base> &alignment_col);

log_prob sequence_alignment_felsenstein(Tree &t, const alignment &a, probability alpha);

std::pair<log_prob, probability> get_optimal_alpha(Tree &t, const alignment &a);

std::vector<std::pair<log_prob, probability> > get_intervals_alphas(Tree &t, const alignment &a,
                                                                    std::size_t interval_length);
