#include <iostream>
#include <string>
#include <vector>

using namespace std;

void sayHello() {
    std::cout << "Hello, World!" << std::endl;
}

int main(int argc, char* argv[])
{
    int i = 0;
    vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the c++ extension!"};
    
    for(string s : msg) {
        cout << s << " ";
        i++;
    }
    cout << endl;

    sayHello();
}
