#include "token_types.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <memory>
#include <utility>


//Hasher
inline constexpr auto hash_djb2a(const string_view sv) {
    unsigned long hash{ 5381 };
    for (unsigned char c : sv) {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}
 
//Turns it into a method
inline constexpr auto operator"" _sh(const char *str, size_t len) {
    return hash_djb2a(string_view{ str, len });
}


//Parses into an ATS
AST_vector parser(Token_vector t_program) {
    //Declaration
    AST_vector node_tree;
    Node_package _parsed;
    Node_package _parsedn;
    int t_counter=0;
    
    //Parser functions
    struct parsers {

        //Function that dectects which node to parse
        static Node_package parse_left(Token_vector t_program,int t_counter) {
            Node_package returner;
            switch (t_program[t_counter].first) {
                case Token_indentifier:
                    if (find(keyWords.begin(),keyWords.end(),t_program[t_counter].second)!=keyWords.end()) {
                        returner=parse_keywords(t_program,t_counter);
                    } else

                    if (t_program[t_counter+1].first==Token_assignment || (t_program[t_counter+1].first==Token_typeIdentifier && t_program[t_counter+2].first==Token_indentifier && t_program[t_counter+3].first==Token_assignment)) {
                        returner=parse_assignment(t_program,t_counter);
                    } else
                    
                    if (t_program[t_counter+1].first==Token_o_paren) {
                        returner=parse_callExpression(t_program,t_counter);
                    }
                break;

                default:
                    throw invalid_argument("I really don't think it's possible to get this error message. If you're getting this you're doing somethhing seriously wrong with this: "+t_program[t_counter].second);
                break;
            };
            t_counter=returner.second;

            return move(returner);
        }

        //Parse a right expression
        static Node_package parse_right(Token_vector t_program,int t_counter,unique_ptr<ASTNode> t_node=nullptr) {
            Node_package returner;
            switch (t_program[t_counter].first) {
                case Token_indentifier:
                    if (t_program[t_counter+1].first==Token_o_paren) {
                        returner=parse_callExpression(t_program,t_counter);
                    }
                break;

                case Token_real:
                    returner=parse_real(t_program,t_counter);
                break;

                case Token_string:
                    returner=parse_string(t_program,t_counter);
                break;
                
                case Token_o_paren:
                    t_counter++;
                    returner=parsers::parse_right(t_program,t_counter);
                    returner.second++;
                break;

                default:
                    throw invalid_argument("Unexpected Token: "+t_program[t_counter].second);
                break;
            };
            t_counter=returner.second;
            
            if (parsers::check_binOp(t_program[t_counter].first)==2) {
                returner=parse_multBinOps(t_program,t_counter,move(returner.first));

                t_counter=returner.second;
                if (parsers::check_binOp(t_program[t_counter].first)==1) {
                    returner=parse_binOps(t_program,t_counter,move(returner.first));
                }

            } else if (parsers::check_binOp(t_program[t_counter].first)==1) {
                returner=parse_binOps(t_program,t_counter,move(returner.first));

            }

            return move(returner);
        }

        //Parse a right expression in a BinOp
        static Node_package parse_right_inBinOP(const Token_vector& t_program,int t_counter,unique_ptr<ASTNode> t_node=nullptr) {
            Node_package returner;
            switch (t_program[t_counter].first) {
                case Token_indentifier:
                    if (t_program[t_counter+1].first==Token_o_paren) {
                        returner=parse_callExpression(t_program,t_counter);
                    }
                break;

                case Token_real:
                    returner=parse_real(t_program,t_counter);
                break;
                
                case Token_o_paren:
                    t_counter++;
                    returner=parsers::parse_right(t_program,t_counter);
                    returner.second++;
                break;

                default:
                    throw invalid_argument("Unexpected Token: "+t_program[t_counter].second);
                break;
            };
            t_counter=returner.second;

            if (parsers::check_binOp(t_program[t_counter].first)==2) {
                returner=parse_multBinOps(t_program,t_counter,move(returner.first));
            }

            return move(returner);
        }

        //Parse a right expression in a MultBinOp
        static Node_package parse_right_inMultBinOP(Token_vector t_program,int t_counter,unique_ptr<ASTNode> t_node=nullptr) {
            Node_package returner;
            switch (t_program[t_counter].first) {
                case Token_indentifier:
                    if (t_program[t_counter+1].first==Token_o_paren) {
                        returner=parse_callExpression(t_program,t_counter);
                    }
                break;

                case Token_real:
                    returner=parse_real(t_program,t_counter);
                break;
                
                case Token_o_paren:
                    t_counter++;
                    returner=parsers::parse_right(t_program,t_counter);
                    returner.second++;
                break;

                default:
                    throw invalid_argument("Unexpected Token: "+t_program[t_counter].second);
                break;
            };

            return move(returner);
        }

        //Parse a Keyword
        static Node_package parse_keywords(Token_vector t_program,int t_counter) {
            Node_package _returner;

            switch (hash_djb2a(t_program[t_counter].second)) {
                case "if"_sh:
                    _returner=parsers::parse_if(t_program,t_counter);
                break;
                
                case "while"_sh:
                    _returner=parsers::parse_while(t_program,t_counter);
                break;

                case "do"_sh:
                    _returner=parsers::parse_do(t_program,t_counter);
                break;
            }

            return _returner;
        }

        //Check the Symbol
        static int check_binOp(int _token) {
            switch (_token) {
                case Token_multiplication:
                case Token_division:
                    return 2;
                break;

                case Token_addition:
                case Token_subtraction:
                    return 1;
                break;

                default:
                    return 0;
                break;
            }
        }

        //Parse Real Numbers
        static pair<unique_ptr<RealNode>,int> parse_real(Token_vector t_program,int t_counter) {
            RealNode __temp;
            __temp.real=stold(t_program[t_counter++].second);
            
            return make_pair(make_unique<RealNode>(move(__temp)),t_counter);
        }

        //Parse String
        static pair<unique_ptr<StringNode>,int> parse_string(Token_vector t_program,int t_counter) {
            StringNode __temp;
            __temp._str=t_program[t_counter++].second;
            
            return make_pair(make_unique<StringNode>(move(__temp)),t_counter);
        }

        //Data Types
        static Types get_type(string token_str) {
            Types returner;
            switch (hash_djb2a(token_str)) {
                case "void"_sh:
                    returner=Type_void;
                break;

                case "auto"_sh:
                    returner=Type_real;
                break;

                case "bool"_sh:
                    returner=Type_boolean;
                break;

                case "real"_sh:
                    returner=Type_real;
                break;

                case "str"_sh:
                    returner=Type_string;
                break;

                case "array"_sh:
                    returner=Type_array;
                break;

                case "tuple"_sh:
                    returner=Type_tuple;
                break;

                case "pair"_sh:
                    returner=Type_pair;
                break;

                case "scr"_sh:
                    returner=Type_function;
                break;

                case "file"_sh:
                    returner=Type_file;
                break;

                case "ptr"_sh:
                    returner=Type_pointer;
                break;

                default:
                    throw invalid_argument("Invalid datatype: "+token_str);
                break;
            }
            return returner;
        }

        //Parse Assignments
        static pair<unique_ptr<AssignmentNode>,int> parse_assignment(Token_vector t_program,int t_counter) {
            AssignmentNode __temp;
            VariableNode __iden;
            __iden.identifier=t_program[t_counter].second;
            __temp.identifier=make_unique<VariableNode>(__iden);

            t_counter++;

            if (t_program[t_counter].first==Token_typeIdentifier) {
                __temp.type=get_type(t_program[t_counter+1].second);
                t_counter+=2;
            }

            t_counter++;

            Node_package __parsed=parsers::parse_right(t_program,t_counter);
            __temp.value=move(__parsed.first);
            t_counter=__parsed.second;
            
            return make_pair(make_unique<AssignmentNode>(move(__temp)),t_counter);
        }

        //Parses Function Calls
        static pair<unique_ptr<CallNode>,int> parse_callExpression(Token_vector t_program,int t_counter ) {
            CallNode __temp;
            VariableNode __iden;
            __iden.identifier=t_program[t_counter].second;
            __temp.identifier=make_unique<VariableNode>(__iden);
            t_counter++;
            
            if (t_program[t_counter+1].first!=Token_c_paren)
            do {
                t_counter++;
                Node_package __parsed=parsers::parse_right(t_program,t_counter);
                __temp.args.emplace_back(move(__parsed.first));
                t_counter=__parsed.second;
            } while (t_program[t_counter].first!=Token_c_paren);

            t_counter++;

            return make_pair(make_unique<CallNode>(move(__temp)),t_counter);
        }

        //Parses Binary Operations
        static pair<unique_ptr<BinaryOpNode>,int> parse_binOps(Token_vector t_program,int t_counter,unique_ptr<ASTNode> _left) {
            BinaryOpNode __temp;
            __temp.operate=Tokens(t_program[t_counter].first);
            __temp.left=move(_left);

            t_counter++;

            Node_package r_parsed=parsers::parse_right_inBinOP(t_program,t_counter);
            t_counter=r_parsed.second;
            __temp.right=move(r_parsed.first);

            pair<unique_ptr<BinaryOpNode>,int> __returner;

            if (parsers::check_binOp(t_program[t_counter].first)>=1) {
                __returner=parsers::parse_binOps(t_program,t_counter,move(make_unique<BinaryOpNode>(move(__temp))));
            
            } else __returner=make_pair(make_unique<BinaryOpNode>(move(__temp)),t_counter);

            return __returner;
        }

        //Parses Mult Binary Operations
        static pair<unique_ptr<BinaryOpNode>,int> parse_multBinOps(Token_vector t_program,int t_counter,unique_ptr<ASTNode> _left) {
            BinaryOpNode __temp;
            __temp.operate=Tokens(t_program[t_counter].first);
            __temp.left=move(_left);

            t_counter++;

            Node_package r_parsed=parsers::parse_right_inMultBinOP(t_program,t_counter);
            t_counter=r_parsed.second;
            __temp.right=move(r_parsed.first);

            pair<unique_ptr<BinaryOpNode>,int> __returner;

            if (check_binOp(t_program[t_counter].first)>=2) {
                __returner=parsers::parse_multBinOps(t_program,t_counter,move(make_unique<BinaryOpNode>(move(__temp))));
            
            } else __returner=make_pair(make_unique<BinaryOpNode>(move(__temp)),t_counter);

            return __returner;
        }

        //Parses Subprogram
        static pair<AST_vector,int> parse_subprogram(Token_vector t_program,int t_counter) {
            AST_vector s_tree;
            Node_package s_parsed;
            if (t_program[t_counter].first==Token_c_o_paren) {
                t_counter++;
                if (t_program[t_counter].first==Token_newline) t_counter++;
                
                do {
                    s_parsed=parsers::parse_left(t_program,t_counter);
                    t_counter=s_parsed.second;

                    if (t_program[t_counter].first==Token_c_c_paren) {
                        break;
                    } else if (t_program[t_counter].first!=Token_newline) {
                        throw invalid_argument("Unexpected token: "+t_program[t_counter].second);
                    } t_counter++;

                    s_tree.emplace_back(move(s_parsed.first));
                } while (t_program[t_counter].first!=Token_c_c_paren);

                t_counter++;

            } else {
                s_parsed=parsers::parse_left(t_program,t_counter);
                t_counter=s_parsed.second;
                s_tree.emplace_back(move(s_parsed.first));
            }

            return make_pair(move(s_tree),t_counter);
        }

        //Parses If Statements
        static pair<unique_ptr<IfNode>,int> parse_if(Token_vector t_program,int t_counter) {
            IfNode __temp;

            t_counter++;

            Node_package con_parsed=parsers::parse_right(t_program, t_counter);
            __temp.expression=move(con_parsed.first);
            t_counter=con_parsed.second;

            if (t_program[t_counter].first==Token_newline) t_counter++;

            pair<AST_vector,int> sub_parsed=parsers::parse_subprogram(t_program,t_counter);
            __temp.program=move(sub_parsed.first);
            t_counter=sub_parsed.second;

            if ((t_program[t_counter].first==Token_indentifier&&t_program[t_counter].second=="else")||(t_program[t_counter].first==Token_newline&&(t_program[t_counter+1].first==Token_indentifier&&t_program[t_counter+1].second=="else"))) {
                if (t_program[t_counter].first==Token_newline) t_counter++;
                t_counter++;
                if (t_program[t_counter].first==Token_newline) t_counter++;

                __temp.elsed=true;
                sub_parsed=parsers::parse_subprogram(t_program,t_counter);
                __temp.else_program=move(sub_parsed.first);
                t_counter=sub_parsed.second;
            }
            
            return make_pair(make_unique<IfNode>(move(__temp)),t_counter);
        }

        //Parses While Loops
        static pair<unique_ptr<WhileNode>,int> parse_while(Token_vector t_program,int t_counter) {
            WhileNode __temp;
            __temp.top=true;

            t_counter++;

            Node_package con_parsed=parsers::parse_right(t_program, t_counter);
            __temp.expression=move(con_parsed.first);
            t_counter=con_parsed.second;

            if (t_program[t_counter].first==Token_newline) t_counter++;

            pair<AST_vector,int> sub_parsed=parsers::parse_subprogram(t_program,t_counter);
            __temp.program=move(sub_parsed.first);
            t_counter=sub_parsed.second;
            
            return make_pair(make_unique<WhileNode>(move(__temp)),t_counter);
        }

        //Parses Do Loops
        static pair<unique_ptr<WhileNode>,int> parse_do(Token_vector t_program,int t_counter) {
            WhileNode __temp;
            __temp.top=false;

            if (t_program[t_counter].first==Token_newline) t_counter++;

            pair<AST_vector,int> sub_parsed=parsers::parse_subprogram(t_program,t_counter);
            __temp.program=move(sub_parsed.first);
            t_counter=sub_parsed.second;

            if (t_program[t_counter].first==Token_newline) t_counter++;
            if (!(t_program[t_counter].first==Token_indentifier&&t_program[t_counter].second=="else")) throw invalid_argument("Expected: while\n got: " + t_program[t_counter].second);
            t_counter++;

            Node_package con_parsed=parsers::parse_right(t_program, t_counter);
            __temp.expression=move(con_parsed.first);
            t_counter=con_parsed.second;

            
            return make_pair(make_unique<WhileNode>(move(__temp)),t_counter);
        }

        //Parses Function Declaration
        static pair<unique_ptr<FuncNode>,int> parse_func(Token_vector t_program,int t_counter) {
            FuncNode __temp;

            t_counter++;
            
            Node_package iden_parsed=parsers::parse_right(t_program, t_counter);
            __temp.identifier=;
            VariableNode __iden;
            __iden.identifier=move(iden_parsed.first);
            Node_package con_parsed=parsers::parse_right(t_program, t_counter);
            __temp.expression=move(con_parsed.first);
            t_counter=con_parsed.second;

            if (t_program[t_counter].first==Token_newline) t_counter++;

            pair<AST_vector,int> sub_parsed=parsers::parse_subprogram(t_program,t_counter);
            __temp.program=move(sub_parsed.first);
            t_counter=sub_parsed.second;

            if ((t_program[t_counter].first==Token_indentifier&&t_program[t_counter].second=="else")||(t_program[t_counter].first==Token_newline&&(t_program[t_counter+1].first==Token_indentifier&&t_program[t_counter+1].second=="else"))) {
                if (t_program[t_counter].first==Token_newline) t_counter++;
                t_counter++;
                if (t_program[t_counter].first==Token_newline) t_counter++;

                __temp.elsed=true;
                sub_parsed=parsers::parse_subprogram(t_program,t_counter);
                __temp.else_program=move(sub_parsed.first);
                t_counter=sub_parsed.second;
            }
            
            return make_pair(make_unique<IfNode>(move(__temp)),t_counter);
        }
    };

    //Parsing loop
    do {
        _parsed=parsers::parse_left(t_program,t_counter);
        t_counter=_parsed.second;
        if (t_program[t_counter].first!=Token_newline) {
            throw invalid_argument("Unexpected token: "+t_program[t_counter].second);
        } t_counter++;
        node_tree.emplace_back(move(_parsed.first));
    } while (t_counter<t_program.size());

    return node_tree;
}
