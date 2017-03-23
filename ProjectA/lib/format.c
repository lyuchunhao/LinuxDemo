/*
 * format.c
 *
 *  Created on: 2017年1月19日
 *      Author: ubuntu
 */
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>



/*
 * (Hex)
 * 十六进制数的基数是16，采用的数码是
 * 0、1、2、3、4、5、6、7、8、9、A、B、C、D、E、F
 * (String)
 * 一个字符串 string 就是由一系列的字符组成，其中每个字符等同于一个字节
 */


/*
 * 函数名      : HexNum2String
 * 功能        : 将bin转换成字符串格式(dst需要2倍的src空间)
 * 参数        : u8Src [in]: 十六进制 s32Len[in]:需要转换的长度
 * 参数        : u8Dst [in]: 字符串
 * 返回值      : void (Example: {0x1A,02B}2bytes-->"1A2B"4bytes)
 * 作者        : 吕春豪  2017-1-20
 */
void HexNum2String(char *u8Src, char *u8Dst, int32_t s32Len)
{
	int32_t i;

	if(!u8Src || !u8Dst)
	{
		printf("parameters error \n");
		return;
	}

//	sprintf(u8Dst, "%02X%02X%02X%02X", u8Src[0],
//			u8Src[1], u8Src[2], u8Src[3]);

	for(i = 0; i < s32Len; i ++)
	{
		sprintf(u8Dst + i*2, "%02X", (uint8_t)u8Src[i]);
	}

}

/*
 * 函数名      : Hex2String
 * 功能        : 将bin转换成字符串格式(dst需要3倍的src空间)
 * 参数        : src [in]: 十六进制 s32Len[in]:需要转换的长度
 * 参数        : dst [in]: 字符串
 * 返回值      : void (Example: {0x1A,02B}2bytes-->"1A 2B "6bytes)
 * 作者        : 吕春豪  2017-1-19
 */
void Hex2String(char *u8Src, char *u8Dst, int32_t s32Len)
{
	int32_t i;
	uint8_t a[2];

	if(!u8Src || !u8Dst)
	{
		printf("parameters error \n");
		return;
	}

	for(i = 0; i < s32Len; i ++)
	{
		a[0] = u8Src[i] >> 4;    // high
		a[1] = u8Src[i] & 0x0F;  // low
		switch(a[0])
		{
			case 0: case 1: case 2: case 3: case 4:
			case 5: case 6: case 7: case 8: case 9:
				a[0] += '0';
				break;

			case 0x0A: case 0x0B: case 0x0C: case 0x0D: case 0x0E: case 0x0F:
				a[0] = a[0] - 0x0A + 'A';
				break;
		}

		switch(a[1])
		{
			case 0: case 1: case 2: case 3: case 4:
			case 5: case 6: case 7: case 8: case 9:
				a[1] += '0';
				break;

			case 0x0A: case 0x0B: case 0x0C: case 0x0D: case 0x0E: case 0x0F:
				a[1] = a[1] - 0x0A + 'A';
				break;
		}

		u8Dst[i * 3] = a[0];           //高4位
		u8Dst[i * 3 + 1] = a[1];       //低4位
		u8Dst[i * 3 + 2] = ' ';        //空
	}

	u8Dst[i * 3] = 0;
}
/*
 * 函数名      : String2HexNum
 * 功能        : 将字符串转换成16进制bin数据
 * 参数        : c8In [in]: 字符串  s32Len[in]:需要转换的长度
 * 参数        : c8Out [out]: 十六进制
 * 返回值      : 转换后的数据长度 >= 0
 * 描述        : (Example: "1A2B3C"-->{0x1A,0x2B,0x3C} return 3)
 *              (Example: "1A2N3C"-->{0x1A,0x2} return 1(这种情况需要避免的))
 * 作者        : 吕春豪  2017-1-19
 */
int32_t String2HexNum(char *c8In, char *c8Out, int32_t s32Len)
{
	int32_t i;
	int32_t s32Err;

	if(!c8In || !c8Out)
	{
		printf("parameters error \n");
		return -1;
	}

	for (i = 0; i < s32Len/2; i++)
	{
		s32Err = sscanf(c8In + i*2, "%02hhX", c8Out + i);
		if(s32Err == -1)
		{
			printf("sscanf error: %s \n", strerror(errno));
			break;
		}
	}

	return i;
}
/*
 * 函数名      : PrintHexData
 * 功能        : 将十六进制数据以字符串形式打印出来
 * 参数        : pData [in]: 数据
 * 参数        : s32Len[in]:需要打印的长度
 * 返回值      : void
 * 作者        : 吕春豪  2017-1-19
 */
void PrintHexData(void *pData, int32_t s32Len)
{
	int32_t i;
	uint8_t *u8Data = (uint8_t *)pData;

	for(i = 0; i < s32Len; i ++)
	{
		printf("%02X ", u8Data[i]);
	}
	printf("\n");
}
/*
 * 函数名      : HexDataCompare
 * 功能        : 比较两个相同的数据结构
 * 参数        : pDataA [in]: 数据A
 * 参数        : pDataB [in]: 数据B
 * 参数        : s32Len [in]: 需要比较的数据长度
 * 返回值      : 0相等 -1不相等
 * 作者        : 吕春豪  2017-1-22
 */
int32_t HexDataCompare(void *pDataA, void *pDataB, int32_t s32Len)
{
	int32_t i;
	uint8_t *u8DataA = (uint8_t *)pDataA;
	uint8_t *u8DataB = (uint8_t *)pDataB;

	if(!pDataA || !pDataB)
	{
		printf("parameters error \n");
		return -1;
	}

	for(i = 0; i < s32Len; i ++)
	{
		if(u8DataA[i] != u8DataB[i])
		{
			return -1;
		}
	}

	return 0;
}
