#include <iostream>
#include <fstream>
#include "AnalizadorSemantico.h" 

string convertirAPseudoCodigo(const string& palabra) {
    static const unordered_map<string, string> conversionMap = {
        {"entero", "int"},
        {"real", "float"},
        {"cadena", "string"},
        {"booleano", "bool"},
        {"caracter", "char"},
        {"leer", "cin"},
        {"imprimir", "cout"},
        {"si", "if"},
        {"sino", "else"},
        {"segun", "switch"},
        {"caso", "case"},
        {"termina", "break"},
        {"para", "for"},
        {"mientras", "while"},
        {"haga", "do"},
        {"otroCaso", "default"}
    };

    auto it = conversionMap.find(palabra);
    if (it != conversionMap.end()) {
        return it->second;
    } else {
        return palabra;
    }
}

class GeneradorCodigo {
public:
    int n=1;
    void generar(const ASTNodePtr& root, const string& nombreArchivo) {
        ofstream archivoSalida(nombreArchivo);
        if (!archivoSalida.is_open()) {
            throw runtime_error("No se pudo abrir el archivo de salida");
        }
        archivoSalida << "#include <iostream>" << endl;
        archivoSalida << "#include <string>" << endl;
        archivoSalida << "using namespace std;" << endl;
        archivoSalida << "int main(){" << endl;
        recorrerAST(root, archivoSalida,n);
        archivoSalida << "}" << endl;

        archivoSalida.close();
    }

    void recorrerAST(const ASTNodePtr& node, ofstream& archivoSalida,int n) {
        switch (node->type) {
            case ASTNodeType::Declaration:
                for(int i=0;i<n;i++){
                    archivoSalida<<"\t";
                }
                generarCodigoDeclaracion(node, archivoSalida);
                break;
            case ASTNodeType::Assignment:
                for(int i=0;i<n;i++){
                    archivoSalida<<"\t";
                }
                generarCodigoAsignacion(node, archivoSalida);
                break;
            case ASTNodeType::Read:
                for(int i=0;i<n;i++){
                    archivoSalida<<"\t";
                }
                generarCodigoLectura(node, archivoSalida);
                break;
            case ASTNodeType::Print:
                for(int i=0;i<n;i++){
                    archivoSalida<<"\t";
                }
                generarCodigoImpresion(node, archivoSalida);
                break;
            case ASTNodeType::If:
                for(int i=0;i<n;i++){
                    archivoSalida<<"\t";
                }
                generarCodigoDecision(node, archivoSalida);
                break;
            case ASTNodeType::Switch:
                for(int i=0;i<n;i++){
                    archivoSalida<<"\t";
                }
                generarCodigoDecision(node, archivoSalida);
                break;
            case ASTNodeType::While:
                for(int i=0;i<n;i++){
                    archivoSalida<<"\t";
                }
                generarCodigoRepeticion(node, archivoSalida);
                break;
            case ASTNodeType::For:
                for(int i=0;i<n;i++){
                    archivoSalida<<"\t";
                }
                generarCodigoRepeticion(node, archivoSalida);
                break;
            case ASTNodeType::Expression:
                for(int i=0;i<n;i++){
                    archivoSalida<<"\t";
                }
                generarCodigoExpresion(node, archivoSalida);
                break;
            default:
                for (const auto& child : node->children) {
                    recorrerAST(child, archivoSalida,n);
                }
                break;
        }
    }

    void generarCodigoDeclaracion(const ASTNodePtr& node, ofstream& archivoSalida) {
        string varType = node->value;
        int a=0;
        for (const auto& child : node->children) {
            if(a>0){
                archivoSalida<<"\t";
            }
            string varName = child->value;
            archivoSalida << convertirAPseudoCodigo(varType) << " " << varName;
            if (!child->children.empty()) {
                if(child->children[0]->value[0] == '\"' && (child->children[0]->value[1] != 't')){
                    string aux = child->children[0]->value.substr(1,child->children[0]->value.size()-2);
                    archivoSalida << " = "<<'\"' << aux << '\"';
                }else{
                    archivoSalida << " = " << child->children[0]->value;
                }
            }
            archivoSalida << ";" << endl;
            a++;
        }
    }

    void generarCodigoAsignacion(const ASTNodePtr& node, ofstream& archivoSalida) {
        if(node->children[0]->value != "asignacion(es)"){
            for (const auto& child : node->children) {
                string varName = child->value;
                archivoSalida << varName;
                if (child->children[0]->children[0]->type != ASTNodeType::Operator ) {
                    archivoSalida << child->children[0]->value << child->children[0]->children[0]->value;
                }else{
                    archivoSalida << child->children[0]->value <<
                    child->children[0]->children[0]->children[0]->value << child->children[0]->children[0]->value
                    <<child->children[0]->children[0]->children[1]->value;
                }
                archivoSalida << ";" << endl;
            }
        }
        //string varName = node->value;
        //archivoSalida << varName << " = " << node->children[0]->value << ";" << endl;
    }

