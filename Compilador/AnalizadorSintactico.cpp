#include "AnalizadorSintactico.h"

bool isAssignmentOperator(const string& op) {
    static const vector<string> comparisonOperators = {"=", "+=", "-=", "*=", "/="};
    return find(comparisonOperators.begin(), comparisonOperators.end(), op) != comparisonOperators.end();
}

bool isComparisonOperator(const string& op) {
    static const vector<string> comparisonOperators = {"==", "!=", "<", ">", "<=", ">="};
    return find(comparisonOperators.begin(), comparisonOperators.end(), op) != comparisonOperators.end();
}

bool isAutoincrementOperator(const string& op) {
    static const vector<string> comparisonOperators = {"++", "--"};
    return find(comparisonOperators.begin(), comparisonOperators.end(), op) != comparisonOperators.end();
}

bool isType(const string& value) {
    static const vector<string> types = {"entero", "real", "cadena", "booleano", "caracter"};
    return find(types.begin(), types.end(), value) != types.end();
}

string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::Identifier: return "Identifier";
        case TokenType::Keyword: return "Keyword";
        case TokenType::Operator: return "Operator";
        case TokenType::Separator: return "Separator";
        case TokenType::Integer: return "Integer";
        case TokenType::Real: return "Real";
        case TokenType::String: return "String";
        case TokenType::Boolean: return "Boolean";
        case TokenType::Character: return "Character";
        case TokenType::EndOfFile: return "EndOfFile";
        case TokenType::Error: return "Error";
        default: return "Unknown";
    }
}

void AnalizadorSintactico::advance() {
    if (tokenIndex < tokens.size() - 1) {
        tokenIndex++;
        currentToken = tokens[tokenIndex];
    } else {
        currentToken = { TokenType::EndOfFile, "", currentToken.line, currentToken.column };
    }
}

Token AnalizadorSintactico::peek(size_t offset) {
    if (tokenIndex + offset < tokens.size()) {
        return tokens[tokenIndex + offset];
    } else {
        return { TokenType::EndOfFile, "", currentToken.line, currentToken.column };
    }
}

void AnalizadorSintactico::expect(TokenType type) {
    if (currentToken.type == type) {
        advance();
    } else {
        throw runtime_error("Unexpected token: " + currentToken.value + " type: " + tokenTypeToString(currentToken.type)
        + " in line: " + to_string(currentToken.line) + " in column: " + to_string(currentToken.column)  
        + " expected: " + tokenTypeToString(type));
    }
}

void AnalizadorSintactico::expect(TokenType type, string expect_) {
    if (currentToken.type == type && currentToken.value == expect_) {
        advance();
    } else {
        throw runtime_error("Unexpected token: " + currentToken.value + " type: " + tokenTypeToString(currentToken.type) 
        + " in line: " +  to_string(currentToken.line) + " in column: " +  to_string(currentToken.column) 
        + ".Expected: " + expect_ + " type: " + tokenTypeToString(type));
    }
}

ASTNodePtr AnalizadorSintactico::parse() {
    return parseProgram();
}

ASTNodePtr AnalizadorSintactico::parseProgram() {
    expect(TokenType::Keyword,"algoritmo"); 
    string programName = currentToken.value;
    expect(TokenType::Identifier);
    expect(TokenType::Separator,"{"); 
    expect(TokenType::Keyword,"principal"); 
    expect(TokenType::Separator,"{"); 
    ASTNodePtr mainBlock = parseBlock();
    expect(TokenType::Separator,"}");
    expect(TokenType::Separator,"}");
    expect(TokenType::EndOfFile); 
    return make_shared<ASTNode>(ASTNodeType::Program, currentToken.line, currentToken.column, programName, mainBlock);
}

ASTNodePtr AnalizadorSintactico::parseBlock() {
    ASTNodePtr blockNode = make_shared<ASTNode>(ASTNodeType::Block, currentToken.line, currentToken.column,"cuerpo");
    while ((currentToken.type != TokenType::Separator && currentToken.value != "}") || currentToken.value != "termina") {
        if((currentToken.type == TokenType::Separator && currentToken.value == "}")||currentToken.value == "termina") break;
        blockNode->children.push_back(parseStatement());
    }
    return blockNode;
}

