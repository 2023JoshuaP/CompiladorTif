#include "Compilador/AnalizadorLexico.cpp"
#include "Compilador/AnalizadorSintactico.cpp"
#include "Compilador/AnalizadorSemantico.cpp"
//#include "Compilador/CodigoObjeto.cpp"
#include <fstream>
#include <sstream>
#include <iostream>

void generateDotFile(shared_ptr<ASTNode> node, ofstream& out, int& nodeId) {
    if (!node) return;

    int currentId = nodeId++;
    
    out << "  node" << currentId << " [label=\"" <<static_cast<int>(node->type)<<"/" <<node->value << "\"];\n";

    for (auto& child : node->children) {
        int childId = nodeId;
        generateDotFile(child, out, nodeId);
        out << "  node" << currentId << " -> node" << childId << ";\n";
    }
}
void writeDotFile(shared_ptr<ASTNode> root, const string& filename) {
    ofstream out(filename);
    out << "digraph AST {\n";
    int nodeId = 0;
    generateDotFile(root, out, nodeId);
    out << "}\n";
    out.close();
}

void limpiarCaracterEnArchivo(const string& filename, char caracter) {
    ifstream file(filename);
    if (!file) {
        cerr << "No se pudo abrir el archivo " << filename << endl;
        return;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    string contenido = buffer.str();
    contenido.erase(remove(contenido.begin(), contenido.end(), caracter), contenido.end());
    
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "No se pudo escribir en el archivo " << filename << endl;
        return;
    }
    outFile << contenido;
    outFile.close();
}

int main(){
    ifstream file("pseudocodeExample.txt");
    if (!file) {
        cerr << "No se pudo abrir el archivo pseudocodeExample.txt" << endl;
        return 1;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string source = buffer.str();
    //analisis lexico
    AnalizadorLexico lexer(source);
    vector<Token> tokens = lexer.tokenize();
    
    for (const Token& token : tokens) {
        if (token.type == TokenType::Error) {
            cerr << "Error: Carácter inválido encontrado en línea " << token.line << ", columna " << token.column 
                << ". Valor: " << token.value << endl;
            return 1;
        }
        cout<< "Tipo: " << static_cast<int>(token.type) << ", Valor: " << token.value 
            << ", Linea: " << token.line << ", Columna: " << token.column << endl;
    }
    
    //analisis sintactico
    AnalizadorSintactico parser(tokens);
    ASTNodePtr astRoot = parser.parse();
    writeDotFile(astRoot, "ast.dot");
    AnalizadorSemantico symbolTable;
    symbolTable.analizar(astRoot);
    symbolTable.TablaDeSimbolos_.imprimir();
    //GeneradorCodigo CPP;
    //CPP.generar(astRoot,"salida2.cpp");
    //limpiarCaracterEnArchivo("salida2.cpp",'\\');
    return 0;
}