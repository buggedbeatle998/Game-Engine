#include "to_cpp.hpp"

#include "token_types.hpp"
#include "util.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <memory>


// Turns to AST into cpp
string to_cpp(AST_vector ast_tree) {
    string _cpp;

    // Emitter functions
    struct emitters {

        // Indentifies the Node Type
        static string emit(shared_ptr<ASTNode> ast_node_ptr) {
            switch (dynamic_cast<ASTNode*> (ast_node_ptr.get())->getName()) {
                // Detected a Real
                case Node_real:
                    return emit_real(ast_node_ptr);
                break;
                
                // Detected a String
                case Node_string:
                    return emit_string(ast_node_ptr);
                break;

                // Detected a Chunk
                case Node_chunk:
                    return emit_chunk(ast_node_ptr);
                break;

                // Detected an Assignment
                case Node_assignment:
                    return emit_assignment(ast_node_ptr);
                break;

                // Detected a Function Call
                case Node_call:
                    return emit_call(ast_node_ptr);
                break;

                // Detected a Binary Operations
                case Node_binOp:
                    return emit_binOp(ast_node_ptr);
                break;

                // Detected a If Statement
                case Node_if:
                    return emit_if(ast_node_ptr);
                break;

                // Detected a While Loops
                case Node_while:
                    return emit_while(ast_node_ptr);
                break;

                default:
                    return "";
                break;
            }
        }

        // Real Node
        static string emit_real(shared_ptr<ASTNode> ast_node_ptr) {
            RealNode* ast_node = dynamic_cast<RealNode*> (ast_node_ptr.get());

            return to_string((*ast_node).real);
        }

        // String Node
        static string emit_string(shared_ptr<ASTNode> ast_node_ptr) {
            StringNode* ast_node = dynamic_cast<StringNode*> (ast_node_ptr.get());

            return "\"" + ast_node->_str + "\"";
        }        

        // Chunk Node
        static string emit_chunk(shared_ptr<ASTNode> ast_node_ptr) {
            ChunkNode* ast_node = dynamic_cast<ChunkNode*> (ast_node_ptr.get());
            string str_r = "{";
            AST_vector sub_tree = move(ast_node->program);
            
            for (int i = 0; i < sub_tree.size(); i++) {
                str_r += emitters::emit(move(sub_tree[i])) + ";";
            }

            str_r += "}";

            return str_r;
        }

        // Assignment Node
        static string emit_assignment(shared_ptr<ASTNode> ast_node_ptr) {
            AssignmentNode* ast_node = dynamic_cast<AssignmentNode*> (ast_node_ptr.get());
            shared_ptr __value = move((*ast_node).value);

            string str_r = get_cpptype(ast_node->type) + " " + ((*ast_node).identifier.get()->identifier) + " = " + emitters::emit(__value);

            return str_r;
        }

        // Function Call Node
        static string emit_call(shared_ptr<ASTNode> ast_node_ptr) {
            CallNode* ast_node = dynamic_cast<CallNode*> (ast_node_ptr.get());

            switch (hash_djb2a(ast_node->identifier.get()->identifier)) {
                case ("print"_sh):
                    return "printf(" + emitters::emit(move(ast_node->args[0])) + ")";
                break;

                case ("div"_sh):
                    return "div(" + emitters::emit(move(ast_node->args[0])) + ", " + emitters::emit(move(ast_node->args[1])) + ")";
                break;

                case ("pow"_sh):
                    return "pow(" + emitters::emit(move(ast_node->args[0])) + ", " + emitters::emit(move(ast_node->args[1])) + ")";
                break;

                default:
                    std::string _str = (ast_node->identifier.get()->identifier);
                    for (int i = 0; i < (ast_node->args).size(); i++) {
                        if (i == 0)
                            _str += "(";
                        else
                            _str += ", ";
                        _str += emitters::emit(move(ast_node->args[i]));
                    }
                    _str += ")";
                    
                    return _str;
                break;
            }

            return "";
        }

        // Binary Operator Node
        static string emit_binOp(shared_ptr<ASTNode> ast_node_ptr) {
            BinaryOpNode* ast_node = dynamic_cast<BinaryOpNode*> (ast_node_ptr.get());

            string str_r = "(" + emitters::emit(move(ast_node->left));
            switch (ast_node->operate) {
                case (Token_addition):
                    str_r += " + ";
                break;

                case (Token_subtraction):
                    str_r += " - ";
                break;

                case (Token_multiplication):
                    str_r += " * ";
                break;

                case (Token_division):
                    str_r += " / ";
                break;

                default:
                    throw invalid_argument("Undefined Operator");
                break;
            }
            str_r += emitters::emit(move(ast_node->right)) + ")";

            return str_r;
        }

        // If Node
        static string emit_if(shared_ptr<ASTNode> ast_node_ptr) {
            IfNode* ast_node = dynamic_cast<IfNode*> (ast_node_ptr.get());
            string str_r = "if (" + emitters::emit(move(ast_node->expression)) + ") ";

            str_r += emitters::emit(move(ast_node->program));

            if (ast_node->elsed) {
                str_r += " else " + emitters::emit(move(ast_node->else_program));
            }

            return str_r;
        }

        // While Node
        static string emit_while(shared_ptr<ASTNode> ast_node_ptr) {
            WhileNode* ast_node = dynamic_cast<WhileNode*> (ast_node_ptr.get());
            string str_r;

            if (ast_node->top) {
                str_r = "while (" + emitters::emit(move(ast_node->expression)) + ") ";
                str_r += emitters::emit(move(ast_node->program));
            } else {
                str_r = "do ";
                str_r += emitters::emit(move(ast_node->program));
                str_r += " while (" + emitters::emit(move(ast_node->expression)) + ")";
            }

            return str_r;
        }
    };
    
    // Emitter loop
    do {
        shared_ptr<ASTNode> __test = move(ast_tree[0]);
        ast_tree.erase(ast_tree.begin());
        _cpp += emitters::emit(__test) + ";";
    } while (ast_tree.size()>0);

    return _cpp;
}
