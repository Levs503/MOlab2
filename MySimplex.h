//
// Created by Лев on 13.10.2020.
//

#ifndef LAB02MO_MYSIMPLEX_H
#define LAB02MO_MYSIMPLEX_H
#pragma once
#include "Answer.h"
Answer Simplex();
Answer dualTaskSimplex();
Answer Simplex(std::string& choos, double *&CoeffOfFunction, double **&matrix, int line, int column,  double *&svobodCoeff);

#endif//LAB02MO_MYSIMPLEX_H
