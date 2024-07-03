#include "AnalizadorLexico.h"

AnalizadorLexico::AnalizadorLexico(const string& source) : source(source), position(0), line(1), column(1) {}

vector<Token> AnalizadorLexico::tokenize() {
    vector<Token> tokens;
    while (position < source.size()) {
        if (isspace(source[position])) {
            advance();
        }
        else if (source[position] == '/' && source[position + 1] == '/') {
            tokenizeComment(tokens);
        }
        else if (isalpha(source[position]) || source[position] == '_') {
            tokenizeIdentifierOrKeyword(tokens);
        }
        else if (isdigit(source[position])) {
            tokenizeNumber(tokens);
        }
        else if (source[position] == '"') {
            tokenizeString(tokens);
        }
        else if (isOperator(source[position])) {
            tokenizeOperator(tokens);
        }
        else if (isSeparator(source[position])) {
            tokenizeSeparator(tokens);
        }
        else {
            tokens.push_back({TokenType::Error, string(1, source[position]), line, column});
            advance();
        }
    }
    tokens.push_back({TokenType::EndOfFile, "", line, column});
    return tokens;
}

void AnalizadorLexico::advance() {
    if (source[position] == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    position++;
}

void AnalizadorLexico::tokenizeComment(vector<Token>& tokens) {
    size_t start = position;
    while (position < source.size() && source[position] != '\n') {
        position++;
    }
    tokens.push_back({TokenType::Comment, source.substr(start, position - start), line, column});
}

void AnalizadorLexico::tokenizeIdentifierOrKeyword(vector<Token>& tokens) {
    size_t start = position;
    while (position < source.size() && (isalnum(source[position]) || source[position] == '_')) {
        position++;
    }
    std::string value = source.substr(start, position - start);
    TokenType type = isKeyword(value) ? TokenType::Keyword : TokenType::Identifier;
    tokens.push_back({type, value, line, column});
}

void AnalizadorLexico::tokenizeNumber(vector<Token>& tokens) {
    size_t start = position;
    while (position < source.size() && isdigit(source[position])) {
        position++;
    }
    if (position < source.size() && source[position] == '.') {
        position++;
        while (position < source.size() && isdigit(source[position])) {
            position++;
        }
    }
    tokens.push_back({TokenType::Number, source.substr(start, position - start), line, column});
}

void AnalizadorLexico::tokenizeString(vector<Token>& tokens) {
    size_t start = position;
    position++;
    while (position < source.size() && source[position] != '"') {
        position++;
    }
    if (position < source.size()) {
        position++;
    }
    tokens.push_back({TokenType::String, source.substr(start, position - start), line, column});
}

void AnalizadorLexico::tokenizeOperator(vector<Token>& tokens) {
    size_t start = position;
    while (position < source.size() && isOperator(source[position])) {
        position++;
    }
    tokens.push_back({TokenType::Operator, source.substr(start, position - start), line, column});
}

void AnalizadorLexico::tokenizeSeparator(vector<Token>& tokens) {
    tokens.push_back({TokenType::Separator, std::string(1, source[position]), line, column});
    advance();
}

bool AnalizadorLexico::isKeyword(const std::string& value) {
    static const std::vector<std::string> keywords = {
        "algoritmo", "principal", "entero", "real", "cadena", "booleano", "caracter",
        "leer", "imprimir", "si", "sino", "segun", "caso", "termina", "para", "mientras", "haga"
    };
    return std::find(keywords.begin(), keywords.end(), value) != keywords.end();
}

bool AnalizadorLexico::isOperator(char ch) {
    static const std::string operators = "=+-*/%<>";
    return operators.find(ch) != std::string::npos;
}

bool AnalizadorLexico::isSeparator(char ch) {
    static const std::string separators = "{}[](),;";
    return separators.find(ch) != std::string::npos;
}
