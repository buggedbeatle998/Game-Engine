#include "parser.cpp"

#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <memory>


std::string get_cpptype(Types token_str) {
    std::string returner;
    switch (token_str) {
        case Type_undefined:
            returner="void";
        break;

        case Type_noone:
            returner="bool";
        break;

        case Type_auto:
            returner="auto";
        break;

        case Type_boolean:
            returner="bool";
        break;

        case Type_real:
            returner="long double";
        break;

        case Type_string:
            returner="std::string";
        break;

        case "array"_sh:
            returner="std::vector<std::any>";
        break;

        case "tuple"_sh:
            returner="std::tuple<std::any>";
        break;

        case "pair"_sh:
            returner="std::pair<std::any,std::any>";
        break;

        case "scr"_sh:
            returner="std::function";
        break;

        case "file"_sh:
            returner="ifstream";
        break;

        case "ptr"_sh:
            returner="std::any*";
        break;

        default:
            throw std::invalid_argument("Invalid datatype: "+token_str);
        break;
    }
    return returner;
}


//Turns to AST into cpp
std::string to_cpp(std::vector<std::unique_ptr<ASTNode>> ast_tree) {
    std::string _cpp;

    //Emitter functions
    struct emmiters {

        //Indentifies the Node Type
        static std::string emit(std::shared_ptr<ASTNode> ast_node_ptr) {

            // //Detected a Real
            // if (std::dynamic_pointer_cast<RealNode>(ast_node_ptr)!=nullptr) {
            //     return emit_real(ast_node_ptr);
            // }

            // //Detected an Assignment
            // if (std::dynamic_pointer_cast<AssignmentNode>(ast_node_ptr)!=nullptr) {
            //     return emit_assignment(ast_node_ptr);
            // }

            // //Detected a Function Call
            // if (std::dynamic_pointer_cast<CallNode>(ast_node_ptr)!=nullptr) {
            //     return emit_call(ast_node_ptr);
            // }

            // //Detected a Binary Operations
            // if (std::dynamic_pointer_cast<BinaryOpNode>(ast_node_ptr)!=nullptr) {
            //     return emit_binOp(ast_node_ptr);
            // }

            switch (dynamic_cast<ASTNode*>(ast_node_ptr.get())->getName()) {
                //Detected a Real
                case Node_real:
                    return emit_real(ast_node_ptr);
                break;

                //Detected an Assignment
                case Node_assignment:
                    return emit_assignment(ast_node_ptr);
                break;

                //Detected a Function Call
                case Node_call:
                    return emit_call(ast_node_ptr);
                break;

                //Detected a Binary Operations
                case Node_binOp:
                    return emit_binOp(ast_node_ptr);
                break;

                default:
                    return "";
                break;
            }

        }

        //Real Node
        static std::string emit_real(std::shared_ptr<ASTNode> ast_node_ptr) {
            RealNode* ast_node=dynamic_cast<RealNode*>(ast_node_ptr.get());

            return std::to_string((*ast_node).real);
        }

        //Assignment Node
        static std::string emit_assignment(std::shared_ptr<ASTNode> ast_node_ptr) {
            AssignmentNode* ast_node=dynamic_cast<AssignmentNode*>(ast_node_ptr.get());
            std::shared_ptr __value=std::move((*ast_node).value);

            std::string str_r=get_cpptype(ast_node->type)+" "+((*ast_node).identifier.get()->identifier)+" = "+emmiters::emit(__value);

            return str_r;
        }

        //Function Call Node
        static std::string emit_call(std::shared_ptr<ASTNode> ast_node_ptr) {
            CallNode* ast_node=dynamic_cast<CallNode*>(ast_node_ptr.get());

            switch (hash_djb2a(ast_node->identifier.get()->identifier)) {
                case ("print"_sh):
                    return "std::cout << "+emmiters::emit(std::move(ast_node->args[0]));
                break;

                case ("div"_sh):
                    return "div("+emmiters::emit(std::move(ast_node->args[0]))+","+emmiters::emit(std::move(ast_node->args[1]))+")";
                break;

                default:
                    return "";
                break;
            }

            return "";
        }

        //Binary Operator Node
        static std::string emit_binOp(std::shared_ptr<ASTNode> ast_node_ptr) {
            BinaryOpNode* ast_node=dynamic_cast<BinaryOpNode*>(ast_node_ptr.get());

            std::string r_str="("+emmiters::emit(std::move(ast_node->left));
            switch (ast_node->operate) {
                case (Token_addition):
                    r_str+=" + ";
                break;

                case (Token_subtraction):
                    r_str+=" - ";
                break;

                case (Token_multiplication):
                    r_str+=" * ";
                break;

                case (Token_division):
                    r_str+=" / ";
                break;

                default:
                    throw std::invalid_argument("Undefined Operator");
                break;
            }
            r_str+=emmiters::emit(std::move(ast_node->right))+")";

            return r_str;
        }
    };
    
    //Emitter loop
    do {
        std::shared_ptr<ASTNode> __test=move(ast_tree[0]);
        ast_tree.erase(ast_tree.begin());
        _cpp+=emmiters::emit(__test)+";";
    } while (ast_tree.size()>0);

    return _cpp;
}

//Test
// int main() {
//     std::string stringed;
//     std::getline(std::cin >> std::ws, stringed);
//     std::string tokened=to_cpp(parser(to_token(stringed)));
//     std::cout << tokened;
    
//     return 0;
// }