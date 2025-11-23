//
// Created by andru_kfgnofp on 11/22/2025.
//

#include "Tree.h"

#include <cmath>

long double Tree::jukes_cantor_probability(const base from, const base to,
                                           const long double alpha, const long double t) {
    if (from == to) {
        return (1 + 3 * std::exp(-4.0 / 3.0 * alpha * t)) / 4.0;
    }
    return 1 - std::exp(-4.0 / 3.0 * alpha * t);
}