    void generarCodigoLectura(const ASTNodePtr& node, ofstream& archivoSalida) {
        string varName = node->children[0]->value;
        archivoSalida << "cin >> " << varName << ";" << endl;
    }

    void generarCodigoImpresion(const ASTNodePtr& node, ofstream& archivoSalida) {
        archivoSalida << "cout << ";
        for (const auto& child : node->children) {
            archivoSalida << child->value <<" << "; 
        }
        archivoSalida << " endl;" << endl;
    }

    void generarCodigoDecision(const ASTNodePtr& node, ofstream& archivoSalida) {
        if (node->type == ASTNodeType::If) {
            archivoSalida << "if (" << generarCodigoCondicion(node->children[0]) << ") {" << endl;
            recorrerAST(node->children[1], archivoSalida,n+1);
            for(int i=0;i<n;i++){
                archivoSalida<<"\t";
            }
            archivoSalida << "}";
            if (node->children.size() > 2) {
                archivoSalida << " else {" << endl;
                recorrerAST(node->children[2], archivoSalida,n+1);
                for(int i=0;i<n;i++){
                    archivoSalida<<"\t";
                }
                archivoSalida << "}";
            }
            archivoSalida << endl;
        } else if (node->type == ASTNodeType::Switch) {
            string varName = node->children[0]->value;
            archivoSalida << "switch (" << varName << ") {" << endl;
            for(size_t i = 1; i < node->children.size()-1; i++) {
                for(int i=0;i<n+1;i++){
                    archivoSalida<<"\t";
                }
                archivoSalida << "case " << node->children[i]->value << ":" << endl;
                archivoSalida<<"\t";
                recorrerAST(node->children[i]->children[0], archivoSalida,n+1);
                archivoSalida<<"\t";
                archivoSalida << "break;" << endl;
            }
            for(int i=0;i<n+1;i++){
                archivoSalida<<"\t";
            }
            archivoSalida << "default:" << endl;
            archivoSalida<<"\t";
            recorrerAST(node->children.back(), archivoSalida,n+1);
            archivoSalida<<"\t";
            archivoSalida << "break;" << endl;
            archivoSalida << "}" << endl;
        }
    }

    void generarCodigoRepeticion(const ASTNodePtr& node, ofstream& archivoSalida) {
        if (node->type == ASTNodeType::While) {
            if(node->value == "Mientras"){
                archivoSalida << "while (" << generarCodigoCondicion(node->children[0]) << ") {" << endl;
                recorrerAST(node->children[1], archivoSalida,n+1);
                archivoSalida << "}" << endl;
            }else if(node->value == "Haga-mientras"){
                archivoSalida << "do {" << endl;
                recorrerAST(node->children[1], archivoSalida,n+1);
                archivoSalida<< "} while (" << generarCodigoCondicion(node->children[0])<< ")" << endl;
            }
        } else if (node->type == ASTNodeType::For) {
            archivoSalida << "for (";
            if(node->children[0]->children[0]->type == ASTNodeType::Declaration){
                generarCodigoDeclaracion(node->children[0]->children[0],archivoSalida);
            }else{
                generarCodigoAsignacion(node->children[0]->children[0],archivoSalida);

            }
            
            cout<<"declare"<<endl;
            cout<<generarCodigoCondicion(node->children[1])<<endl;
            archivoSalida<< generarCodigoCondicion(node->children[1]) << "; ";
            cout<<generarCodigoCondicion(node->children[1])<<endl;
            cout<<"declare"<<endl;


            generarCodigoAsignacion(node->children[2]->children[0],archivoSalida);
            archivoSalida << ") {" << endl;
            recorrerAST(node->children[3], archivoSalida,n+1);
            archivoSalida << "}" << endl;
        }
    }

    void generarCodigoExpresion(const ASTNodePtr& node, ofstream& archivoSalida) {
        archivoSalida << node->value;
        for (const auto& child : node->children) {
            archivoSalida << " " << child->value;
        }
        archivoSalida << ";" << endl;
    }

    string generarCodigoCondicion(const ASTNodePtr& node) {
        string condition;
        if (node->children[0]->type == ASTNodeType::Variable) {
            condition = node->children[0]->value;
        } else if(node->children[0]->type == ASTNodeType::Operator){
            condition = node->children[0]->children[0]->value + node->children[0]->value + node->children[0]->children[1]->value;
        }
        return condition;
    }
};

