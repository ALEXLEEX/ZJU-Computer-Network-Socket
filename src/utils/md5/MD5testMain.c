#include <stdio.h>
#include <string.h>
#include "global.h"
#include "md5.h"

void md5String(unsigned char str[], unsigned int len, unsigned char md5[16])
{
	// ���������Ľṹ
	MD5_CTX ctx;
	
	// ��ʼ��������
	MD5Init(&ctx);
	
	// ����ɢ��ժҪ
	MD5Update(&ctx, (unsigned char *)str, strlen(str));
	
	// ��ɼ��� 
	MD5Final(md5, &ctx);
}

int main(int argc, char *argv[]) 
{
	// Ҫ������ַ���
	char str[4096];
	printf("Input: ");
	gets(str);
	
	unsigned char md5[16];   // ɢ��ժҪ���
	md5String(str, strlen(str), md5);
	
	printf("sizeof(UINT2)=%d, sizeof(UNIT4)=%d\n", sizeof(UINT2), sizeof(UINT4));
	
	// ��ʮ��������ʽ������
	int i;
	printf("MD5: ");
	for (i=0; i<16; i++) printf("%02X", (unsigned)md5[i]);
	printf("\n");
	
	return 0;
}
