#include "token_types.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <memory>

//Converts a BS program to a vector of Tokens
vector<pair<int,string>> to_token(string line) {
    //declaration
    vector<pair<int,string>> Token_a;
    int place=0;

    //processing
    do {
        string str_i(1,line.at(place));
        place++;

        //ignore whitespace except newline
        if (str_i==" "||str_i=="\t") {
            continue;
        }

        //newlines
        if (str_i==";"||str_i=="\n")
        {
            if (Token_a[Token_a.size()-1].first!=Token_newline) //compresses multiple newlines down to one
                Token_a.push_back(make_pair(Token_newline,str_i));
            continue;
        }

        //identifiers
        if (regex_match(str_i,regex("([A-Za-z_])"))) {
            string _iden="";
            _iden+=str_i;
            while (place<line.length()) {
                str_i=line.at(place);
                if (!regex_match(str_i,regex("([A-Za-z0-9_])"))) break;
                _iden+=str_i;
                place++;
            }
            if (!regex_search(_iden,regex("([A-Za-z])"))) throw invalid_argument("Invalid Identifier: "+_iden);
            Token_a.push_back(make_pair(Token_indentifier,_iden));
            continue;
        }

        //assignments and comparitive equals
        if (str_i=="=") {
            if (line.at(place)=='=') {
                Token_a.push_back(make_pair(Token_comp_equals,str_i));
                continue;
            }
            Token_a.push_back(make_pair(Token_assignment,str_i));
            continue;
        }

        //reals
        if (regex_match(str_i,regex("([0-9.])"))) {
            string _real="";
            _real+=str_i;
            while (place<line.length()) {
                str_i=line.at(place);
                if (!regex_match(str_i,regex("([0-9._])"))) break;
                if (str_i!="_") _real+=str_i;
                place++;
            }
            if (count(begin(_real),end(_real),'.')>1) throw invalid_argument("Invalid Real: "+_real);
            if (count(begin(_real),end(_real),'.')==0) _real+=".0";
            else if (_real.at(_real.length()-1)=='.') _real+="0";
            Token_a.push_back(make_pair(Token_real,_real));
            continue;
        }
        
        //string
        if (str_i=="\""||str_i=="\'") {
            string _temp=str_i;
            string _string="";
            str_i=line.at(place);
            while (str_i!=_temp) {
                _string+=str_i!="\"" ? str_i : "\\\"";
                place++;
                str_i=line.at(place);
            }
            place++;
            Token_a.push_back(make_pair(Token_string,_string));
            continue;
        }

        //addition
        if (str_i=="+") {
            Token_a.push_back(make_pair(Token_addition,str_i));
            continue;
        }

        //subtraction and type identifier
        if (str_i=="-") {
            if (line.at(place)=='>') {
                Token_a.push_back(make_pair(Token_typeIdentifier,str_i+line.at(place)));
                place++;
                continue;
            }
            Token_a.push_back(make_pair(Token_subtraction,str_i));
            continue;
        }

        //multiplication
        if (str_i=="*") {
            Token_a.push_back(make_pair(Token_multiplication,str_i));
            continue;
        }

        //division
        if (str_i=="/") {
            Token_a.push_back(make_pair(Token_division,str_i));
            continue;
        }

        //Comma
        if (str_i==",") {
            Token_a.push_back(make_pair(Token_coma,str_i));
            continue;
        }

        //open parenclose paren
        if (str_i=="(") {
            Token_a.push_back(make_pair(Token_o_paren,str_i));
            continue;
        }

        //close parenopen square paren
        if (str_i==")") {
            Token_a.push_back(make_pair(Token_c_paren,str_i));
            continue;
        }

        //open square parenclose square paren
        if (str_i=="[") {
            Token_a.push_back(make_pair(Token_s_o_paren,str_i));
            continue;
        }

        //close square parenopen curly paren
        if (str_i=="]") {
            Token_a.push_back(make_pair(Token_s_c_paren,str_i));
            continue;
        }

        //open curly paren
        if (str_i=="{") {
            Token_a.push_back(make_pair(Token_c_o_paren,str_i));
            continue;
        }

        //close curly paren
        if (str_i=="}") {
            Token_a.push_back(make_pair(Token_c_c_paren,str_i));
            continue;
        }

        //hash
        if (str_i=="`") {
            Token_a.push_back(make_pair(Token_hasher,str_i));
            continue;
        }

        //tenary
        if (str_i=="?") {
            Token_a.push_back(make_pair(Token_1_tenary,str_i));
            continue;
        }

        //tenary and walrus
        if (str_i==":") {
            if (line.at(place)=='=') {
                Token_a.push_back(make_pair(Token_walrus,str_i+line.at(place)));
                place++;
                continue;
            }
            Token_a.push_back(make_pair(Token_0_tenary,str_i));
            continue;
        }

        //not
        if (str_i=="!") {
            if (line.at(place)=='=') {
                Token_a.push_back(make_pair(Token_not_equals,str_i+line.at(place)));
                place++;
                continue;
            }
            Token_a.push_back(make_pair(Token_not,str_i));
            continue;
        }

        //greater and/or equals
        if (str_i==">") {
            if (line.at(place)=='=') {
                Token_a.push_back(make_pair(Token_greater_equals,str_i+line.at(place)));
                place++;
                continue;
            }
            Token_a.push_back(make_pair(Token_greater,str_i));
            continue;
        }
        
        //greater and/or equals
        if (str_i=="<") {
            if (line.at(place)=='=') {
                Token_a.push_back(make_pair(Token_less_equals,str_i+line.at(place)));
                place++;
                continue;
            }
            Token_a.push_back(make_pair(Token_less,str_i));
            continue;
        }

        invalid_argument("Unknown Token: "+str_i);
    } while (place<line.length());

    //returns an vector of tokens
    return Token_a;
}

// int main() {
//     string stringed;
//     getline(cin >> ws, stringed);
//     vector tokened=_to_token(stringed);
//     for (int i=0;i<tokened.size();i++) {
//         cout << tokened[i].first << "\n";
//         cout << tokened[i].second << "\n";
//     }
//     system("pause");
//     return 0;
// }