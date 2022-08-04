#include <iostream>

class MyClass
{
private:
    int counter;
public:
    MyClass()
    {
        counter = 0;
    }
    void Foo()
    { 
        std::cout << "Foo" << std::endl;    
    }

    void Foo() const
    {
        std::cout << "Foo const" << std::endl;
    }

};

int main()
{
    MyClass cc;
    const MyClass ccc;
    cc.Foo();
    ccc.Foo();
}