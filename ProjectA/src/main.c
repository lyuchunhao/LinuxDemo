/****************************************************************************
 * Copyright(c), 2001-2060, ******************************* 版权所有
 ****************************************************************************
 * 文件名称             : main.c
 * 版本                 : 0.0.1
 * 作者                 : 吕春豪
 * 创建日期             : 2016年5月30日
 * 描述                 : 
 ****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#include "ProjectA.h"
#include "format.h"
#include "dirtraversal.h"
#include "jsonfile.h"


typedef struct _tagStClass
{
	uint8_t flag;
	uint8_t u8Data[16];

	int32_t s32Len;
}StClass;

/* 分析: 该结构体有24bytes内存 */
StClass stClass = {
		1,
		"123456789",
		480
};

static int32_t TraversalDirCallback(const char *pCurPath, struct dirent *pInfo, void *pContext)
{
	char pPathName[256];
	StUpgradeDescribeInfo stUpgradeTmp;

	/* 查找出符合字符串内容的文件 */
	if(strstr(pInfo->d_name, (const char *)pContext) != NULL)
	{
		printf("%s \n", pInfo->d_name);
		//<todo>解析JSON文件，存入map容器中

		memset(pPathName, 0, 256);
		strcat(pPathName, pCurPath);
		strcat(pPathName, pInfo->d_name);
		printf("pPathName: %s\n", pPathName);

		memset(&stUpgradeTmp, 0, sizeof(StUpgradeDescribeInfo));
		GetUpgradeInfoFromJsonFile(pPathName, &stUpgradeTmp);

		printf("s32TotalLength: %d \n", stUpgradeTmp.s32TotalLength);
		printf("u32TotalCRC32: %08X \n", stUpgradeTmp.u32TotalCRC32);
		printf("FVer: %d%d%d\n", stUpgradeTmp.c8FrwVersion[0], stUpgradeTmp.c8FrwVersion[1],\
				stUpgradeTmp.c8FrwVersion[2]);
		printf("HVer: %d%d%d\n", stUpgradeTmp.c8HdwVersion[0], stUpgradeTmp.c8HdwVersion[1],\
				stUpgradeTmp.c8HdwVersion[2]);
		printf("Model: %s \n", stUpgradeTmp.c8Model);
		printf("FileName: %s \n", stUpgradeTmp.c8FileName);

		printf("Device-Length: %d \n", stUpgradeTmp.stDevice.s32Length);
		printf("Device-u32CRC32: %08X \n", stUpgradeTmp.stDevice.u32CRC32);
		printf("Device-FVer: %d%d%d\n", stUpgradeTmp.stDevice.c8FrwVersion[0], stUpgradeTmp.stDevice.c8FrwVersion[1],\
				stUpgradeTmp.stDevice.c8FrwVersion[2]);

		printf("Module-Length: %d \n", stUpgradeTmp.stModule.s32Length);
		printf("Module-u32CRC32: %08X \n", stUpgradeTmp.stModule.u32CRC32);
		printf("Module-FVer: %d%d%d\n", stUpgradeTmp.stModule.c8FrwVersion[0], stUpgradeTmp.stModule.c8FrwVersion[1],\
				stUpgradeTmp.stModule.c8FrwVersion[2]);
	}

	return 0;
}

static StUpgradeDescribeInfo stUpgradeInfo = {
		4096,
		0x1122CCDD,
		"KN-550BHZ",
		{1, 2, 3},
		{4, 5, 6},
		"upgrade.bin",

		{1024, 0x1A2B3C4D},
		{3096, 0x4A5B6C7D},
};

int main(int argc, char *argv[])
{
	printf("This is ProjectA test, now do it \n");
	FunA();
#if 0
	printf("%d %d\n", sizeof(long int), sizeof(time_t));
	FunctionTimeTest();
	FunctionEndianTest();

	/* 生成json文件 */
	printf("\njsonfile test 1\n");
	stUpgradeInfo.stDevice.c8FrwVersion[0] = 1;
	stUpgradeInfo.stDevice.c8FrwVersion[1] = 2;
	stUpgradeInfo.stDevice.c8FrwVersion[2] = 3;
	stUpgradeInfo.stModule.c8FrwVersion[0] = 5;
	stUpgradeInfo.stModule.c8FrwVersion[1] = 6;
	stUpgradeInfo.stModule.c8FrwVersion[2] = 7;
	printf("jsonfile test 2\n");
	MakeUpgradeJsonFile("/home/ubuntu/workspace/tmp/KN-550.json", stUpgradeInfo);

	/* 目录遍历相关测试 */
	int32_t s32Return;
	s32Return = AlreadyRunningUsingName("shadow");
	printf("s32Return = %d \n", s32Return);

	printf("\n\n");
	TraversalDir("/home/ubuntu/workspace/tmp/", false, TraversalDirCallback, ".json");

	StUpgradeDescribeInfo stUpgradeTmp;
	memset(&stUpgradeTmp, 0, sizeof(StUpgradeDescribeInfo));
	//GetUpgradeInfoFromJsonFile("/home/ubuntu/workspace/tmp/KN-550.json", &stUpgradeTmp);

	char a,b,c;
	char c8Str[16] = "3.5.6";
	sscanf(c8Str, "%d.%d.%d", &a, &b, &c);
	printf("a = %d, b = %d, c = %d \n", a, b, c);
#endif

	int32_t s32Err, i;
	uint8_t u8Data[256];
	s32Err = ReadDataFromFile("/home/ubuntu/workspace/tmp/KN-550.bin", 10, u8Data);
	if(s32Err == -1)
	{
		printf("ReadDataFromFile error \n");
	}
	else
	{
		printf("Length: %d \n", s32Err);
		for(i = 0; i < s32Err; i++)
		{
			printf("%02X ", u8Data[i]);
		}
		printf("\n");
	}

	double pi = -3.1415;
	printf("pi = %f (%d) \n", pi, sizeof(long double));

	printf("ProjectA is finish, so all is shit \n");
	return 0;
}









