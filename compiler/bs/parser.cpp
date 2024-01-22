#include "to_token.cpp"
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <memory>
#include <utility>

//This is the file that I'm having problems with. The Function Call Node. Initialized in token_types.

//Parses into an ATS
std::vector<std::unique_ptr<ASTNode>> parser(std::string line) {
    std::vector<std::pair<int,std::string>> t_program=_to_token(line+";");
    std::vector<std::unique_ptr<ASTNode>> node_tree;
    std::pair<std::unique_ptr<ASTNode>,int> _parsed;
    std::pair<std::unique_ptr<ASTNode>,int> _parsedn;
    int t_counter=0;
    
    //Parser functions
    struct parsers {

        //Function that dectects which node to parse
        static std::pair<std::unique_ptr<ASTNode>,int> parse(std::vector<std::pair<int,std::string>> t_program,int t_counter) {
            switch (t_program[t_counter].first) {
                case Token_newline:
                {
                    t_counter++;
                    ASTNode __temp;
                    return std::make_pair(std::make_unique<ASTNode>(__temp),-1);
                break;
                }

                case Token_indentifier:
                    if (t_program[t_counter+1].first==Token_assignment) {
                        return parse_assignment(t_program,t_counter);
                    }

                    if (t_program[t_counter+1].first==Token_o_paren) {
                        return parse_callExpression(t_program,t_counter);
                    }
                break;

                case Token_real:
                    return parse_real(t_program,t_counter);
                break;

                default:
                    throw std::invalid_argument("I really don't think it's possible to get this error message. If you're getting this you're doing somethhing seriously wrong with this: "+t_program[t_counter].second);
                break;
            };

            //This code never happens
            t_counter++;
            ASTNode __temp;
            return std::make_pair(std::make_unique<ASTNode>(__temp),t_counter);
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
            t_counter++;
            
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
                std::pair<std::unique_ptr<ASTNode>,int> __parsed=parsers::parse(t_program,t_counter);
                std::shared_ptr<ASTNode> __arg=std::move(__parsed.first);
                __temp.args.emplace_back(__arg);
                t_counter=__parsed.second;
            } while (t_program[t_counter].first!=Token_c_paren);

            t_counter++;

            return std::make_pair(std::make_unique<CallNode>(std::move(__temp)),t_counter);
        }
    };

    //Parsing loop
    do {
        _parsed=parsers::parse(t_program,t_counter);
        t_counter=_parsed.second;
        _parsedn=parsers::parse(t_program,t_counter);
        t_counter=(_parsedn.second<0 ? t_counter+1 : _parsedn.second);
        node_tree.emplace_back(std::move((_parsedn.second==-1 ? _parsed : _parsedn).first));
        
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
