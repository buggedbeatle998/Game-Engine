#include "token_types.cpp"
#include <string>
#include <iostream>
#include <vector>
#include <regex>

std::vector<std::pair<int,std::string>> _to_token(std::string line) {
    //declaration
    std::vector<std::pair<int,std::string>> Token_a;
    int place=0;

    //processing
    do {
        std::string str_i(1,line.at(place));
        place++;

        //ignore whitespace except newline
        if (str_i==" "||str_i=="\t") {
            continue;
        }

        //identifiers
        if (std::regex_match(str_i,std::regex("([A-Za-z_])"))) {
            std::string _iden="";
            _iden+=str_i;
            while (place<line.length()) {
                str_i=line.at(place);
                if (!std::regex_match(str_i,std::regex("([A-Za-z0-9_])"))) break;
                _iden+=str_i;
                place++;
            }
            if (!std::regex_search(_iden,std::regex("([A-Za-z])"))) throw std::invalid_argument("Invalid Identifier: "+_iden);
            Token_a.push_back(std::make_pair(Token_indentifier,_iden));
            continue;
        }

        if (str_i=="=") {
            Token_a.push_back(std::make_pair(Token_assignment,str_i));
            continue;
        }

        //reals
        if (std::regex_match(str_i,std::regex("([0-9.])"))) {
            std::string _real="";
            _real+=str_i;
            while (place<line.length()) {
                str_i=line.at(place);
                if (!std::regex_match(str_i,std::regex("([0-9._])"))) break;
                if (str_i!="_") _real+=str_i;
                place++;
            }
            if (std::count(std::begin(_real),std::end(_real),'.')>1) throw std::invalid_argument("Invalid Real: "+_real);
            if (_real.at(_real.length()-1)) _real=_real.erase(_real.length()-1,_real.length());
            Token_a.push_back(std::make_pair(Token_real,_real));
            continue;
        }

        //string
        if (str_i=="\""||str_i=="\'"||str_i=="`") {
            std::string _temp=str_i;
            std::string _string="";
            str_i=line.at(place);
            while (str_i!=_temp) {
                _string+=str_i;
                place++;
                str_i=line.at(place);
            }
            place++;
            Token_a.push_back(std::make_pair(Token_string,_string));
            continue;
        }

        //addition
        if (str_i=="+") {
            Token_a.push_back(std::make_pair(Token_addition,str_i));
            continue;
        }

        //subtraction
        if (str_i=="-") {
            Token_a.push_back(std::make_pair(Token_subtraction,str_i));
            continue;
        }

        //multiplication
        if (str_i=="*") {
            Token_a.push_back(std::make_pair(Token_multiplication,str_i));
            continue;
        }

        //division
        if (str_i=="/") {
            Token_a.push_back(std::make_pair(Token_division,str_i));
            continue;
        }

        //open parenclose paren
        if (str_i=="(") {
            Token_a.push_back(std::make_pair(Token_o_paren,str_i));
            continue;
        }

        //close parenopen square paren
        if (str_i==")") {
            Token_a.push_back(std::make_pair(Token_c_paren,str_i));
            continue;
        }

        //open square parenclose square paren
        if (str_i=="[") {
            Token_a.push_back(std::make_pair(Token_s_o_paren,str_i));
            continue;
        }

        //close square parenopen curly paren
        if (str_i=="]") {
            Token_a.push_back(std::make_pair(Token_s_c_paren,str_i));
            continue;
        }

        //open curly paren
        if (str_i=="{") {
            Token_a.push_back(std::make_pair(Token_c_o_paren,str_i));
            continue;
        }

        //close curly paren
        if (str_i=="}") {
            Token_a.push_back(std::make_pair(Token_c_c_paren,str_i));
            continue;
        }

        std::invalid_argument("Unknown Token: "+str_i);
    } while (place<line.length());

    //returns an vector of tokens
    return Token_a;
}

// int main() {
//     std::string stringed;
//     std::getline(std::cin >> std::ws, stringed);
//     std::vector tokened=_to_token("testing=\"Hello World`\'\"+str(3_6.64)");
//     for (int i=0;i<tokened.size();i++) {
//         std::cout << tokened[i].first << "\n";
//         std::cout << tokened[i].second << "\n";
//     }
//     std::system("pause");
//     return 0;
// }