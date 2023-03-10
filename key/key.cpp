#include <iostream>
#include "conio.h"
 
int main() {
    char ch;
 
    //program pauses here until key is pressed
    ch = getch();
    std::cout << "Key Pressed: " << ch << std::endl;
 
    if(ch == 'a' || ch == 'A')
        std::cout << "Key Pressed: a" << std::endl;
    //if(ch == 's')
        //std::cout << "Key Pressed: s" << std::endl;
    //else if(ch == 'A')
        //std::cout << "Key Pressed: A" << std::endl;
 
    return 0;
}
