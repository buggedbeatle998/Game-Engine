#include "to_token.cpp"

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
vector<unique_ptr<ASTNode>> parser(vector<pair<int,string>> t_program) {
    //Declaration
    vector<unique_ptr<ASTNode>> node_tree;
    Node_package _parsed;
    Node_package _parsedn;
    int t_counter=0;
    
    //Parser functions
    struct parsers {

        //Function that dectects which node to parse
        static Node_package parse_left(vector<pair<int,string>> t_program,int t_counter) {
            Node_package returner;
            switch (t_program[t_counter].first) {
                case Token_indentifier:
                    if (find(keyWords.begin(),keyWords.end(),t_program[t_counter].second)!=keyWords.end()) {
                        //returner=parse_keyword(t_program,t_counter);
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

            return move(returner);
        }

        //Parse a right expression
        static Node_package parse_right(vector<pair<int,string>> t_program,int t_counter,unique_ptr<ASTNode> t_node=nullptr) {
            Node_package returner;
            switch (t_program[t_counter].first) {
                case Token_newline:
                case Token_c_paren:
                case Token_coma:
                    t_counter++;
                    return make_pair(nullptr,t_counter);
                break;

                case Token_indentifier:
                    if (t_program[t_counter+1].first==Token_o_paren) {
                        returner=parse_callExpression(t_program,t_counter);
                    }
                break;

                case Token_real:
                    returner=parse_real(t_program,t_counter);
                break;

                // case Token_addition:
                // case Token_subtraction:
                // case Token_multiplication:
                // case Token_division:
                //     returner=parsers::parse_binOps(t_program,t_counter,move(t_node));
                //     t_counter=returner.second;
                // break;
                
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
        static Node_package parse_right_inBinOP(const vector<pair<int,string>>& t_program,int t_counter,unique_ptr<ASTNode> t_node=nullptr) {
            Node_package returner;
            switch (t_program[t_counter].first) {
                case Token_newline:
                case Token_c_paren:
                case Token_coma:
                    t_counter++;
                    return make_pair(nullptr,t_counter);
                break;

                case Token_indentifier:
                    if (t_program[t_counter+1].first==Token_o_paren) {
                        returner=parse_callExpression(t_program,t_counter);
                    }
                break;

                case Token_real:
                    returner=parse_real(t_program,t_counter);
                break;

                // case Token_addition:
                // case Token_subtraction:
                // case Token_multiplication:
                // case Token_division:
                //     returner=parsers::parse_binOps(t_program,t_counter,move(t_node));
                //     t_counter=returner.second;
                // break;
                
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
        static Node_package parse_right_inMultBinOP(vector<pair<int,string>> t_program,int t_counter,unique_ptr<ASTNode> t_node=nullptr) {
            Node_package returner;
            switch (t_program[t_counter].first) {
                case Token_newline:
                case Token_c_paren:
                case Token_coma:
                    t_counter++;
                    return make_pair(nullptr,t_counter);
                break;

                case Token_indentifier:
                    if (t_program[t_counter+1].first==Token_o_paren) {
                        returner=parse_callExpression(t_program,t_counter);
                    }
                break;

                case Token_real:
                    returner=parse_real(t_program,t_counter);
                break;

                // case Token_addition:
                // case Token_subtraction:
                // case Token_multiplication:
                // case Token_division:
                //     returner=parsers::parse_binOps(t_program,t_counter,move(t_node));
                //     t_counter=returner.second;
                // break;
                
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
        static Node_package parse_keywords(vector<pair<int,string>> t_program,int t_counter) {
            switch (hash_djb2a(t_program[t_counter].second)) {
                case "if"_sh:
                    
                break;
            }
            Node_package ted;
            return ted;
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
        static pair<unique_ptr<RealNode>,int> parse_real(vector<pair<int,string>> t_program,int t_counter) {
            RealNode __temp;
            __temp.real=stold(t_program[t_counter].second);
            t_counter++;
            
            return make_pair(make_unique<RealNode>(move(__temp)),t_counter);
        }

        //Data Types
        static Types get_type(string token_str) {
            Types returner;
            switch (hash_djb2a(token_str)) {
                case "undefined"_sh:
                    returner=Type_undefined;
                break;

                case "noone"_sh:
                    returner=Type_real;
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
        static pair<unique_ptr<AssignmentNode>,int> parse_assignment(vector<pair<int,string>> t_program,int t_counter) {
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
        static pair<unique_ptr<CallNode>,int> parse_callExpression(vector<pair<int,string>> t_program,int t_counter) {
            CallNode __temp;
            VariableNode __iden;
            __iden.identifier=t_program[t_counter].second;
            __temp.identifier=make_unique<VariableNode>(__iden);
            t_counter++;
            
            if (t_program[t_counter+1].first!=Token_c_paren)
            do {
                t_counter++;
                Node_package __parsed=parsers::parse_right(t_program,t_counter);
                shared_ptr<ASTNode> __arg=move(__parsed.first);
                __temp.args.emplace_back(__arg);
                t_counter=__parsed.second;
            } while (t_program[t_counter].first!=Token_c_paren);

            t_counter++;

            return make_pair(make_unique<CallNode>(move(__temp)),t_counter);
        }

        //Parses Binary Operations
        static pair<unique_ptr<BinaryOpNode>,int> parse_binOps(vector<pair<int,string>> t_program,int t_counter,unique_ptr<ASTNode> _left) {
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
        static pair<unique_ptr<BinaryOpNode>,int> parse_multBinOps(vector<pair<int,string>> t_program,int t_counter,unique_ptr<ASTNode> _left) {
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

        static pair<unique_ptr<RealNode>,int> parse_if(vector<pair<int,string>> t_program,int t_counter) {
            IfNode __temp;

            t_counter++;

            Node_package con_parsed=parsers::parse_right(t_program, t_counter);
            __temp.expression=move(con_parsed.first);

            if (t_program[t_counter].first==Token_newline) t_counter++;

            if (t_program[t_counter++].first==Token_c_o_paren) {
                // while (t_program[t_counter]==Token_c_c_paren)
                // {
                    
                // }
            }
            pair<unique_ptr<RealNode>,int> ted;
            return ted;
        }
    };

    //Parsing loop
    do {
        _parsed=parsers::parse_left(t_program,t_counter);
        t_counter=_parsed.second+1;
        node_tree.emplace_back(move(_parsed.first));
    } while (t_counter<t_program.size());

    return node_tree;
}

//Test
// int main() {
//     string stringed;
//     getline(cin >> ws, stringed);
//     vector tokened=parser(stringed);
//     // RealNode* ast_node=dynamic_cast<RealNode*>(tokened[0].get());
//     // cout << (*ast_node).real;
//     return 0;
// }
