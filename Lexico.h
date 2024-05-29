#ifndef LEXICO_H
#define LEXICO_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <map>
#include <string>

#define MAYOR '>'
#define MENOR '<'
#define IGUAL '='
#define NOT '!'
#define PUNTOYCOMA ';'
#define COMA ','
#define PARI '('
#define PARD ')'
#define CORI '['
#define CORD ']'
#define LLAVEI '{'
#define LLAVED '}'
#define ID 256
#define NUM 257
#define ASIGNACION 258
using namespace std;

class Lexico {
    public:
        Lexico(int argc, char* argv[]);
        ~Lexico();
        void run();
    private:
        int scanner();
        void mostrar(int token);
        int isReserved();

        FILE* file;
        char lexema[80];
        map<string, string> pseudocodeToCpp;
};

#endif