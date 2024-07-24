#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

using namespace std;
class ASTNode;
using ASTNodePtr = shared_ptr<ASTNode>;
enum class ASTNodeType {
    Program, Declaration, Assignment, Read, Print, If, While, For, Switch, Block, Expression, Variable, Operator
};

class ASTNode {
public:
    ASTNodeType type;
    int line;
    int column;
    vector<ASTNodePtr> children;
    string value; 

    ASTNode(ASTNodeType type_, int line_, int column_, const string& value_)
        : type(type_), line(line_), column(column_), value(value_) {}
    ASTNode(ASTNodeType type_, int line_, int column_, const string& value_,const ASTNodePtr& children_)
        : type(type_), line(line_), column(column_), value(value_) {children.push_back(children_);}
    virtual ~ASTNode() = default;
};


#endif
