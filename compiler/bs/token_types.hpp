#pragma once

#include <string>
#include <vector>
#include <memory>


enum Tokens {
    Token_newline,
    Token_indentifier,
    Token_assignment,
    Token_real,
    Token_string,
    Token_addition,
    Token_subtraction,
    Token_multiplication,
    Token_division,
    Token_coma,
    Token_o_paren,
    Token_c_paren,
    Token_s_o_paren,
    Token_s_c_paren,
    Token_c_o_paren,
    Token_c_c_paren,
    Token_hasher,
    Token_1_tenary,
    Token_0_tenary,
    Token_walrus,
    Token_comp_equals,
    Token_greater,
    Token_greater_equals,
    Token_less,
    Token_less_equals,
    Token_not_equals,
    Token_and,
    Token_or,
    Token_not,
    Token_xor,
    Token_typeIdentifier,
};

enum Types {
    Type_undefined,
    Type_noone,
    Type_auto,
    Type_boolean,
    Type_real,
    Type_string,
    Type_array,
    Type_tuple,
    Type_pair,
    Type_function,
    Type_file,
    Type_pointer,
};

std::vector<std::string> keyWords={
    "func"
};

enum NodeType {
    Node_ASTnode,
    Node_real,
    Node_binOp,
    Node_hash,
    Node_variable,
    Node_call,
    Node_assignment,
    Node_func,
};

//Parent class
class ASTNode {
    public:

        virtual ~ASTNode() = default;
        NodeType m_type;

        virtual int getName() {return m_type;};
};

//Any real number
class RealNode : public ASTNode {
    public:
        NodeType m_type=Node_real;
        virtual int getName() {return m_type;};
        long double real;
};

//Plus, Minus, Divide, Multiplication
class BinaryOpNode : public ASTNode {
    public:
        NodeType m_type=Node_binOp;
        virtual int getName() {return m_type;};
        Tokens operate;
        std::unique_ptr<ASTNode> left;
        std::unique_ptr<ASTNode> right;
};

//Allows Strings to turn into identifiers
class HashNode : public ASTNode {
    public:
        NodeType m_type=Node_hash;
        virtual int getName() {return m_type;};
        std::string Token_indentifier;
};

//Variables
class VariableNode : public ASTNode {
    public:
        NodeType m_type=Node_variable;
        virtual int getName() {return m_type;};
        std::string identifier;
};

//Calls a function "indentifier(args)"
class CallNode : public ASTNode {
    public:
        NodeType m_type=Node_call;
        virtual int getName() {return m_type;};
        std::unique_ptr<VariableNode> identifier;
        std::vector<std::shared_ptr<ASTNode>> args;
};

//Assigns a value to a variable
class AssignmentNode : public ASTNode {
    public:
        NodeType m_type=Node_assignment;
        virtual int getName() {return m_type;};
        std::unique_ptr<VariableNode> identifier;
        std::unique_ptr<ASTNode> value;
        Types type=Type_auto;
};

//Creates a function
class FuncNode : public ASTNode {
    public:
        NodeType m_type=Node_func;
        virtual int getName() {return m_type;};
        std::unique_ptr<VariableNode> identifier;
        Types type=Type_auto;
        std::vector<std::tuple<std::string,Types,std::unique_ptr<ASTNode>>> params;
};
