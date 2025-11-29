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

probability felsenstein(const Tree &t, const std::map<std::string, Base> &alignment_col);

probability single_col_felsenstein(const Tree &t, probability alpha, const std::map<std::string, Base> &alignment_col);

log_prob sequence_alignment_felsenstein(const Tree &t, const alignment &a);

probability get_optimal_alpha(const Tree &t, const alignment &a);
