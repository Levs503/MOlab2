//
// Created by Лев on 13.10.2020.
//
#include "iostream"
#include "string"
#include "MySimplex.h"
using std::cin;
using std::cout;
using std::endl;
using std::string;

void CreateDualTask(double *&CoeffOfFunction, double **&matrix, int& line, int& column, double *&svobodCoeff, std::string& choos) {
    std::swap(svobodCoeff, CoeffOfFunction);
    double **newMatrix = new double *[column];
    for (size_t g = 0; g < column; g++) {
        svobodCoeff[g]*=-1;
        newMatrix[g] = new double[line];
        for (size_t k = 0; k < line; k++) {
            newMatrix[g][k] = -matrix[k][g];
        }
    }
    for (size_t y = 0; y < line; y++) {
        delete [] matrix[y];
    }
    delete [] matrix;
    matrix = newMatrix;
    std::swap(line, column);
    if(choos == "max") choos = "min";
    else choos = "max";
}

Answer dualTaskSimplex() {
    cout << "min or max?" << endl;
    string choos;
    cin >> choos;
    bool flag = true;
    while (flag) {
        if (choos == "max") {
            flag = false;
        } else if (choos != "min") {
            cout << "wrong!!! enter min or max again" << endl;
            cin >> choos;
        } else
            flag = false;
    }
    int numberCoeff;
    cout << "enter number of coefficients of the objective function\n(don't forget variables with a coefficient of 0)" << endl;
    cin >> numberCoeff;
    flag = true;
    while (flag) {
        if (numberCoeff > 0) flag = false;
        else {
            cout << "wrong!!! number of coefficients < 0, enter again";
            cin >> numberCoeff;
        }
    }

    double *CoeffOfFunction = new double[numberCoeff];

    cout << "enter coefficients of the objective function" << endl;
    for (size_t k = 0; k < numberCoeff; k++) {
        double coeff;
        cin >> coeff;
        CoeffOfFunction[k] = coeff;
    }

    cout << "please bring system to the form:\n"
            "a11 * x1+...+a1n * xn<=b1\n"
            "a21 * x1+...+a2n * xn<=b2\n"
            "...........................\n"
            "am1 * x1+...+amn * xn<=bm\n\n";
    cout << "enter size of constraint matrix, for example matrix size above is m n" << endl;
    int line, column;
    cin >> line >> column;
    flag = true;
    while (flag) {
        if (line > 0 && column > 0) flag = false;
        else {
            cout << "wrong!!! line or column < 0, enter again";
            cin >> line >> column;
        }
    }
    double **matrix = new double *[line];
    cout << "enter coefficient of matrix: a11, a12, ... , amn" << endl;
    for (size_t k = 0; k < line; k++) {
        matrix[k] = new double[column + 1];
        for (size_t l = 0; l < column; l++) {
            double a;
            cin >> a;
            matrix[k][l] = a;
        }
    }

    double *svobodCoeff = new double[line];
    cout << "enter " << line << " free member of the inequality system" << endl;
    for (size_t k = 0; k < line; k++) {
        double coeff;
        cin >> coeff;
        svobodCoeff[k] = coeff;
    }

    CreateDualTask(CoeffOfFunction, matrix, line, column, svobodCoeff, choos);
    Answer ans = Simplex(choos, CoeffOfFunction, matrix, line, column, svobodCoeff);
    return ans;
}

int main(){
    Answer ans = dualTaskSimplex();
    cout<<ans;
}
