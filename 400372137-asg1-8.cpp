#include <cstdlib>
#include <iostream>


template<typename T1, typename T2>
//template class stores two objects of any type
class Pair 
{
    T1 value1;
    T2 value2;

public:
    //constructor that initializes value1 and value2
    Pair(T1 val1, T2 val2) : value1(val1), value2(val2) {}

    //function to display pair in the format <value1, value2>
    void print() 
    {
        std::cout << "<" << value1 << ", " << value2 << ">" << std::endl;
    }
};

int main() 
{
    //creating and printing pair with an int and a double
    Pair<int, double> p1(1, 7.3);
    p1.print();
    
    //creating and printing pair with a string and a double
    Pair<std::string, double> p2("hello", 7.7);
    p2.print();
    
    //creating and printing pair with a float and a long
    Pair<float, long> p3(1.2, 777777773);
    p3.print();
}

