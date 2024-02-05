#include "to_token.cpp"

#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <memory>
#include <utility>


//Parses into an ATS
std::vector<std::unique_ptr<ASTNode>> parser(std::vector<std::pair<int,std::string>> t_program) {
    //Declaration
    std::vector<std::unique_ptr<ASTNode>> node_tree;
    std::pair<std::unique_ptr<ASTNode>,int> _parsed;
    std::pair<std::unique_ptr<ASTNode>,int> _parsedn;
    int t_counter=0;
    
    //Parser functions
    struct parsers {

        //Function that dectects which node to parse
        static std::pair<std::unique_ptr<ASTNode>,int> parse_left(std::vector<std::pair<int,std::string>> t_program,int t_counter) {
            std::pair<std::unique_ptr<ASTNode>,int> returner;
            switch (t_program[t_counter].first) {
                case Token_indentifier:
                    if (t_program[t_counter+1].first==Token_assignment) {
                        returner=parse_assignment(t_program,t_counter);
                    } else
                    
                    if (t_program[t_counter+1].first==Token_o_paren) {
                        returner=parse_callExpression(t_program,t_counter);
                    }
                break;

                default:
                    throw std::invalid_argument("I really don't think it's possible to get this error message. If you're getting this you're doing somethhing seriously wrong with this: "+t_program[t_counter].second);
                break;
            };
            // t_counter=returner.second;


            // //Checking for Binary Operatiobns
            // std::pair<std::unique_ptr<ASTNode>,int> _parsedn=move(parsers::parse(t_program,t_counter));
            // t_counter=_parsedn.second;
            
            // if (_parsedn.first!=nullptr) {
            //     std::shared_ptr<ASTNode> _temp0=move(_parsedn.first);
            //     if (std::dynamic_pointer_cast<BinaryOpNode>(_temp0)==nullptr) std::invalid_argument("Expected Binary Expression");
            //     BinaryOpNode* _temp1=dynamic_cast<BinaryOpNode*>(_temp0.get());
            //     _temp1->left=std::move(returner.first);
            //     std::pair<std::unique_ptr<BinaryOpNode>,int> returner=std::make_pair(std::make_unique<BinaryOpNode>(std::move(*_temp1)),t_counter);
            // }
            
            // returner.second=t_counter;

            return std::move(returner);
        }

        static std::pair<std::unique_ptr<ASTNode>,int> parse_right(std::vector<std::pair<int,std::string>> t_program,int t_counter,std::unique_ptr<ASTNode> t_node=nullptr) {
            std::pair<std::unique_ptr<ASTNode>,int> returner;
            switch (t_program[t_counter].first) {
                case Token_newline:
                case Token_c_paren:
                case Token_coma:
                    t_counter++;
                    return std::make_pair(nullptr,t_counter);
                break;

                case Token_indentifier:
                    if (t_program[t_counter+1].first==Token_o_paren) {
                        returner=parse_callExpression(t_program,t_counter);
                    }
                break;

                case Token_real:
                    returner=parse_real(t_program,t_counter);
                break;

                case Token_addition:
                case Token_subtraction:
                case Token_multiplication:
                case Token_division:
                    returner=parsers::parse_binOps(t_program,t_counter,std::move(t_node));
                    t_counter=returner.second;
                break;

                default:
                    throw std::invalid_argument("Unexpected Token: "+t_program[t_counter].second);
                break;
            };
            t_counter=returner.second;
            
            if (parsers::check_binOp(t_program[t_counter].first)) {
                std::pair<std::unique_ptr<ASTNode>,int> s_returner=parsers::parse_right(t_program,t_counter,std::move(returner.first));

                return s_returner;

            } else {
                return returner;
            }

            //std::pair<std::unique_ptr<ASTNode>,int> _parsedn=move(parsers::parse_right(t_program,t_counter));
            //t_counter=_parsedn.second;
            
            // if (_parsedn.first!=nullptr) {
            //     std::shared_ptr<ASTNode> _temp0=move(_parsedn.first);
            //     if (std::dynamic_pointer_cast<BinaryOpNode>(_temp0)==nullptr) std::invalid_argument("Expected Binary Expression");
            //     BinaryOpNode* _temp1=dynamic_cast<BinaryOpNode*>(_temp0.get());
            //     _temp1->left=std::move(returner.first);
            //     std::pair<std::unique_ptr<BinaryOpNode>,int> returner=std::make_pair(std::make_unique<BinaryOpNode>(std::move(*_temp1)),t_counter);
            // }
            
            // returner.second=t_counter;

            return std::move(returner);
        }

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
        static std::pair<std::unique_ptr<RealNode>,int> parse_real(std::vector<std::pair<int,std::string>> t_program,int t_counter) {
            RealNode __temp;
            __temp.real=stold(t_program[t_counter].second);
            t_counter++;
            
            return std::make_pair(std::make_unique<RealNode>(std::move(__temp)),t_counter);
        }

        //Parse Assignments
        static std::pair<std::unique_ptr<AssignmentNode>,int> parse_assignment(std::vector<std::pair<int,std::string>> t_program,int t_counter) {
            AssignmentNode __temp;
            VariableNode __iden;
            __iden.identifier=t_program[t_counter].second;
            __temp.identifier=std::make_unique<VariableNode>(__iden);

            t_counter+=2;

            std::pair<std::unique_ptr<ASTNode>,int> __parsed=parsers::parse_right(t_program,t_counter);
            __temp.value=std::move(__parsed.first);
            t_counter=__parsed.second;
            
            return std::make_pair(std::make_unique<AssignmentNode>(std::move(__temp)),t_counter);
        }

        //Parses Function Calls
        static std::pair<std::unique_ptr<CallNode>,int> parse_callExpression(std::vector<std::pair<int,std::string>> t_program,int t_counter) {
            CallNode __temp;
            VariableNode __iden;
            __iden.identifier=t_program[t_counter].second;
            __temp.identifier=std::make_unique<VariableNode>(__iden);
            t_counter++;
            
            if (t_program[t_counter+1].first!=Token_c_paren)
            do {
                t_counter++;
                std::pair<std::unique_ptr<ASTNode>,int> __parsed=parsers::parse_right(t_program,t_counter);
                std::shared_ptr<ASTNode> __arg=std::move(__parsed.first);
                __temp.args.emplace_back(__arg);
                t_counter=__parsed.second;
            } while (t_program[t_counter].first!=Token_c_paren);

            t_counter++;

            return std::make_pair(std::make_unique<CallNode>(std::move(__temp)),t_counter);
        }

        //Parses Binary Operations
        static std::pair<std::unique_ptr<BinaryOpNode>,int> parse_binOps(std::vector<std::pair<int,std::string>> t_program,int t_counter,std::unique_ptr<ASTNode> _left) {
            BinaryOpNode __temp;
            __temp.operate=t_program[t_counter].first;
            __temp.left=std::move(_left);
            t_counter++;
            
            std::pair<std::unique_ptr<ASTNode>,int> _right=parsers::parse_right(t_program,t_counter);
            t_counter=_right.second;

            if (check_binOp(t_program[t_counter].first)>1) {
                std::pair<std::unique_ptr<BinaryOpNode>,int> _p_right=parse_binOps(t_program,t_counter,std::move(_right.first));
                __temp.right=std::move(_p_right.first);
                t_counter=_p_right.second;
            } else {
                __temp.right=std::move(_right.first);
            }

            return std::make_pair(std::make_unique<BinaryOpNode>(std::move(__temp)),t_counter);
        }
    };

    //Parsing loop
    do {
        _parsed=parsers::parse_left(t_program,t_counter);
        t_counter=_parsed.second+1;
        node_tree.emplace_back(std::move(_parsed.first));
    } while (t_counter<t_program.size());

    return node_tree;
}

//Test
// int main() {
//     std::string stringed;
//     std::getline(std::cin >> std::ws, stringed);
//     std::vector tokened=parser(stringed);
//     // RealNode* ast_node=dynamic_cast<RealNode*>(tokened[0].get());
//     // std::cout << (*ast_node).real;

//     return 0;
// }
