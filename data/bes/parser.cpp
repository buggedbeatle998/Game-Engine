#include "to_token.cpp"
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <memory>

std::vector<std::unique_ptr<ASTNode>> parser(std::string line) {
    std::vector<std::pair<int,std::string>> t_program=_to_token(line+";");
    std::vector<std::unique_ptr<ASTNode>> node_tree;
    std::pair<std::unique_ptr<ASTNode>,int> _parsed;
    int t_counter=0;
    struct parsers {
        static std::pair<std::unique_ptr<ASTNode>,int> parse(std::vector<std::pair<int,std::string>> t_program,int t_counter) {
            switch (t_program[t_counter].first) {
                case Token_newline:
                break;

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
            ASTNode __temp;
            return std::make_pair(std::make_unique<ASTNode>(__temp),0);
        }

        static std::pair<std::unique_ptr<ASTNode>,int> parse_callExpression(std::vector<std::pair<int,std::string>> t_program,int t_counter) {
            CallNode __temp;
            __temp.identifier=t_program[t_counter].second;
            std::pair<std::unique_ptr<ASTNode>,int> __parsed;
            t_counter++;

            do {
                t_counter++;
                __parsed=parsers::parse(t_program,t_counter);
                __temp.args.push_back(std::move(__parsed.first));
                t_counter=__parsed.second;
            } while (t_program[t_counter].first!=Token_c_paren);

            t_counter++;
            
            return std::make_pair(std::make_unique<ASTNode>(__temp),t_counter);
        }

        static std::pair<std::unique_ptr<ASTNode>,int> parse_real(std::vector<std::pair<int,std::string>> t_program,int t_counter) {
            RealNode __temp;
            __temp.real=stold(t_program[t_counter].second);
            t_counter++;
            
            return std::make_pair(std::make_unique<ASTNode>(__temp),t_counter);
        }
    };

    do {
        _parsed=parsers::parse(t_program,t_counter);

        node_tree.push_back(std::move(_parsed.first));
        t_counter=_parsed.second;
    } while (t_counter<t_program.size());

    return node_tree;
}

// int main() {
//     std::string stringed;
//     std::getline(std::cin >> std::ws, stringed);
//     std::vector tokened=parser(stringed);
//     return 0;
// }
