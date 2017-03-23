/****************************************************************************
 * Copyright(c), 2001-2060, ******************************* 版权所有
 ****************************************************************************
 * 文件名称             : jsonfile.c
 * 版本                 : 0.0.1
 * 作者                 : 吕春豪
 * 创建日期             : 2017年2月17日 上午10:11:18
 * 描述                 : 
 ****************************************************************************/
#include <stdio.h>
#include <string.h>

#include "ProjectA.h"
#include "json/json.h"
#include "jsonfile.h"
#include "format.h"




/*
 * MakeUpgradeJsonFile
 * 将升级结构体转换成json存入制定file中
 */
void MakeUpgradeJsonFile(char *pDirName, StUpgradeDescribeInfo stUpgradeInfo)
{
	char c8Tmp[16];
	uint8_t u8Data[4];

	/* 设备 */
	json_object *pJson_Dev = json_object_new_object();
	if (pJson_Dev == NULL)
	{
		PRINT("json_object_new_object error!\n");
		return;
	}
	json_object_object_add(pJson_Dev, "Length", json_object_new_int(stUpgradeInfo.stDevice.s32Length));

	memset(c8Tmp, 0, 16);
	sprintf(c8Tmp, "%d.%d.%d", stUpgradeInfo.stDevice.c8FrwVersion[0], \
			stUpgradeInfo.stDevice.c8FrwVersion[1], \
	stUpgradeInfo.stDevice.c8FrwVersion[2]);
	json_object_object_add(pJson_Dev, "FVer", json_object_new_string(c8Tmp));

	memset(c8Tmp, 0, 16);
	memcpy(u8Data, (char *)&stUpgradeInfo.stDevice.u32CRC32, sizeof(uint32_t));
	sprintf(c8Tmp, "%02X%02X%02X%02X",  u8Data[3], u8Data[2], u8Data[1], u8Data[0]);
	json_object_object_add(pJson_Dev, "CRC32", json_object_new_string(c8Tmp));

	/* 从模块 */
	json_object *pJson_Mod = json_object_new_object();
	if (pJson_Mod == NULL)
	{
		PRINT("json_object_new_object error!\n");
		return;
	}
	json_object_object_add(pJson_Mod, "Length", json_object_new_int(stUpgradeInfo.stModule.s32Length));

	memset(c8Tmp, 0, 16);
	sprintf(c8Tmp, "%d.%d.%d", stUpgradeInfo.stModule.c8FrwVersion[0], \
			stUpgradeInfo.stModule.c8FrwVersion[1], \
	stUpgradeInfo.stModule.c8FrwVersion[2]);
	json_object_object_add(pJson_Mod, "FVer", json_object_new_string(c8Tmp));

	memset(c8Tmp, 0, 16);
	memcpy(u8Data, (char *)&stUpgradeInfo.stModule.u32CRC32, sizeof(uint32_t));
	sprintf(c8Tmp, "%02X%02X%02X%02X",  u8Data[3], u8Data[2], u8Data[1], u8Data[0]);
	json_object_object_add(pJson_Mod, "CRC32", json_object_new_string(c8Tmp));

	/* 总版本号 */
	json_object *pJson = json_object_new_object();
	json_object_object_add(pJson, "TotalLength", json_object_new_int(stUpgradeInfo.s32TotalLength));
	json_object_object_add(pJson, "Model", json_object_new_string(stUpgradeInfo.c8Model));
	json_object_object_add(pJson, "FileName", json_object_new_string(stUpgradeInfo.c8FileName));

	memset(c8Tmp, 0, 16);
	memcpy(u8Data, (char *)&stUpgradeInfo.u32TotalCRC32, sizeof(uint32_t));
	sprintf(c8Tmp, "%02X%02X%02X%02X",  u8Data[3], u8Data[2], u8Data[1], u8Data[0]);
	json_object_object_add(pJson, "TotalCRC32", json_object_new_string(c8Tmp));

	memset(c8Tmp, 0, 16);
	sprintf(c8Tmp, "%d.%d.%d", stUpgradeInfo.c8FrwVersion[0], stUpgradeInfo.c8FrwVersion[1],\
			stUpgradeInfo.c8FrwVersion[2]);
	json_object_object_add(pJson, "FVer", json_object_new_string(c8Tmp));

	memset(c8Tmp, 0, 16);
	sprintf(c8Tmp, "%d.%d.%d", stUpgradeInfo.c8HdwVersion[0], stUpgradeInfo.c8HdwVersion[1],\
			stUpgradeInfo.c8HdwVersion[2]);
	json_object_object_add(pJson, "HVer", json_object_new_string(c8Tmp));

	json_object_object_add(pJson, "Device", pJson_Dev);
	json_object_object_add(pJson, "Module", pJson_Mod);

	json_object_to_file(pDirName, pJson);
	json_object_put(pJson);

}
//获取版本号 lvchunhao 2017-1-17
static int32_t GetVersionFromJson(json_object *pSon_obj,char *pReturn)
{
#if 0
	/* 方法一 */
	int a,b,c;
	char JsonB[30];
	const char *pTmp=NULL;

	if(pSon_obj==NULL){
		return -1;
	}
    sprintf(JsonB,"%s",json_object_to_json_string(pSon_obj));   //“3.5.6”
    a = atoi(JsonB+1);
    pReturn[0]=a;

    pTmp = strchr(JsonB, '.');
    if(pTmp==NULL){
    	return -1;
    }
    b = atoi(pTmp+1);
    pReturn[1]=b;

    pTmp = strchr(pTmp+1, '.');
    if(pTmp==NULL){
    	return -1;
    }
    c = atoi(pTmp+1);
    pReturn[2]=c;
#else
    /* 方法二 */
    int a,b,c;
    char c8Data[32];
    memset(c8Data, 0, 32);
    strcpy(c8Data, json_object_get_string(pSon_obj));      //3.5.6
    sscanf(c8Data, "%d.%d.%d", &a, &b, &c);
    pReturn[0] = a;
    pReturn[1] = b;
    pReturn[2] = c;
#endif
    return 0;
}
//将CRC32字符串转换成十六进制 lvchunhao 2017-1-17
//"1A2B3C4D" == > 0x1A2B3C4D
static uint32_t GetCRC32FromReturn(json_object *pJsonObj)
{
	char i, c8R[16];
	char c8RServer[36];
	char c8Buf[4] = {0};

//	printf("%s\n", json_object_get_string(pJsonObj));      //11223344
//	printf("%s\n", json_object_to_json_string(pJsonObj));  //"11223344"

	snprintf(c8RServer, 16,"%s", json_object_to_json_string(pJsonObj));
	for (i = 0; i < 4; i++)
	{
		c8Buf[0] = c8RServer[i * 2 + 1];		/* \" */
		c8Buf[1] = c8RServer[i * 2 + 2];
		sscanf(c8Buf, "%02hhX", c8R + 3 - i);
	}

	uint32_t u32Retrn;
	memcpy((char *)&u32Retrn, c8R, sizeof(uint32_t));

	return u32Retrn;
}
/*
 * 函数名      : GetUpgradeInfoFromJsonFile
 * 功能        : 从json文件中获取升级描述信息
 * 参数        :pPathName[in]:文件路径及名字
 * 参数        :stUpgradeInfo[out]输出结构体
 * 返值              :0成功 -1失败
 * 作者        : lvchunhao 2017-2-16 星期四
 */
