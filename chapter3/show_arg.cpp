// show_arg.cpp--从系统调用exec的角度理解main函数通过命令行传递参数
#include <stdio.h>

int main(int argc, char * argv[]) {// int argc--argument count:参数个数 char * argv[]--argument value:参数列表
    printf("参数个数: %d\n",argc);
    while (--argc)// 第一个参数不输出
        printf("%s\n",*(++argv));// 第一个参数不输出，因此从argv[1]开始--argv[0]为"./show_arg"
}