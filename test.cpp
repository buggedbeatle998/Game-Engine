#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <any>


//Testing file to test random things

class base {
    public:
        virtual ~base() = default;

        virtual std::string getName() {return name;};
        int cool=5;
        std::string name;
};

class child1 : public base {
    public:
        std::vector<std::unique_ptr<base>> args;
        std::string name="cheese";
        virtual std::string getName() {return child1::name;};
};

class child2 : public base {    
    public:
        int what=5;
        std::string name="tomato";
        virtual std::string getName() {return name;};
};

// std::unique_ptr<base> _test() {
//     child1 __test;
//     __test.name="2";
//     return std::make_unique<child1>(__test);
// }

int main() {
    // struct hhhhh {
    //     static 
    // }
    // auto x = 1;
    // child2 child2_;
    // child2_.name="hhh";
    // child1 _temp;
    // std::unique_ptr<base> _cheesed = std::make_unique<child2>(child2_);
    
    // std::cout << "hi: " << (_cheesed.get()->getName());

    
    // std::vector<std::any> cheese={1,"hi",true};

    // for (int i=0;i++;i<3) {
    //     std::cout << "hi";//cheese[i];
    // }

    int __test;
    std::cout << __test;

    return 0;
}
