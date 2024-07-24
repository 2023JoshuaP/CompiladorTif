#ifndef ANALIZADORSEMANTICO_H
#define ANALIZADORSEMANTICO_H

#include "ast.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

struct InformacionVariable {
    string tipo;                      
    string valor;                     
    int lineaDeclaracion;             
    int columnaDeclaracion;           

    InformacionVariable(): tipo(""), valor(""), lineaDeclaracion(-1), columnaDeclaracion(-1) {}
    InformacionVariable(const string& tipo_, const string& valor_, int lineaDecl_, int columnaDecl_)
        : tipo(tipo_), valor(valor_), lineaDeclaracion(lineaDecl_), columnaDeclaracion(columnaDecl_) {}
    InformacionVariable(const InformacionVariable& other) 
    : tipo(other.tipo), valor(other.valor), lineaDeclaracion(other.lineaDeclaracion), columnaDeclaracion(other.columnaDeclaracion) {}
    string toString() const {
        string oss;
        oss= "Tipo: " + tipo + ", Valor: " + valor
            + ", Linea de declaracion: " + to_string(lineaDeclaracion)
            + ", Columna de declaracion: " + to_string(columnaDeclaracion);
        return oss;
    }

};
class TablaDeSimbolos {
public:
    void agregar(const string& nombre, const string& tipo, const string& valor, int lineaDecl, int columnaDecl);
    InformacionVariable obtenerInformacion(const string& nombre) const;
    bool existe(const string& nombre) const;
    void actualizarValor(const std::string& nombre, const std::string& nuevoValor);
    void imprimir() const {
        for (const auto& par : tabla) {
            cout << "Nombre: " << par.first << ", " << par.second.toString() << endl;
        }
    }
private:
    unordered_map<string, InformacionVariable> tabla;
};

class AnalizadorSemantico {
    public:
        AnalizadorSemantico() = default;
        void analizar(const ASTNodePtr& root);
        TablaDeSimbolos TablaDeSimbolos_;

    private:
        void visitarNodo(const ASTNodePtr& node);
        void verificarDeclaracion(const ASTNodePtr& node);
        void verificarAsignacion(const ASTNodePtr& node);
        void verificarLectura(const ASTNodePtr& node);
        void verificarImpresion(const ASTNodePtr& node);
        void verificarDecision(const ASTNodePtr& node);
        void verificarRepeticion(const ASTNodePtr& node);
        void verificarExpresion(const ASTNodePtr& node);
        void verificarExpresionBooleana(const ASTNodePtr& node);
        string obtenerTipo(const ASTNodePtr& node);
};

#endif