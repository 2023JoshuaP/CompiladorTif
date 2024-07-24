#ifndef ANALIZADORLEXICO_H
#define ANALIZADORLEXICO_H
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;
enum class TokenType {
    Keyword, Identifier, Integer, Real, Boolean, Character, String, Operator, Separator, Comment, EndOfFile, Error
};

struct Token {
    TokenType type;
    string value;
    int line;
    int column;
};

class AnalizadorLexico {
public:
    AnalizadorLexico(const string& source);
    vector<Token> tokenize();
private:
    string source;
    size_t position;
    int line;
    int column;

    void advance();
    void tokenizeComment();
    void tokenizeIdentifierOrKeyword(vector<Token>& tokens);
    void tokenizeNumber(vector<Token>& tokens);
    void tokenizeString(vector<Token>& tokens);
    void tokenizeOperator(vector<Token>& tokens);
    void tokenizeSeparator(vector<Token>& tokens);
    void tokenizeCharacter(vector<Token>& tokens);
    bool isKeyword(const string& value);
    bool isBoolean(const string& value);
    bool isOperator(char ch);
    bool isSeparator(char ch);
};

#endif
