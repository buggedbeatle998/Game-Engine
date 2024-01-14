#include "to_token.cpp"
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <memory>
#include <utility>

// #ifdef _MSC_VER
// #define _CRT_SECURE_NO_WARNINGS
// #endif

// #ifdef __cplusplus
// #include <cstdio>
// #else
// #include <stdio.h>
// #endif

std::vector<std::unique_ptr<ASTNode>> parser(std::string line) {
    std::vector<std::pair<int,std::string>> t_program=_to_token(line+";");
    std::vector<std::unique_ptr<ASTNode>> node_tree;
    std::pair<std::unique_ptr<ASTNode>,int> _parsed;
    std::pair<std::unique_ptr<ASTNode>,int> _parsedn;
    int t_counter=0;
    

    struct parsers {
        static auto parse(std::vector<std::pair<int,std::string>> t_program,int t_counter) {
            //std::pair<std::unique_ptr<ASTNode>,int>
            switch (t_program[t_counter].first) {
                case Token_newline:
                {
                    t_counter++;
                    ASTNode __temp;
                    return std::make_pair(std::make_unique<ASTNode>(__temp),-1);
                break;
                }

                case Token_indentifier:
                    if (t_program[t_counter+1].first==Token_o_paren) {
                        return parse_callExpression(t_program,t_counter);
                    }
                break;

                case Token_real:
                    return parse_real(t_program,t_counter);
                break;

                default:
                    throw std::invalid_argument("Wowie, if you're reading this you've probally done something really really wrong and you should probally rethink your life choises instead of creating whatever the fuck this is: "+t_program[t_counter].second);
                break;
            };
            t_counter++;
            ASTNode __temp;
            return std::make_pair(std::make_unique<ASTNode>(__temp),t_counter);
        }

        static std::pair<std::unique_ptr<CallNode>,int> parse_callExpression(std::vector<std::pair<int,std::string>> t_program,int t_counter) {
            CallNode __temp;
            __temp.identifier=t_program[t_counter].second;
            t_counter++;
            
            if (t_program[t_counter+1].first!=Token_c_paren)
            do {
                t_counter++;
                auto __parsed=parsers::parse(t_program,t_counter);
                __temp.args.push_back((__parsed.first));
                t_counter=__parsed.second;
            } while (t_program[t_counter].first!=Token_c_paren);

            t_counter++;

            return std::make_pair(std::make_unique<CallNode>(__temp),t_counter);
        }

        static std::pair<std::unique_ptr<RealNode>,int> parse_real(std::vector<std::pair<int,std::string>> t_program,int t_counter) {
            RealNode __temp;
            __temp.real=stold(t_program[t_counter].second);
            t_counter++;
            
            return std::make_pair(std::make_unique<RealNode>(__temp),t_counter);
        }
    };

    do {
        _parsed=parsers::parse(t_program,t_counter);
        t_counter=_parsed.second;
        _parsedn=parsers::parse(t_program,t_counter);
        t_counter=(_parsedn.second<0 ? t_counter+1 : _parsedn.second);
        node_tree.push_back(std::move((_parsedn.second==-1 ? _parsed : _parsedn).first));
        
    } while (t_counter<t_program.size());

    return node_tree;
}

int main() {
    // CallNode __temp;
    // __temp.args.emplace_back(std::move(__parsed.first));
    std::string stringed;
    std::getline(std::cin >> std::ws, stringed);
    std::vector tokened=parser(stringed);
    RealNode* ast_node=dynamic_cast<RealNode*>(tokened[0].get());
    std::cout << (*ast_node).real;
    return 0;
}
