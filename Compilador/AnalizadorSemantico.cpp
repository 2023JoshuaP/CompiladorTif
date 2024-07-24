#include "AnalizadorSemantico.h"
#include <regex>

bool isComparisonOperator2(const string& op) {
    static const vector<string> comparisonOperators = {"==", "!=", "<", ">", "<=", ">="};
    return find(comparisonOperators.begin(), comparisonOperators.end(), op) != comparisonOperators.end();
}
bool isAutoincrementOperator2(const string& op) {
    static const vector<string> comparisonOperators = {"++", "--"};
    return find(comparisonOperators.begin(), comparisonOperators.end(), op) != comparisonOperators.end();
}
bool esReal(const string& cadena) {
    regex patron(R"(\d+\.\d+)");
    smatch coincidencia;
    return regex_match(cadena, coincidencia, patron);
}
bool verInt(const string& cadena) {
    for (char c : cadena) {
        if (!isdigit(c)) {
            return false; 
        }
    }
    return true;
}

void TablaDeSimbolos::agregar(const string& nombre, const string& tipo, const string& valor, int lineaDecl, int columnaDecl) {
    if (tabla.find(nombre) != tabla.end()) {
        throw runtime_error("Variable ya declarada: " + nombre + " en linea: " + to_string(lineaDecl) + " en columna: " + to_string(columnaDecl));
    }
    tabla[nombre] = InformacionVariable(tipo, valor, lineaDecl, columnaDecl);
}
InformacionVariable TablaDeSimbolos::obtenerInformacion(const string& nombre) const {
    auto it = tabla.find(nombre);
    if (it == tabla.end()) {
        throw runtime_error("Variable no declarada: " + nombre);
    }
    return it->second;
}
bool TablaDeSimbolos::existe(const string& nombre) const {
    return tabla.find(nombre) != tabla.end();
}
void TablaDeSimbolos::actualizarValor(const string& nombre, const string& nuevoValor) {
    auto it = tabla.find(nombre);
    if (it != tabla.end()) {
        it->second.valor = nuevoValor;
    } else {
        throw runtime_error("Variable no declarada: " + nombre);
    }
}


