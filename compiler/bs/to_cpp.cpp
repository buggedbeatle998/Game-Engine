#include "parser.cpp"
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <memory>

//Very unfinished

//Hasher
inline constexpr auto hash_djb2a(const std::string_view sv) {
    unsigned long hash{ 5381 };
    for (unsigned char c : sv) {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}
 
//Turns it into a method
inline constexpr auto operator"" _sh(const char *str, size_t len) {
    return hash_djb2a(std::string_view{ str, len });
}

//Turns to AST into cpp
std::string _to_cpp(std::vector<std::unique_ptr<ASTNode>> ast_tree) {
    std::string _cpp;

    //Emitter functions
    struct emmiters {

        //Indentifies the Node Type
        static std::string emit(std::shared_ptr<ASTNode> ast_node_ptr) {

            //Detected a Function Call
            if (std::dynamic_pointer_cast<CallNode>(ast_node_ptr)!=nullptr) {
                return emit_call(ast_node_ptr);
            }
            
            //Detected a Real
            RealNode __temp;
            __temp.real=5;
            std::shared_ptr<ASTNode> _cheese=std::make_shared<RealNode> (__temp);

            if (std::dynamic_pointer_cast<RealNode>(ast_node_ptr)!=nullptr) {
                return emit_real(ast_node_ptr);
            }

            return "";
        }

        //Function Call Node
        static std::string emit_call(std::shared_ptr<ASTNode> ast_node_ptr) {
            CallNode* ast_node=dynamic_cast<CallNode*>(ast_node_ptr.get());
            switch (hash_djb2a((*ast_node).identifier)) {
                case ("print"_sh):
                    return "std::cout << "+emmiters::emit(std::move((*ast_node).args[0]))+";";
                break;

                default:
                    return "";
                break;
            }
            return "";
        }

        //Real Node
        static std::string emit_real(std::shared_ptr<ASTNode> ast_node_ptr) {
            RealNode* ast_node=dynamic_cast<RealNode*>(ast_node_ptr.get());
            return std::to_string((*ast_node).real);
        }
    };
    
    //Emitter loop
    do {
        std::shared_ptr<ASTNode> __test=move(ast_tree[0]);
        ast_tree.erase(ast_tree.begin());
        _cpp=emmiters::emit(__test);
    } while (ast_tree.size()>0);

    return _cpp;
}

//Test
int main() {
    std::string stringed;
    std::getline(std::cin >> std::ws, stringed);
    std::string tokened=_to_cpp(parser(stringed));
    std::cout << tokened;
    
    return 0;
}