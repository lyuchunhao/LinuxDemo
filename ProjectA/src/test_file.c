/****************************************************************************
 * Copyright(c), 2001-2060, ******************************* 版权所有
 ****************************************************************************
 * 文件名称             : test_file.c
 * 版本                 : 0.0.1
 * 作者                 : 吕春豪
 * 创建日期             : 2017年2月22日 上午10:09:01
 * 描述                 : 
 ****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>






int32_t ReadDataFromFile(char *u8PathName, uint16_t u16BlockNum, uint8_t u8Data[256])
{
	int32_t s32Err;
	FILE *Fdfile = fopen(u8PathName, "rb");
	if(Fdfile == NULL)
	{
		printf("fopen error: %s \n", strerror(errno));
		return -1;
	}

	/* 重设文件流的读写位置为文件开头 */
	rewind(Fdfile);
	s32Err = fseek(Fdfile, 128*u16BlockNum, SEEK_SET);
	if(s32Err != 0)
	{
		printf("fseek error: %s \n", strerror(errno));
		return -1;
	}
	printf("s32Err = %d\n", s32Err);
	s32Err = ftell(Fdfile);
	printf("s32Err = %d", s32Err);

	/* 实际读取的元素数.如果返回值与count(不是count*size)不相同,则可能文件结尾或发生错误 */
	/* 如果读到的数据小于128，则有可能到了文件尾或错误发生 */
	s32Err = fread(u8Data, 1, 128,  Fdfile);
	if(s32Err == -1)
	{
		printf("fread error: %s \n", strerror(errno));
		return -1;
	}

//	fseek(Fdfile, 0, SEEK_END);
//	s32Err = ftell(Fdfile);
//	printf("s32Err = %d", s32Err);

	return s32Err;

}
