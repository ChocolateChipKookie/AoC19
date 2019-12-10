#include <iostream>
#include <vector>

void f1(){
	std::vector<unsigned> inputs {123265, 68442, 94896, 94670, 145483, 93807, 88703, 139755, 53652, 52754, 128052, 8153$    unsigned total = 0;
    for (auto elem : inputs){
        unsigned val = elem / 3;
        val -= 2;
        total += val;
    }
    std::cout << total<< std::endl;
}

void f2(){
	std::vector<int> inputs {123265, 68442, 94896, 94670, 145483, 93807, 8870$    unsigned total = 0;
    for (auto elem : inputs){
        while (true){
            elem /= 3;
            elem -= 2;
            if(elem <= 0) break;
            total += elem;
        }
    }
    std::cout << total<< std::endl;
}


int main(void){
    f1();
	f2();
}