ASTNodePtr AnalizadorSintactico::parseStatement() {
    switch(currentToken.type) {
        case TokenType::Keyword:
            if (currentToken.value == "entero" || currentToken.value == "real" || currentToken.value == "cadena" || currentToken.value == "booleano" || currentToken.value == "caracter") {
                return parseDeclaration();
            } else if (currentToken.value == "leer") {
                return parseRead();
            } else if (currentToken.value == "imprimir") {
                return parsePrint();
            } else if (currentToken.value == "si") {
                return parseIf();
            } else if (currentToken.value == "mientras") {
                return parseWhile();
            } else if (currentToken.value == "haga") {
                return parseDoWhile();
            } else if (currentToken.value == "para") {
                return parseFor();
            } else if (currentToken.value == "segun") {
                return parseSwitch();
            }
            break;
        case TokenType::Identifier:
            return parseAssignment();
        default:
            throw runtime_error("Unexpected token: " + currentToken.value + " type: " + tokenTypeToString(currentToken.type)
            + " in line: " +  to_string(currentToken.line) + " in column: " +  to_string(currentToken.column));
    }
    return nullptr;
}

ASTNodePtr AnalizadorSintactico::parseDeclaration() {
    string type = currentToken.value;
    advance();
    ASTNodePtr declNode = make_shared<ASTNode>(ASTNodeType::Declaration, currentToken.line, currentToken.column, type);
    while(true){
        string varName = currentToken.value;
        expect(TokenType::Identifier);
        ASTNodePtr varNode = make_shared<ASTNode>(ASTNodeType::Variable, currentToken.line, currentToken.column, varName);
        declNode->children.push_back(varNode);
        if(currentToken.type == TokenType::Operator && currentToken.value == "=") {
            advance();
            if(declNode->value == "entero"){
                varNode->children.push_back(parseExpression(TokenType::Integer));
            }else if(declNode->value == "real"){
                varNode->children.push_back(parseExpression(TokenType::Real));
            }else if(declNode->value == "cadena"){
                varNode->children.push_back(parseExpression(TokenType::String));
            }else if(declNode->value == "booleano"){
                varNode->children.push_back(parseExpression(TokenType::Boolean));
            }else if(declNode->value == "caracter"){
                varNode->children.push_back(parseExpression(TokenType::Character));
            }else {
                expect(TokenType::Integer);
            }
        }
        if(currentToken.type != TokenType::Separator || currentToken.value != ",") {
            break;
        }
        advance();
    }
    expect(TokenType::Separator,";");
    return declNode;
}

ASTNodePtr AnalizadorSintactico::parseAssignment() {
    ASTNodePtr assignNode = make_shared<ASTNode>(ASTNodeType::Assignment, currentToken.line, currentToken.column, "asignacion(es)");
    while(true){
        string varName = currentToken.value;
        expect(TokenType::Identifier);
        string opName = currentToken.value;
        if(isAssignmentOperator(opName)){
            ASTNodePtr single_assignNode = make_shared<ASTNode>(ASTNodeType::Assignment, currentToken.line, currentToken.column, varName);
            ASTNodePtr operatorNode = make_shared<ASTNode>(ASTNodeType::Operator, currentToken.line, currentToken.column, opName);
            expect(TokenType::Operator);
            single_assignNode->children.push_back(operatorNode);
            assignNode->children.push_back(single_assignNode);
            if(currentToken.type == TokenType::Identifier){
                varName = currentToken.value;
                ASTNodePtr var1Node = make_shared<ASTNode>(ASTNodeType::Variable, currentToken.line, currentToken.column, varName);
                expect(TokenType::Identifier);
                opName = currentToken.value;
                ASTNodePtr op2Node = make_shared<ASTNode>(ASTNodeType::Operator, currentToken.line, currentToken.column, opName);
                expect(TokenType::Operator);
                varName = currentToken.value;
                operatorNode->children.push_back(op2Node);
                op2Node->children.push_back(var1Node);
                if(currentToken.type == TokenType::Identifier){
                    ASTNodePtr var2Node = make_shared<ASTNode>(ASTNodeType::Variable, currentToken.line, currentToken.column, varName);
                    expect(TokenType::Identifier);
                    op2Node->children.push_back(var2Node);
                }else{
                    op2Node->children.push_back(parseExpression());
                }
            }else{
                operatorNode->children.push_back(parseExpression());
            }
        }else if(isAutoincrementOperator(opName)){
            ASTNodePtr single_assignNode = make_shared<ASTNode>(ASTNodeType::Assignment, currentToken.line, currentToken.column, varName);
            ASTNodePtr operatorNode = make_shared<ASTNode>(ASTNodeType::Operator, currentToken.line, currentToken.column, opName);
            expect(TokenType::Operator);
            single_assignNode->children.push_back(operatorNode);
            assignNode->children.push_back(single_assignNode);
        }
        if(currentToken.type == TokenType::Separator && currentToken.value != ",") break;
        expect(TokenType::Separator,",");  
    }
    expect(TokenType::Separator,";");
    return assignNode;
}

