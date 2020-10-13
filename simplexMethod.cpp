#include "Answer.h"
#include <array>
#include <iostream>
#include <list>
#include <map>
#include <string>


using std::cin;
using std::cout;
using std::endl;
using std::string;


void CreateSimplexTable(int line, int column, double **&simplexTable, double **&matrix, double *&coeffOfFunc, double *&svobodCoef) {
    simplexTable = new double *[line];

    for (size_t k = 0; k < line; k++) {
        simplexTable[k] = new double[column];
    }
    for (size_t k = 0; k < line - 1; k++) {
        for (size_t g = 0; g < column; g++) {
            if (g == 0)
                simplexTable[k][0] = svobodCoef[k];
            else {
                simplexTable[k][g] = matrix[k][g - 1];
            }
        }
    }

    for (size_t k = 0; k < column; k++) {
        if (k == 0)
            simplexTable[line - 1][k] = 0;
        else
            simplexTable[line - 1][k] = -coeffOfFunc[k - 1];
    }

    for (size_t k = 0; k < line; k++) {
        for (size_t h = 0; h < column; h++) {
            cout << simplexTable[k][h] << " ";
        }
        cout << endl;
    }
}

void transformSimplexTable(double **&simplexTable, int basPos, int freePos, int line, int column) {// line-базисная, column-свободная
    for (auto k = 0; k < line; k++) {
        if (k == basPos) continue;
        for (auto y = 0; y < column; y++) {
            if (y == freePos) continue;
            simplexTable[k][y] = simplexTable[k][y] - simplexTable[k][freePos] * simplexTable[basPos][y] / simplexTable[basPos][freePos];
        }
    }
    simplexTable[basPos][freePos] = 1 / simplexTable[basPos][freePos];
    for (auto d = 0; d < column; d++) {
        if (d != freePos)
            simplexTable[basPos][d] *= simplexTable[basPos][freePos];
    }
    for (auto h = 0; h < line; h++) {
        if (h != basPos)
            simplexTable[h][freePos] *= (-simplexTable[basPos][freePos]);
    }
}

bool CheckFuncCoeff(double **&simplexTable, int column, int line) {
    for (size_t k = 1; k < column; k++) {
        if (simplexTable[line - 1][k] > 0) return false;
    }
    return true;
}

size_t FindPositivePosInLastLine(double **&simplexTable, int column, int line) {
    for (size_t k = 1; k < column; k++) {
        if (simplexTable[line - 1][k] > 0) return k;
    }
    return -1;
}

bool CheckColumn(double **&simplexTable, int column, int line) {
    for (size_t r = 0; r < line; r++) {
        if (simplexTable[r][column] > 0) return false;
    }
    return true;
}

size_t FindMinimumPositiveElement(double **&simplexTable, int column, int line) {
    std::map<double, size_t> position;
    for (size_t r = 0; r < line - 1; r++) {
        if (simplexTable[r][column] > 0) position.insert({simplexTable[r][0] / simplexTable[r][column], r});
    }
    return (*position.begin()).second;
}
Answer CreateAnswer(double **&simplexTable, std::map<int, int> &basis, std::map<int, int> &free, int line) {
    Answer ret;
    ret.extremum = simplexTable[line - 1][0];
    std::map<int, double> allVariables;

    for (auto &k : basis) {
        allVariables.insert({k.second, simplexTable[k.first][0]});
    }
    for (auto &k : free) {
        allVariables.insert({k.second, 0});
    }
    ret.values.resize(allVariables.size());
    int l = 0;
    for (auto &k : allVariables) {
        ret.values[l] = (k.second);
        l++;
    }
    return ret;
}

Answer FindExt(double **&simplexTable, std::map<int, int> &basis, std::map<int, int> &free, int line, int column) {
    while (!CheckFuncCoeff(simplexTable, column, line)) {

        size_t positiveColumnPos = FindPositivePosInLastLine(simplexTable, column, line);
        if (CheckColumn(simplexTable, positiveColumnPos, line)) throw std::logic_error("not limiting");
        size_t positiveLinePos = FindMinimumPositiveElement(simplexTable, positiveColumnPos, line);
        double save = basis[positiveLinePos];
        basis[positiveLinePos] = free[positiveColumnPos - 1];
        free[positiveColumnPos - 1] = save;
        transformSimplexTable(simplexTable, positiveLinePos, positiveColumnPos, line, column);
        /*for(size_t g =0; g< line; g++){
            for(size_t f =0; f<column; f++){
                std::cout<<simplexTable[g][f]<<" ";
            }
            std::cout<<std::endl;
        }
        std::cout<<std::endl;*/
    }

    Answer forReturn = CreateAnswer(simplexTable, basis, free, line);
    return forReturn;
}


Answer Simplex() {
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
    if (choos == "max") {
        for (size_t k = 0; k < numberCoeff; k++) {
            CoeffOfFunction[k] = -CoeffOfFunction[k];
        }
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
    double **simplexTable;
    CreateSimplexTable(line + 1, column + 1, simplexTable, matrix, CoeffOfFunction, svobodCoeff);
    std::map<int, int> basis;// строчка или столбец, идентификатор переменной
    std::map<int, int> free; // строчка или столбец, идентификатор переменной
    for (size_t k = 0; k < column; k++) {
        free.insert({k, k + 1});
    }
    for (size_t k = 0; k < line; k++) {
        basis.insert({k, column + k + 1});
    }

    Answer ret = FindExt(simplexTable, basis, free, line + 1, column + 1);
    if (choos == "max") ret.extremum *= -1;
    ret.values.resize(free.size());
    return ret;
}

Answer Simplex(std::string& choos, double *&CoeffOfFunction, double **&matrix, int line, int column,  double *&svobodCoeff) {
    cout<<"It S MATRIX";
    for(size_t g =0; g< line; g++){
        for(size_t f =0; f<column; f++){
            std::cout<<matrix[g][f]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    double **simplexTable;
    CreateSimplexTable(line + 1, column + 1, simplexTable, matrix, CoeffOfFunction, svobodCoeff);
    std::map<int, int> basis;// строчка или столбец, идентификатор переменной
    std::map<int, int> free; // строчка или столбец, идентификатор переменной
    for (size_t k = 0; k < column; k++) {
        free.insert({k, k + 1});
    }
    for (size_t k = 0; k < line; k++) {
        basis.insert({k, column + k + 1});
    }

    Answer ret = FindExt(simplexTable, basis, free, line + 1, column + 1);
    if (choos == "max") ret.extremum *= -1;
    ret.values.resize(free.size());
    return ret;
}
