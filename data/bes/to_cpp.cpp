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
            ASTNode ast_node=*ast_node_ptr;
            switch (ast_node.name) {
                case Node_call:
                    emit_call(ast_node_ptr);
                break;
            }
        }

        static std::string emit_call(std::unique_ptr<ASTNode> ast_node_ptr) {
            CallNode ast_node=*ast_node_ptr;
            switch (hash_djb2a(ast_node.identifier)) {
                case ("print"_sh):
                    return "std::cout <<";
                break;
            }
        }
    };

    return _cpp;
}

int main() {
    return 0;
}