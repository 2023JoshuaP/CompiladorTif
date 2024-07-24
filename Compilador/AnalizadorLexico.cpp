#include "AnalizadorLexico.h"

AnalizadorLexico::AnalizadorLexico(const string& source_) : source(source_), position(0), line(1), column(1) {}

vector<Token> AnalizadorLexico::tokenize() {
    vector<Token> tokens;
    while (position < source.size()) {
        if (isspace(source[position])) {
            advance();
        }
        else if (source[position] == '/' && source[position + 1] == '/') {
            tokenizeComment();
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
        else if (source[position] == '\''&& source[position + 2] == '\'') {
            tokenizeCharacter(tokens);
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

void AnalizadorLexico::tokenizeComment(){
    while (position < source.size() && source[position] != '\n') {
        position++;
    }
    advance();
}

void AnalizadorLexico::tokenizeIdentifierOrKeyword(vector<Token>& tokens) {
    size_t start = position;
    while (position < source.size() && (isalnum(source[position]) || source[position] == '_')) {
        position++;
    }
    string value = source.substr(start, position - start);
    TokenType type;
    if (isKeyword(value)) {
        type = TokenType::Keyword;
    } else if (isBoolean(value)) {
        type = TokenType::Boolean;
    } else {
        type = TokenType::Identifier;
    }
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
        tokens.push_back({TokenType::Real, source.substr(start, position - start), line, column});
    }else{
        tokens.push_back({TokenType::Integer, source.substr(start, position - start), line, column});
    }
}

void AnalizadorLexico::tokenizeString(vector<Token>& tokens) {
    size_t start = position+1;
    position++;
    while (position < source.size() && source[position] != '"') {
        if (source[position] == '\\' && position + 1 < source.size()) {
            position++; // Skip the escape character
        }
        position++;
    }
    if (position < source.size()) {
        position++;
        tokens.push_back({TokenType::String, "\\\"" + source.substr(start, position - start-1) + "\\\"", line, column});
    } else {
        tokens.push_back({TokenType::Error, source.substr(start, position - start), line, column});
    }
}

void AnalizadorLexico::tokenizeCharacter(vector<Token>& tokens) {
    size_t start = position;
    position++;
    if (position < source.size() && source[position] != '\'') {
        position++;
    }
    if (position < source.size() && source[position] == '\'') {
        position++;
        tokens.push_back({TokenType::Character, source.substr(start, position - start), line, column});
    } else {
        tokens.push_back({TokenType::Error, source.substr(start, position - start), line, column});
    }
}

void AnalizadorLexico::tokenizeOperator(vector<Token>& tokens) {
    size_t start = position;
    while (position < source.size() && isOperator(source[position])) {
        position++;
    }
    tokens.push_back({TokenType::Operator, source.substr(start, position - start), line, column});
}

void AnalizadorLexico::tokenizeSeparator(vector<Token>& tokens) {
    tokens.push_back({TokenType::Separator, string(1, source[position]), line, column});
    advance();
}

bool AnalizadorLexico::isKeyword(const string& value) {
    static const vector<string> keywords = {
        "algoritmo", "principal", "entero", "real", "cadena", "booleano", "caracter",
        "leer", "imprimir", "si", "sino", "segun", "caso", "termina", "para", "mientras",
        "haga", "otroCaso"
    };
    return find(keywords.begin(), keywords.end(), value) != keywords.end();
}
bool AnalizadorLexico::isBoolean(const string& value) {
    static const vector<string> booleans = {"true", "false"};
    return find(booleans.begin(), booleans.end(), value) != booleans.end();
}
bool AnalizadorLexico::isOperator(char ch) {
    static const string operators = "=+-*/%<>&|!";
    return operators.find(ch) != string::npos;
}

bool AnalizadorLexico::isSeparator(char ch) {
    static const string separators = "{}[](),;:'";
    return separators.find(ch) != string::npos;
}