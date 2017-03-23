/*
 * format.h
 *
 *  Created on: 2017年1月19日
 *      Author: ubuntu
 */

#ifndef PROJECTA_INC_FORMAT_H_
#define PROJECTA_INC_FORMAT_H_

#include <stdint.h>

typedef union _tagUn4Bytes
{
	uint32_t u32Data;
	char c8Data[4];
}Un4Bytes;


void PrintHexData(void *pData, int32_t s32Len);
void HexNum2String(char *u8Src, char *u8Dst, int32_t s32Len);
void Hex2String(char *u8Src, char *u8Dst, int32_t s32Len);
int32_t String2HexNum(char *c8In, char *c8Out, int32_t s32Len);
int32_t HexDataCompare(void *pDataA, void *pDataB, int32_t s32Len);


#endif /* PROJECTA_INC_FORMAT_H_ */
