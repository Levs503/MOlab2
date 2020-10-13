//
// Created by Лев on 21.09.2020.
//

#ifndef LABMO1SIMPLEX_ANSWER_H
#define LABMO1SIMPLEX_ANSWER_H
#pragma once
#include <ostream>
#include <vector>

struct Answer {
    double extremum;
    std::vector<double> values;
};

std::ostream& operator<<(std::ostream& out, Answer& ans);


#endif//LABMO1SIMPLEX_ANSWER_H
