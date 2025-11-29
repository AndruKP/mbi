//
// Created by andru_kfgnofp on 11/23/2025.
//
#pragma once
#include "Tree.h"

class alignment {
    unsigned long length_ = 0;
    bool is_empty = true;
    //name, seq
    std::map<std::string, std::string> alignment_seq;

public:
    void add_seq(const std::string &name, const std::string &seq) {
        if (alignment_seq.empty()) {
            length_ = seq.length();
        }
        alignment_seq[name] = seq;
    }

    [[nodiscard]] unsigned long seq_length() const { return length_; }
    [[nodiscard]] unsigned long length() const { return alignment_seq.size(); }

    [[nodiscard]] std::map<std::string, std::string> get_map() const;

    [[nodiscard]] std::vector<alignment> split_alignment(std::size_t interval_length) const ;
};

std::istream &operator>>(std::istream &is, alignment &a);
