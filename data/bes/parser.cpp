#include <to_token.cpp>

std::vector<ASTNode> parser(std::string line) {
    std::vector<std::pair<int,std::string>> t_line=_to_token(line);
    std::vector<ASTNode> node_tree;
    int t_counter=0;
    //function(arg0,5+3,arg2){}
    //<Token_indentifier,Token_o_paren,Token_coma,Node,Token_coma,Node,Token_coma,Node,Token_c_paren>
    do {

        std::pair<int,std::string> curr_token=t_line[t_counter];
        t_counter++;
        
        std::unique_ptr<ASTNode> parse() {
            switch (curr_token.first) {
                case Token_indentifier:
                    if (t_line[t_counter].first==Token_o_paren) {
                        CallNode __temp;
                        __temp.identifier=curr_token.second;
                        do {
                            t_counter++;
                            
                            std::unique_ptr<parse()>

                            t_counter++;
                        } while (t_line[t_counter].first!=Token_c_paren);
                        t_counter++;
                        return;
                    }
                break;

                default:
                    throw std::invalid_argument("Wowie, if you're reading this you've probally done something really really wrong and you should probally rethink your life choises instead of creating whatever the fuck this is: "+curr_token.second);
                break;
            }
        }
    } while (t_counter<t_line.size());

    return node_tree;
}

int main() {
    std::string stringed;
    std::getline(std::cin >> std::ws, stringed);
    std::vector tokened=parse(stringed);
}