#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

template <typename T>
class MyStack {
    vector<T> _stack;    
public:
    MyStack() { cout << "MyStack ctor" << endl; }
};