int32_t GetUpgradeInfoFromJsonFile(char *pPathName, StUpgradeDescribeInfo *stUpgradeInfo)
{
	int32_t s32Err;
	json_object *pJson, *pSon_obj;
	json_object *pJson_dev, *pJson_Mod;

	/* 将Json文件转换成object对象 */
	pJson = json_object_from_file(pPathName);
	if(pJson == NULL)
	{
		PRINT("json_object_from_file error \n");
		goto end;
	}

	/* 解析获取的json对象内容 */
	pSon_obj =  json_object_object_get(pJson, "TotalLength");
	if(pSon_obj == NULL)
	{
		PRINT("json_object_object_get error \n");
		goto end;
	}
	stUpgradeInfo->s32TotalLength = json_object_get_int(pSon_obj);
	/* 获取固件总CRC32校验 */
	pSon_obj =  json_object_object_get(pJson, "TotalCRC32");
	if(pSon_obj == NULL)
	{
		PRINT("json_object_object_get error \n");
		goto end;
	}
	stUpgradeInfo->u32TotalCRC32 = GetCRC32FromReturn(pSon_obj);
	/* 获取固件型号 */
	pSon_obj =  json_object_object_get(pJson, "Model");
	if(pSon_obj == NULL)
	{
		PRINT("json_object_object_get error \n");
		goto end;
	}
	strcpy(stUpgradeInfo->c8Model, json_object_get_string(pSon_obj));
	/* 获取固件名字 */
	pSon_obj =  json_object_object_get(pJson, "FileName");
	if(pSon_obj == NULL)
	{
		PRINT("json_object_object_get error \n");
		goto end;
	}
	strcpy(stUpgradeInfo->c8FileName, json_object_get_string(pSon_obj));

	/* 获取固件总软件版本号 */
	pSon_obj =  json_object_object_get(pJson, "FVer");
	if(pSon_obj == NULL)
	{
		PRINT("json_object_object_get error \n");
		goto end;
	}
	s32Err = GetVersionFromJson(pSon_obj, stUpgradeInfo->c8FrwVersion);
	if(s32Err != 0)
	{
		PRINT("GetVersionFromJson error \n");
		goto end;
	}
	/* 获取固件总硬件版本号 */
	pSon_obj =  json_object_object_get(pJson, "HVer");
	if(pSon_obj == NULL)
	{
		PRINT("json_object_object_get error \n");
		goto end;
	}
	s32Err = GetVersionFromJson(pSon_obj, stUpgradeInfo->c8HdwVersion);
	if(s32Err != 0)
	{
		PRINT("GetVersionFromJson error \n");
		goto end;
	}

	/* 获取设备固件信息 */
	pJson_dev = json_object_object_get(pJson, "Device");
	if(pJson_dev == NULL)
	{
		PRINT("json_object_object_get error \n");
		goto end;
	}
	/* 获取设备固件软件版本号(应该与总软件版本号相等) */
	pSon_obj =  json_object_object_get(pJson_dev, "FVer");
	if(pSon_obj == NULL)
	{
		PRINT("json_object_object_get error \n");
		goto end;
	}
	s32Err = GetVersionFromJson(pSon_obj, stUpgradeInfo->stDevice.c8FrwVersion);
	if(s32Err != 0)
	{
		PRINT("GetVersionFromJson error \n");
		goto end;
	}
	/* 设备固件长度 */
	pSon_obj =  json_object_object_get(pJson_dev, "Length");
	if(pSon_obj == NULL)
	{
		PRINT("json_object_object_get error \n");
		goto end;
	}
	stUpgradeInfo->stDevice.s32Length = json_object_get_int(pSon_obj);
	/* 设备固件CRC32*/
	pSon_obj =  json_object_object_get(pJson_dev, "CRC32");
	if(pSon_obj == NULL)
	{
		PRINT("json_object_object_get error \n");
		goto end;
	}
	stUpgradeInfo->stDevice.u32CRC32 = GetCRC32FromReturn(pSon_obj);;


	/* 获取从模块固件信息 */
	pJson_Mod = json_object_object_get(pJson, "Module");
	if(pJson_Mod == NULL)
	{
		PRINT("json_object_object_get error \n");
		goto end;
	}
	/* 获取从模块固件软件版本号(从模块软件版本号与总软件版本号不相等) */
	pSon_obj =  json_object_object_get(pJson_Mod, "FVer");
	if(pSon_obj == NULL)
	{
		PRINT("json_object_object_get error \n");
		goto end;
	}
	s32Err = GetVersionFromJson(pSon_obj, stUpgradeInfo->stModule.c8FrwVersion);
	if(s32Err != 0)
	{
		PRINT("GetVersionFromJson error \n");
		goto end;
	}
	/* 从模块固件长度 */
	pSon_obj =  json_object_object_get(pJson_Mod, "Length");
	if(pSon_obj == NULL)
	{
		PRINT("json_object_object_get error \n");
		goto end;
	}
	stUpgradeInfo->stModule.s32Length = json_object_get_int(pSon_obj);
	/* 设备固件CRC32*/
	pSon_obj =  json_object_object_get(pJson_Mod, "CRC32");
	if(pSon_obj == NULL)
	{
		PRINT("json_object_object_get error \n");
		goto end;
	}
	stUpgradeInfo->stModule.u32CRC32 = GetCRC32FromReturn(pSon_obj);;

	json_object_put(pJson);
	return 0;
end:
	json_object_put(pJson);
	return -1;
}

