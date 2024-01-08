#include <string>
#include <variant>
#include <iostream>

enum Tokens {
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

class ASTNode {

};

class RealNode : public ASTNode {
    public:
        long double real;
};

class BinaryOpNode : public ASTNode {
    public:
        int operate;
        std::unique_ptr<ASTNode> left;
        std::unique_ptr<ASTNode> right;
};

class HashNode : public ASTNode {
};

class VariableNode : public ASTNode {
    public:
        std::string identifier;
};
class CallNode : public ASTNode {
    public:
        std::string identifier;
        std::vector<std::unique_ptr<ASTNode>> args;
};

class AssignmentNode : public ASTNode {
    public:
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
