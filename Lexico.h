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

class Lexico {
public:
    Lexico(int argc, char* argv[]);
    ~Lexico();
    void run();

private:
    int scan();
    void show(int token);
    int isReserved();

    FILE* file;
    char lexeme[80];
    std::map<std::string, std::string> pseudocodeToCpp; // Mapa para palabras clave
};

#endif // LEXICO_H
