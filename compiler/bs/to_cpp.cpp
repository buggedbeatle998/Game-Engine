#include "parser.cpp"

#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <memory>


string get_cpptype(Types token_str) {
    string returner;
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
            returner="string";
        break;

        case "array"_sh:
            returner="vector<any>";
        break;

        case "tuple"_sh:
            returner="tuple<any>";
        break;

        case "pair"_sh:
            returner="pair<any,any>";
        break;

        case "scr"_sh:
            returner="function";
        break;

        case "file"_sh:
            returner="ifstream";
        break;

        case "ptr"_sh:
            returner="any*";
        break;

        default:
            throw invalid_argument("Invalid datatype: "+token_str);
        break;
    }
    return returner;
}


//Turns to AST into cpp
string to_cpp(vector<unique_ptr<ASTNode>> ast_tree) {
    string _cpp;

    //Emitter functions
    struct emmiters {

        //Indentifies the Node Type
        static string emit(shared_ptr<ASTNode> ast_node_ptr) {

            // //Detected a Real
            // if (dynamic_pointer_cast<RealNode>(ast_node_ptr)!=nullptr) {
            //     return emit_real(ast_node_ptr);
            // }

            // //Detected an Assignment
            // if (dynamic_pointer_cast<AssignmentNode>(ast_node_ptr)!=nullptr) {
            //     return emit_assignment(ast_node_ptr);
            // }

            // //Detected a Function Call
            // if (dynamic_pointer_cast<CallNode>(ast_node_ptr)!=nullptr) {
            //     return emit_call(ast_node_ptr);
            // }

            // //Detected a Binary Operations
            // if (dynamic_pointer_cast<BinaryOpNode>(ast_node_ptr)!=nullptr) {
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
        static string emit_real(shared_ptr<ASTNode> ast_node_ptr) {
            RealNode* ast_node=dynamic_cast<RealNode*>(ast_node_ptr.get());

            return to_string((*ast_node).real);
        }

        //Assignment Node
        static string emit_assignment(shared_ptr<ASTNode> ast_node_ptr) {
            AssignmentNode* ast_node=dynamic_cast<AssignmentNode*>(ast_node_ptr.get());
            shared_ptr __value=move((*ast_node).value);

            string str_r=get_cpptype(ast_node->type)+" "+((*ast_node).identifier.get()->identifier)+" = "+emmiters::emit(__value);

            return str_r;
        }

        //Function Call Node
        static string emit_call(shared_ptr<ASTNode> ast_node_ptr) {
            CallNode* ast_node=dynamic_cast<CallNode*>(ast_node_ptr.get());

            switch (hash_djb2a(ast_node->identifier.get()->identifier)) {
                case ("print"_sh):
                    return "printf("+emmiters::emit(move(ast_node->args[0]))+")";
                break;

                case ("div"_sh):
                    return "div("+emmiters::emit(move(ast_node->args[0]))+","+emmiters::emit(move(ast_node->args[1]))+")";
                break;

                case ("pow"_sh):
                    return "pow("+emmiters::emit(move(ast_node->args[0]))+","+emmiters::emit(move(ast_node->args[1]))+")";
                break;

                default:
                    return "";
                break;
            }

            return "";
        }

        //Binary Operator Node
        static string emit_binOp(shared_ptr<ASTNode> ast_node_ptr) {
            BinaryOpNode* ast_node=dynamic_cast<BinaryOpNode*>(ast_node_ptr.get());

            string r_str="("+emmiters::emit(move(ast_node->left));
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
                    throw invalid_argument("Undefined Operator");
                break;
            }
            r_str+=emmiters::emit(move(ast_node->right))+")";

            return r_str;
        }
    };
    
    //Emitter loop
    do {
        shared_ptr<ASTNode> __test=move(ast_tree[0]);
        ast_tree.erase(ast_tree.begin());
        _cpp+=emmiters::emit(__test)+";";
    } while (ast_tree.size()>0);

    return _cpp;
}

//Test
// int main() {
//     string stringed;
//     getline(cin >> ws, stringed);
//     string tokened=to_cpp(parser(to_token(stringed)));
//     cout << tokened;
    
//     return 0;
// }