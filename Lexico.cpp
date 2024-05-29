#include "Lexico.h"

Lexico::Lexico(int argc, char* argv[]) {
    file = stdin;
    if (argc == 2) {
        file = fopen(argv[1], "rt");
        if (file == nullptr) {
            file = stdin;
        }
    }
    if (file == stdin) {
        cout << "Ingrese texto .......... termine con Ctrl+Z" << endl;
    }
    pseudocodeToCpp["Proceso"] = "int main() {";
    pseudocodeToCpp["FinProceso"] = "return 0; }";
}