    #include "Compilador/AnalizadorLexico.cpp"
    #include "Compilador/AnalizadorSintactico.cpp"
    #include "Compilador/AnalizadorSemantico.cpp"
    #include "Compilador/CodigoObjeto.cpp"
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

    void replaceBackslashQuote(const string& filename) {
        ifstream infile(filename);
        if (!infile.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        string tempFilename = filename + ".temp";
        ofstream outfile(tempFilename);
        if (!outfile.is_open()) {
            cerr << "Error creating temporary file: " << tempFilename << endl;
            return;
        }

        string line;
        while (getline(infile, line)) {
            size_t pos = 0;
            while ((pos = line.find("\\\"", pos)) != string::npos) {
                line.replace(pos, 2, "\"");
                pos += 1;
            }
            outfile << line << "\n";
        }

        infile.close();
        outfile.close();

        if (rename(tempFilename.c_str(), filename.c_str()) != 0) {
            cerr << "Error renaming temporary file to original file" << endl;
        }
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
        /*
        for (const Token& token : tokens) {
            if (token.type == TokenType::Error) {
                cerr << "Error: Carácter inválido encontrado en línea " << token.line << ", columna " << token.column 
                    << ". Valor: " << token.value << endl;
                return 1;
            }
            cout<< "Tipo: " << static_cast<int>(token.type) << ", Valor: " << token.value 
                << ", Linea: " << token.line << ", Columna: " << token.column << endl;
        }
        */
        //analisis sintactico
        AnalizadorSintactico parser(tokens);
        ASTNodePtr astRoot = parser.parse();
        writeDotFile(astRoot, "ast.dot");
        AnalizadorSemantico symbolTable;
        symbolTable.analizar(astRoot);
        //symbolTable.TablaDeSimbolos_.imprimir();
        GeneradorCodigo CPP;
        CPP.generar(astRoot,"salida.cpp");
        replaceBackslashQuote("salida.cpp");
        return 0;
    }