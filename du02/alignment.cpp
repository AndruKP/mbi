//
// Created by andru_kfgnofp on 11/23/2025.
//

#include "alignment.h"

std::map<std::string, std::string> alignment::get_map() const {
    return alignment_seq;
}

std::istream &operator>>(std::istream &is, alignment &a) {
    std::string line;

    std::string currSequence;
    std::string currSeqName;

    while (std::getline(is, line)) {
        if (line.empty()) {
            continue;
        }

        if (line[0] == '>') {
            if (!currSequence.empty()) {
                a.add_seq(currSeqName, currSequence);
                currSequence.clear();
            }
            std::istringstream iss(line);
            char greater;
            iss >> greater >> currSeqName;
        } else {
            currSequence += line;
        }
    }

    if (!currSequence.empty()) {
        a.add_seq(currSeqName, currSequence);
    }

    return is;
}
