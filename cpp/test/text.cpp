#include <iostream>

union data {
    char x;
    short y;
    int z;
} a, b;

int m, n, u;

int main() {
    a.x = 65;
    std::cout << a.x << std::endl;

    b.x = 65;
    std::cout << b.x << std::endl;

    return 0;
}

// 定义两个 int 变量，一个命名为 a , 一个命名为 b
int a, b;

// 把 int 换为 
union data {
     char x;
     short y;
     int z;
 } a, b;

a = 20;

b = 40;

int main() {

    union { char x; int y; } a, b, sum;

    a.y = 20;
    b.y = 30;
    sum.y = a.y + b.y;

}