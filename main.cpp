#include "Compilador/AnalizadorLexico.cpp"
#include <fstream>
#include <sstream>

int main() {
    // Leer el archivo de texto
    std::ifstream file("pseudocodeExample.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // Crear el analizador léxico y tokenizar el archivo
    AnalizadorLexico lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    // Imprimir los tokens
    for (const Token& token : tokens) {
        std::cout << "Tipo: " << static_cast<int>(token.type) << ", Valor: " << token.value 
                  << ", Linea: " << token.line << ", Columna: " << token.column << std::endl;
    }

    return 0;
}
