#include <stdio.h>
#include <cstdlib>

#define BUF_SIZE 32

int main() {
    char buf[BUF_SIZE];

    FILE * fp, * input_fp;
    fp = fopen("data.txt","r");
    input_fp = fopen("input.txt","w");

    if (!fp) {
        printf("file data.txt open failed\n");
        exit(0);
    }

    if (!input_fp) {
        printf("file input.txt open failed\n");
        exit(0);
    }

    int i = 0;
    while (fgets(buf,BUF_SIZE,fp)) {
        printf("num: %d\n",++i);
        fputs(buf,input_fp);
//        fwrite(buf,BUF_SIZE,1,input_fp);
//        fflush(fp);
//        fflush(input_fp);
    }

    fflush(fp);
    fflush(input_fp);
    fclose(fp);
    fclose(input_fp);
}