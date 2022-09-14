#include <iostream>
#include <stdio.h>
#include <string.h>

int main() {
/*    std::cout << "Hello, World!" << std::endl;
    char bt[1024]="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa_1";
    int t=0;
    for(int i=0;i<1024;i++){
        if(bt[i]=='a'){
            t=t+1;
        }
    }
    printf("count is %d",t);*/

    char str[50]={0};

    //如果一开始有赋值，数据，使用sizeof就会有乱码，strlen正常
    //如果赋值为0，使用sizeof最起码能显示出结果，strlen什么都没有
   /* strcpy(str,"This is string.h library function");
    puts(str);*/

    memset(str,97, strlen (str));
    puts(str);
    return 0;
}
