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

enum Nodes {
    Node_real,
    Node_binaryop,
    Node_hash,
    Node_variable,
    Node_call,
    Node_assignment,
};

class ASTNode {
    public:
        virtual ~ASTNode() = default;
        int name;
        //virtual void placeholder() {}
    // long double real;
    // int operate;
    // std::unique_ptr<ASTNode> left;
    // std::unique_ptr<ASTNode> right;
    // std::string identifier;
    // std::vector<std::unique_ptr<ASTNode>> args;
    // std::unique_ptr<VariableNode> variable;
    // std::unique_ptr<ASTNode> value;
};

class RealNode : public ASTNode {
    public:
        int name=Node_real;
        long double real;
};

class BinaryOpNode : public ASTNode {
    public:
        int name=Node_binaryop;
        int operate;
        std::unique_ptr<ASTNode> left;
        std::unique_ptr<ASTNode> right;
};

class HashNode : public ASTNode {
    public:
        int name=Node_hash;
};

class VariableNode : public ASTNode {
    public:
        int name=Node_variable;
        std::string identifier;
};
class CallNode : public ASTNode {
    public:
        int name=Node_call;
        std::string identifier;
        std::vector<std::unique_ptr<ASTNode>> args;
};

class AssignmentNode : public ASTNode {
    public:
        int name=Node_assignment;
        std::unique_ptr<VariableNode> variable;
        std::unique_ptr<ASTNode> value;
};








//template<typename name=std::string,std::string value > using 
// typedef struct{std::string name} Token_string;
// struct Token_assignment {std::string name="Token_assignment";std::string value;};
// struct Token_real {std::string name="Token_real";std::string value;};
// struct Token_string {std::string name="Token_assignment";std::string value;};
// struct Token_addition {std::string name="";value="";};
// struct Token_subtraction {std::string name="";value="";};
// struct Token_multiplication {std::string name="";value="";};
// struct Token_division {std::string name="";value="";};
// struct Token_o_paren {std::string name="";value="";};
// struct Token_c_paren {std::string name="";value="";};
// struct Token_s_o_paren {std::string name="";value="";};
// struct Token_s_c_paren {std::string name="";value="";};
// struct Token_c_o_paren {std::string name="";value="";};
// struct Token_c_c_paren {std::string name="";value="";};

//std::variant<>;

// int main() {
//     std::cout << Token_string;

//     return 0;
// }
