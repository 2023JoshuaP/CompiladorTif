#ifndef ANALIZADORSINTACTICO_H
#define ANALIZADORSINTACTICO_H

#include "ast.h"
#include "AnalizadorLexico.h"

class AnalizadorSintactico {
public:
    AnalizadorSintactico(vector<Token>& tokens_): tokens(tokens_), tokenIndex(0) {
        if (!tokens.empty()) {
            currentToken = tokens[tokenIndex];
        } else {
            cout<<"Tokens not found"<<endl;
        }
    }
    ASTNodePtr parse();

private:
    vector<Token> tokens;
    size_t tokenIndex;
    Token currentToken;

    void advance();
    Token peek(size_t offset = 0);
    void expect(TokenType type);
    void expect(TokenType type, std::string asd);
    
    ASTNodePtr parseProgram();
    ASTNodePtr parseBlock();
    ASTNodePtr parseStatement();
    ASTNodePtr parseDeclaration();
    ASTNodePtr parseAssignment();
    ASTNodePtr parseRead();
    ASTNodePtr parsePrint();
    ASTNodePtr parseIf();
    ASTNodePtr parseWhile();
    ASTNodePtr parseDoWhile();
    ASTNodePtr parseFor();
    ASTNodePtr parseSwitch();
    ASTNodePtr parseExpression();
    ASTNodePtr parseExpression(TokenType type);
    ASTNodePtr parseExpressionBooleana();
    ASTNodePtr parseExpressionForInit();
    ASTNodePtr parseExpressionForAsign();
};

#endif
