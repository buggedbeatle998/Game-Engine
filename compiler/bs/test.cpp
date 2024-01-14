#include <memory>
#include <string>
#include <iostream>
#include <vector>

class base {
    public:
        virtual ~base() = default;
        int cool=5;
};

class child1 : public base {
    public:
        std::string name="cheese";
        std::vector<std::unique_ptr<base>> args;
};

class child2 : public base {
    public:
        std::string name="tomato";
        int what=5;
};

// std::unique_ptr<base> _test() {
//     child1 __test;
//     __test.name="2";
//     return std::make_unique<child1>(__test);
// }

int main() {
    struct hhhhh {
        static 
    }
    child2 child2_;
    child1 _temp;
    _temp.name="hhh";
    _temp.args.push_back(std::make_unique<child2>(child2_));
    
    std::cout << "hi: " << (*(dynamic_cast<child2*>(_temp.args[0].get()))).name;

    return 0;
}
