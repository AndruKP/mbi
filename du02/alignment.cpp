//
// Created by andru_kfgnofp on 11/23/2025.
//

#include "alignment.h"

std::map<std::string, std::string> alignment::get_map() const {
    return alignment_seq;
}

std::vector<alignment> alignment::split_alignment(const std::size_t interval_length) const {
    std::vector<alignment> result;
    for (std::size_t i = 0; i < seq_length(); i += interval_length) {
        alignment sub;
        for (const auto &[name, seq]: alignment_seq) {
            sub.add_seq(name, seq.substr(i, interval_length));
        }
        result.push_back(sub);
    }
    return result;
}

std::istream &operator>>(std::istream &is, alignment &a) {
    std::string line;

    std::string curr_sequence;
    std::string curr_seq_name;

    while (std::getline(is, line)) {
        if (line.empty()) {
            continue;
        }

        if (line[0] == '>') {
            if (!curr_sequence.empty()) {
                a.add_seq(curr_seq_name, curr_sequence);
                curr_sequence.clear();
            }
            std::istringstream iss(line);
            char greater;
            iss >> greater >> curr_seq_name;
        } else {
            curr_sequence += line;
        }
    }

    if (!curr_sequence.empty()) {
        a.add_seq(curr_seq_name, curr_sequence);
    }

    return is;
}
