#include "parser.cpp"
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <memory>

inline constexpr auto hash_djb2a(const std::string_view sv) {
    unsigned long hash{ 5381 };
    for (unsigned char c : sv) {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}
 
inline constexpr auto operator"" _sh(const char *str, size_t len) {
    return hash_djb2a(std::string_view{ str, len });
}

std::string _to_cpp(std::vector<std::unique_ptr<ASTNode>> ast_tree) {
    std::string _cpp;

    struct emmiters {
        static std::string emit(std::unique_ptr<ASTNode> ast_node_ptr) {
            //std::unique_ptr<ASTNode> __temped=ast_node_ptr;
            //ASTNode ast_node=*ast_node_ptr;
            switch ((*ast_node_ptr).name) {
                case Node_call:
                    return emit_call(std::move(ast_node_ptr));
                break;
            }
            return "";
        }

        static std::string emit_call(std::unique_ptr<ASTNode> ast_node_ptr) {
            CallNode* ast_node=dynamic_cast<CallNode*>(ast_node_ptr.get());
            switch (hash_djb2a((*ast_node).identifier)) {
                case ("print"_sh):
                    std::cout << "did it!";
                    return "std::cout <<";
                break;

                default:
                    return "";
                break;
            }
            return "";
        }
    };

    do {
        std::unique_ptr<ASTNode> __test=move(ast_tree[0]);
        _cpp=emmiters::emit(move(ast_tree[0]));
    } while (true);

    return _cpp;
}

int main() {
    std::string stringed;
    std::getline(std::cin >> std::ws, stringed);
    std::string tokened=_to_cpp(parser(stringed));
    
    return 0;
}