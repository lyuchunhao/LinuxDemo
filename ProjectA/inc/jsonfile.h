/****************************************************************************
 * Copyright(c), 2001-2060, ******************************* 版权所有
 ****************************************************************************
 * 文件名称             : jsonfile.h
 * 版本                 : 0.0.1
 * 作者                 : 吕春豪
 * 创建日期             : 2017年2月17日 上午10:12:02
 * 描述                 : 
 ****************************************************************************/

#ifndef PROJECTA_INC_JSONFILE_H_
#define PROJECTA_INC_JSONFILE_H_

typedef struct _tagStDevUpgrade
{
	uint32_t s32Length;                  //固件长度
	uint32_t u32CRC32;                   //固件CRC32校验

	char c8FrwVersion[3];                //固件软件版本号
}StDevUpgrade;
typedef struct _tagStUpgradeDescribeInfo
{
	uint32_t s32TotalLength;              //固件总长度
	uint32_t u32TotalCRC32;               //固件总CRC32校验
	char c8Model[16];                     //固件型号 KN-550BHZ
	char c8FrwVersion[3];                 //固件总软件版本号
	char c8HdwVersion[3];                 //固件总硬件版本号
	char c8FileName[256];                 //固件路径及名字

	StDevUpgrade stDevice;                //设备固件信息
	StDevUpgrade stModule;                //从模块固件信息

}StUpgradeDescribeInfo;


/*
 * MakeUpgradeJsonFile
 * 将升级结构体转换成json存入制定file中
 */
void MakeUpgradeJsonFile(char *pDirName, StUpgradeDescribeInfo stUpgradeInfo);
/*
 * 函数名      : GetUpgradeInfoFromJsonFile
 * 功能        : 从json文件中获取升级描述信息
 * 参数        :pPathName[in]:文件路径及名字
 * 参数        :stUpgradeInfo[out]输出结构体
 * 返值              :0成功 -1失败
 * 作者        : lvchunhao 2017-2-16 星期四
 */
int32_t GetUpgradeInfoFromJsonFile(char *pPathName, StUpgradeDescribeInfo *stUpgradeInfo);

#endif /* PROJECTA_INC_JSONFILE_H_ */