ASTNodePtr AnalizadorSintactico::parseRead() {
    ASTNodePtr readNode = make_shared<ASTNode>(ASTNodeType::Read, currentToken.line, currentToken.column,"leer");
    expect(TokenType::Keyword,"leer");
    expect(TokenType::Separator,"(");
    string varName = currentToken.value;
    ASTNodePtr varNode = make_shared<ASTNode>(ASTNodeType::Variable, currentToken.line, currentToken.column,varName);
    expect(TokenType::Identifier);
    readNode->children.push_back(varNode);
    expect(TokenType::Separator,")"); 
    expect(TokenType::Separator,";");
    return readNode;
}

ASTNodePtr AnalizadorSintactico::parsePrint() {
    ASTNodePtr printNode = make_shared<ASTNode>(ASTNodeType::Print, currentToken.line, currentToken.column,"salida");
    expect(TokenType::Keyword,"imprimir"); 
    expect(TokenType::Separator,"(");
    ASTNodePtr exprNode = nullptr;
    if(currentToken.type == TokenType::String){
        exprNode = parseExpression(TokenType::String);
    }else{
        exprNode = parseExpression(TokenType::Identifier);
    }
    printNode->children.push_back(exprNode);
    while(currentToken.value == "+"){
        expect(TokenType::Operator);
        if(currentToken.type == TokenType::String){
            exprNode = parseExpression(TokenType::String);
        }else{
            exprNode = parseExpression(TokenType::Identifier);
        }
        printNode->children.push_back(exprNode);
    }
    expect(TokenType::Separator,")");
    expect(TokenType::Separator,";");
    return printNode;
}

ASTNodePtr AnalizadorSintactico::parseIf() {
    ASTNodePtr ifNode = make_shared<ASTNode>(ASTNodeType::If,currentToken.line, currentToken.column,"condicional_si");
    expect(TokenType::Keyword,"si");
    expect(TokenType::Separator,"(");
    ASTNodePtr condition = parseExpressionBooleana();
    ifNode->children.push_back(condition);
    expect(TokenType::Separator,")");
    expect(TokenType::Separator,"{");
    ASTNodePtr thenBlock = parseBlock();
    ifNode->children.push_back(thenBlock);
    expect(TokenType::Separator,"}");

    ASTNodePtr elseBlock = nullptr;
    if (currentToken.type == TokenType::Keyword && currentToken.value == "sino") {
        expect(TokenType::Keyword,"sino");
        expect(TokenType::Separator,"{");
        elseBlock = parseBlock();
        expect(TokenType::Separator,"}");
        ifNode->children.push_back(elseBlock);
    }
    return ifNode;
}

ASTNodePtr AnalizadorSintactico::parseWhile() {
    ASTNodePtr whileNode = make_shared<ASTNode>(ASTNodeType::While, currentToken.line, currentToken.column,"Mientras");
    expect(TokenType::Keyword,"mientras");
    expect(TokenType::Separator,"(");
    ASTNodePtr condition = parseExpressionBooleana();
    expect(TokenType::Separator,")");
    expect(TokenType::Separator,"{");
    ASTNodePtr body = parseBlock();
    expect(TokenType::Separator,"}");
    whileNode->children.push_back(condition);
    whileNode->children.push_back(body);
    return whileNode;
}

ASTNodePtr AnalizadorSintactico::parseDoWhile() {
    ASTNodePtr dowhileNode = make_shared<ASTNode>(ASTNodeType::While, currentToken.line, currentToken.column,"Haga-mientras");
    expect(TokenType::Keyword,"haga");
    expect(TokenType::Separator,"{");
    ASTNodePtr body = parseBlock();
    expect(TokenType::Separator,"}"); 
    expect(TokenType::Keyword,"mientras");
    expect(TokenType::Separator,"(");
    ASTNodePtr condition = parseExpressionBooleana();
    expect(TokenType::Separator,")");
    expect(TokenType::Separator,";");
    dowhileNode->children.push_back(body);
    dowhileNode->children.push_back(condition);
    return dowhileNode;
}

ASTNodePtr AnalizadorSintactico::parseFor() {
    ASTNodePtr forNode = make_shared<ASTNode>(ASTNodeType::For, currentToken.line, currentToken.column,"bucle para");
    expect(TokenType::Keyword,"para");
    expect(TokenType::Separator,"(");
    ASTNodePtr initExpr = parseExpressionForInit();
    ASTNodePtr condition = parseExpressionBooleana();
    expect(TokenType::Separator,";");
    ASTNodePtr incrementExpr = parseExpressionForAsign();
    expect(TokenType::Separator,")");
    expect(TokenType::Separator,"{");
    ASTNodePtr body = parseBlock();
    expect(TokenType::Separator,"}");
    forNode->children.push_back(initExpr);
    forNode->children.push_back(condition);
    forNode->children.push_back(incrementExpr);
    forNode->children.push_back(body);
    return forNode;
}

