#include <stdio.h>
#include <string.h>
#include "global.h"
#include "md5.h"

void md5String(unsigned char str[], unsigned int len, unsigned char md5[16])
{
	// 定义上下文结构
	MD5_CTX ctx;
	
	// 初始化上下文
	MD5Init(&ctx);
	
	// 计算散列摘要
	MD5Update(&ctx, (unsigned char *)str, strlen(str));
	
	// 完成计算 
	MD5Final(md5, &ctx);
}

int main(int argc, char *argv[]) 
{
	// 要计算的字符串
	char str[4096];
	printf("Input: ");
	gets(str);
	
	unsigned char md5[16];   // 散列摘要结果
	md5String(str, strlen(str), md5);
	
	printf("sizeof(UINT2)=%d, sizeof(UNIT4)=%d\n", sizeof(UINT2), sizeof(UINT4));
	
	// 以十六进制形式输出结果
	int i;
	printf("MD5: ");
	for (i=0; i<16; i++) printf("%02X", (unsigned)md5[i]);
	printf("\n");
	
	return 0;
}
