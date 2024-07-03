#ifndef ANALIZADORLEXICO_H
#define ANALIZADORLEXICO_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

enum class TokenType {
    Keyword, Identifier, Number, String, Operator, Separator, Comment, EndOfFile, Error
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
};

class AnalizadorLexico {
public:
    AnalizadorLexico(const std::string& source);

    std::vector<Token> tokenize();

private:
    std::string source;
    size_t position;
    int line;
    int column;

    void advance();

    void tokenizeComment(std::vector<Token>& tokens);
    void tokenizeIdentifierOrKeyword(std::vector<Token>& tokens);
    void tokenizeNumber(std::vector<Token>& tokens);
    void tokenizeString(std::vector<Token>& tokens);
    void tokenizeOperator(std::vector<Token>& tokens);
    void tokenizeSeparator(std::vector<Token>& tokens);

    bool isKeyword(const std::string& value);
    bool isOperator(char ch);
    bool isSeparator(char ch);
};

#endif
