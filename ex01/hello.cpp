#include<stdio.h>
int main(){
	printf("Hello World!\n");
	char userinput[100];
	printf("请输入你想要打印的内容：");
	scanf("%s",userinput);
	printf("您输入的内容是：%s\n",userinput);
	return 0;
} 
