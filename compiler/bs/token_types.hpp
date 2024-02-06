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
};

enum NodeType {
    Node_ASTnode,
    Node_real,
    Node_binOp,
    Node_hash,
    Node_variable,
    Node_call,
    Node_assignment
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
};