ASTNodePtr AnalizadorSintactico::parseSwitch() {
    expect(TokenType::Keyword,"segun");
    expect(TokenType::Separator,"(");
    ASTNodePtr variable = parseExpression(TokenType::Identifier);
    expect(TokenType::Separator,")");
    expect(TokenType::Separator,"{");
    ASTNodePtr switchNode = make_shared<ASTNode>(ASTNodeType::Switch, currentToken.line, currentToken.column,"decision_multiple",variable);
    while(true) {
        if(currentToken.value == "caso") {
            expect(TokenType::Keyword);
            ASTNodePtr caseValue = parseExpression();
            expect(TokenType::Separator,":");
            ASTNodePtr caseBlock = parseBlock();
            switchNode->children.push_back(caseValue);
            caseValue->children.push_back(caseBlock);
            
            expect(TokenType::Keyword,"termina");
            expect(TokenType::Separator,";");
        }else if(currentToken.value == "otroCaso") {
            expect(TokenType::Keyword);
            expect(TokenType::Separator,":");
            ASTNodePtr defaultCase = parseBlock();
            switchNode->children.push_back(defaultCase);
            break;
        }
    }
    expect(TokenType::Separator,"}");
    return switchNode;
}

ASTNodePtr AnalizadorSintactico::parseExpression() {
    ASTNodePtr exprNode = make_shared<ASTNode>(ASTNodeType::Expression, currentToken.line, currentToken.column,currentToken.value);
    advance();
    return exprNode;
}

ASTNodePtr AnalizadorSintactico::parseExpression(TokenType type) {
    string ExpName = currentToken.value;
    ASTNodePtr exprNode = make_shared<ASTNode>(ASTNodeType::Expression, currentToken.line, currentToken.column,ExpName);
    expect(type);
    return exprNode;
}

ASTNodePtr AnalizadorSintactico::parseExpressionBooleana() {
    ASTNodePtr exprNode = nullptr;
    string boolName = currentToken.value;
    if(currentToken.type == TokenType::Boolean){
        exprNode = make_shared<ASTNode>(ASTNodeType::Expression, currentToken.line, currentToken.column, boolName);
        expect(TokenType::Boolean);
    }else if(currentToken.type == TokenType::Identifier){
        exprNode = make_shared<ASTNode>(ASTNodeType::Expression, currentToken.line, currentToken.column,"expresion booleana");
        ASTNodePtr varNode1 = make_shared<ASTNode>(ASTNodeType::Variable, currentToken.line, currentToken.column, boolName);
        expect(TokenType::Identifier);
        boolName = currentToken.value;
        if(currentToken.type == TokenType::Separator){
            exprNode->children.push_back(varNode1);
            return exprNode;
        }
        ASTNodePtr opNode2 = make_shared<ASTNode>(ASTNodeType::Operator, currentToken.line, currentToken.column, boolName);
        expect(TokenType::Operator);
        if(isComparisonOperator(boolName)){
            exprNode->children.push_back(opNode2);
            opNode2->children.push_back(varNode1);
            boolName = currentToken.value;
            if(currentToken.type == TokenType::Identifier){
                ASTNodePtr varNode3 = make_shared<ASTNode>(ASTNodeType::Variable, currentToken.line, currentToken.column, boolName);
                expect(TokenType::Identifier);
                opNode2->children.push_back(varNode3);
            }else{
                opNode2->children.push_back(parseExpression(currentToken.type));
            }
        }else{
            expect(TokenType::Operator,"==");
        }
    }
    return exprNode;
}
ASTNodePtr AnalizadorSintactico::parseExpressionForInit() {
    ASTNodePtr initNode = make_shared<ASTNode>(ASTNodeType::Expression, currentToken.line, currentToken.column,"init_para");
    if(currentToken.type == TokenType::Keyword){
        initNode->children.push_back(parseDeclaration());
    }else if(currentToken.type == TokenType::Identifier){
        initNode->children.push_back(parseAssignment());
    }else{
        expect(TokenType::Keyword);
    }
    return initNode;
}

ASTNodePtr AnalizadorSintactico::parseExpressionForAsign() {
    ASTNodePtr expNode = make_shared<ASTNode>(ASTNodeType::Expression, currentToken.line,currentToken.column,"asign_para");
    if(isAssignmentOperator(peek(1).value)){
        expNode->children.push_back(parseAssignment());
    }else if(isAutoincrementOperator(peek(1).value)){
        ASTNodePtr varNode = make_shared<ASTNode>(ASTNodeType::Variable, currentToken.line,currentToken.column,currentToken.value);
        expect(TokenType::Identifier);
        expNode->children.push_back(varNode);
        ASTNodePtr opNode = make_shared<ASTNode>(ASTNodeType::Operator, currentToken.line,currentToken.column,currentToken.value);
        expect(TokenType::Operator);
        varNode->children.push_back(opNode);
    }
    return expNode;
}