void AnalizadorSemantico::analizar(const ASTNodePtr& root) {
    if (root) {
        visitarNodo(root);
    } else {
        throw runtime_error("AST root is null");
    }
}
void AnalizadorSemantico::visitarNodo(const ASTNodePtr& node) {
    switch (node->type) {
        case ASTNodeType::Declaration:
            verificarDeclaracion(node);
            break;
        case ASTNodeType::Assignment:
            for (const auto& child : node->children) {
                verificarAsignacion(child);
            }
            break;
        case ASTNodeType::Read:
            verificarLectura(node->children[0]);
            break;
        case ASTNodeType::Print:
            verificarImpresion(node);
            break;
        case ASTNodeType::If:
            verificarDecision(node);
            break;
        case ASTNodeType::Switch:
            verificarDecision(node);
            break;
        case ASTNodeType::While:
            verificarRepeticion(node);
            break;
        case ASTNodeType::For:
            verificarRepeticion(node);
            break;
        case ASTNodeType::Expression:
            verificarExpresion(node);
            break;
        default:
            for (const auto& child : node->children) {
                visitarNodo(child);
            }
            break;
    }
}
void AnalizadorSemantico::verificarDeclaracion(const ASTNodePtr& node) {
    string varType = node->value;
    for (const auto& child : node->children) {
        string varName = child->value;
        if (TablaDeSimbolos_.existe(varName)) {
            throw runtime_error("Variable ya declarada: " + varName + " en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
        }
        if (child->children.size() > 0) {
            string exprType = obtenerTipo(child->children[0]);
            if (varType != exprType) {
                throw runtime_error("Tipo de inicialización incompatible para la variable: " + varName + " en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
            }
            TablaDeSimbolos_.agregar(varName,varType, child->children[0]->value, node->line, node->column);
            continue;
        }
        TablaDeSimbolos_.agregar(varName,varType, "", node->line, node->column);
    }
}
void AnalizadorSemantico::verificarAsignacion(const ASTNodePtr& node) {
    string varName = node->value; 
    if (!TablaDeSimbolos_.existe(varName)) {
        throw runtime_error("Variable no declarada: " + varName + " en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
    }
    string varType = (TablaDeSimbolos_.obtenerInformacion(varName)).tipo;
    ASTNodePtr child = node->children[0];
    if(!child->children.empty()){
        child = child->children[0]; //valor \ operador
    }else{
        if(!isAutoincrementOperator2(child->value)){
            throw runtime_error("Tipo de asignación incompatible para la variable: " + varName + " en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
        }
        TablaDeSimbolos_.actualizarValor(varName,TablaDeSimbolos_.obtenerInformacion(varName).valor +" + 1");
        return;
    }
    if(!child->children.empty() && child->children.size()>1){
        string exprType ="";
        if(child->children[0]->type == ASTNodeType::Variable){
            exprType = TablaDeSimbolos_.obtenerInformacion(child->children[0]->value).tipo;
        }else{
            exprType =  obtenerTipo(child->children[0]);
        }
        string exprType2 = "";
        if(child->children[1]->type == ASTNodeType::Variable){
            exprType2 = TablaDeSimbolos_.obtenerInformacion(child->children[1]->value).tipo;
        }else{
            exprType2 =  obtenerTipo(child->children[1]);
        }
        if (varType != exprType || varType != exprType2) {
            throw runtime_error("Tipo de asignación incompatible para la variable: " + varName  + " en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
        }
        TablaDeSimbolos_.actualizarValor(varName,child->children[0]->value + child->value + child->children[1]->value);
    }else{
        string exprType = "";
        if(child->type == ASTNodeType::Variable){
                exprType = TablaDeSimbolos_.obtenerInformacion(child->value).tipo;
            }else{
                exprType =  obtenerTipo(child);
            }
        if (varType != exprType) {
            throw runtime_error("Tipo de asignación incompatible para la variable: " + varName + " en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
        }
        TablaDeSimbolos_.actualizarValor(varName,child->value);
    }  
}
void AnalizadorSemantico::verificarLectura(const ASTNodePtr& node) {
    string varName = node->value;
    if (!TablaDeSimbolos_.existe(varName)) {
        throw runtime_error("Variable no declarada: " + varName + " en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
    }
}
void AnalizadorSemantico::verificarImpresion(const ASTNodePtr& node) {
    for (const auto& child : node->children) {
        if(child->type != ASTNodeType::Expression){
            throw runtime_error("La condición de la instrucción 'si' debe ser booleana. en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
        }
    }
}
void AnalizadorSemantico::verificarDecision(const ASTNodePtr& node) {
    if (node->type == ASTNodeType::If) {
        verificarExpresionBooleana(node->children[0]);
        if(node->children[1]){
            visitarNodo(node->children[1]);
        }
        if(node->children[2]){
            visitarNodo(node->children[2]);
        }
    } else if (node->type == ASTNodeType::Switch) {
        string varType = TablaDeSimbolos_.obtenerInformacion(node->children[0]->value).tipo;
        if(!TablaDeSimbolos_.existe(node->children[0]->value)){
            throw runtime_error("Variable no declarada: " + node->children[0]->value + " en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
        }
        for (size_t i = 1; i < node->children.size()-1; i++) {
            string caseType = obtenerTipo(node->children[i]);
            if (caseType != varType) {
                throw runtime_error("Tipo de caso incompatible en 'segun': " + caseType  + " en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
            }
            if(node->children[i]->children[0]){
                visitarNodo(node->children[i]->children[0]);
            }
        }
        if(node->children[node->children.size()-1]){
            visitarNodo(node->children[node->children.size()-1]);
        }
    }
}
void AnalizadorSemantico::verificarRepeticion(const ASTNodePtr& node) {
    if (node->type == ASTNodeType::While){
        if(node->value == "Mientras"){
            verificarExpresionBooleana(node->children[0]);
            if(node->children[1]){
                visitarNodo(node->children[1]);
            }
        }else if(node->value == "Haga-mientras"){
            if(node->children[0]){
                visitarNodo(node->children[0]);
            }
            verificarExpresionBooleana(node->children[1]);
        }
    }else if(node->type == ASTNodeType::For) {
        if(node->children[0]->children[0]->type == ASTNodeType::Declaration){
            verificarDeclaracion(node->children[0]->children[0]);
        }else if(node->children[0]->children[0]->type == ASTNodeType::Variable){
            verificarAsignacion(node->children[0]->children[0]);
        }
        verificarExpresionBooleana(node->children[1]);
        verificarAsignacion(node->children[2]->children[0]);
        if(node->children.size()>3){
            visitarNodo(node->children[3]);
        }
    }
}
void AnalizadorSemantico::verificarExpresion(const ASTNodePtr& node) {
    string type = "";
    if(node->children[0]->type == ASTNodeType::Variable){
        if(!TablaDeSimbolos_.existe(node->children[0]->value)){
            throw runtime_error("Variable no declarada: " + node->children[0]->value + " en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
        }
        type = TablaDeSimbolos_.obtenerInformacion(node->children[0]->value).tipo;

    }else{
        type = obtenerTipo(node->children[0]);
    }
    for (const auto& child : node->children) {
        if(child->type == ASTNodeType::Variable){
            if (!TablaDeSimbolos_.existe(child->value)) {
                throw runtime_error("Variable no declarada: " + child->value  + " en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
            }
            if(TablaDeSimbolos_.obtenerInformacion(child->value).tipo != type){
                throw runtime_error("Tipos de valores comparados no coinciden: " + type  + " en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
            }
        }else{
            if(obtenerTipo(child) != type){
                throw runtime_error("Tipos de valores comparados no coinciden: " + type  + " en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
            }
        }
    }
}
void AnalizadorSemantico::verificarExpresionBooleana(const ASTNodePtr& node) {
    if(node->value == "expresion booleana"){
        if(!isComparisonOperator2(node->children[0]->value)){
            throw runtime_error("La condición de la instrucción debe ser de tipo booleana. en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
        }
        verificarExpresion(node->children[0]);
    }else{
        string exprType = obtenerTipo(node);
        if (exprType != "booleano") {
            throw runtime_error("La condición de la instrucción debe ser de tipo booleana. + en linea: " + to_string(node->line) + " en columna: " + to_string(node->column));
        }
    }
}
string AnalizadorSemantico::obtenerTipo(const ASTNodePtr& node) {
    if(node->value == "true" || node->value == "false"){
        return "booleano";
    }else if(node->value[0] == '\\'){
        return "cadena";
    }else if(node->value[0] == '\''){
        return "caracter";
    }else if(verInt(node->value)){
        return "entero";
    }else if(esReal(node->value)){
        return "real";
    }
    return "not found";
}
