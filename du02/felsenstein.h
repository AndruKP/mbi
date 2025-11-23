//
// Created by andru_kfgnofp on 11/23/2025.
//
#pragma once

#include <map>
#include <string>
#include "Tree.h"


probability felsenstein(Tree t, probability alpha, const std::map<std::string, Base> &alignment_col);

