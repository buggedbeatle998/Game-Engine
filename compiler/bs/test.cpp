#include <memory>
#include <string>
#include <iostream>
#include <vector>


//Testing file to test random things

class base {
    std::string name="food";
    public:
        virtual ~base() = default;

        virtual std::string getName() {return name;};
        int cool=5;
};

class child1 : public base {
    std::string name="cheese";
    public:
        std::vector<std::unique_ptr<base>> args;
};

class child2 : public base {
    protected:
        std::string name="tomato";

    public:
        int what=5;
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
    auto x = 1;
    child2 child2_;
    child1 _temp;
    std::unique_ptr<base> _cheesed = std::make_unique<child2>(child2_);
    
    std::cout << "hi: " << (_cheesed.get()->getName());

    return 0;
}
