/*
 * test_format.c
 *
 *  Created on: 2017年2月7日
 *      Author: ubuntu
 */
#include <stdio.h>
#include <string.h>

/*
 * 函数名      : FunctionBigLittleEndianTest
 * 功能        : 大小端测试函数
 * 描述        : 小端模式：内存的低地址存数据的低位，内存的高地址存数据的高位。低低高高。
 * 描述        : 大端模式：内存的低位存数据的高位，内存的高地址存数据的低位。  低高高低
 * 参数        : void
 * 返回值      : void (本编译器测试为小端模式:低字节在内存低地址)
 * 作者        : 吕春豪  2017-2-07
 */
void FunctionEndianTest()
{
	int i;
	char c8Data[4];
	int a = 0x12345678;

	//c8Data[0-3]:0x78 0x56 0x34 0x12
	memcpy(c8Data, (char *)(&a), sizeof(int));
	for(i = 0; i < sizeof(int); i ++)
	{
		printf("%02X", (unsigned char)c8Data[i]);
	}
	printf("\n");

	/*
	 * Big endian   : 0x100   0x101  0x102  0x103
	 *                 12      34      56     78
	 * Little endian:  78      56      34     12
	 */
	char *p = (char *)(&a);
	if(*p == 0x78)
	{
		printf("Little endian \n");
	}
	else if(*p == 0x12)
	{
		printf("Big endian \n");
	}
	else
	{
		printf("other situation \n");
	}

}
