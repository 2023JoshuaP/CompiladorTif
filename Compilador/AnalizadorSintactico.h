#ifndef ANALIZADORSINTACTICO_H
#define ANALIZADORSINTACTICO_H

#include <iostream>
#include <vector>
#include <string>
#include "./AnalizadorLexico.h"
using namespace std;

class AnalizadorSintactico {
    public:
        AnalizadorSintactico(const vector<Token>& tokens);
        void parse();
    private:
        std::vector<Token> tokens;
        size_t position;

        Token currentToken();
        void advance();

        void match(TokenType type);
        void error(const std::string& message);

        void parsePseudoCode();
        void parseNombreAlgoritmo();
        void parseCuerpo();
        void parseDeclaracion();
        void parseDeclaraVariable();
        void parseTipo();
        void parseAsignacion();
        void parseOperadorAsignacion();
        void parseLectura();
        void parseImpresion();
        void parseDecision();
        void parseDecisionSimple();
        void parseDecisionMultiple();
        void parseRepeticion();
        void parseCicloPara();
        void parseInicializacionPara();
        void parseAsignacionPara();
        void parseOperadorAutoincremento();
        void parseCicloMientras();
        void parseCicloHagaMientras();
        void parseExpresion();
        void parseExpresionEntera();
        void parseExpresionReal();
        void parseExpresionCadena();
        void parseExpresionBooleana();
        void parseExpresionCaracter();
        void parseVariable();
        void parseIdentificador();
};

#endif