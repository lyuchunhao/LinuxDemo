/****************************************************************************
 * Copyright(c), 2001-2060, ******************************* 版权所有
 ****************************************************************************
 * 文件名称             : ProjectA.h
 * 版本                 : 0.0.1
 * 作者                 : 吕春豪
 * 创建日期             : 2017年2月07日
 * 描述                 : 
 ****************************************************************************/
#ifndef PROJECTA_H_
#define PROJECTA_H_

#include <sys/types.h>
#include <dirent.h>
#include <netinet/in.h>

#ifdef __cplusplus
extern "C" {
#endif



/*错误码定义
 * -------------------------------------------------------------------
 * | 31~28  | 27~26 |    25   |  24  |     23 ~ 16    |    15 ~ 0    |
 * |--------|-------|---------|------|----------------|--------------|
 * | 标识域 |  保留 | 0：保留 | 保留 |        保留    | 各模块错误码 |
 * |  0x0E  |       | 1：用户 |      | 暂用于模块分配 |              |
 * -------------------------------------------------------------------
 */
#define FULL_ERROR_CODE(Identification, UserOrReserved, Module, Code) \
        ((((Identification) & 0x0F) << 28) | \
        (((UserOrReserved) & 0x01) << 25) | \
        (((Module) & 0xFF) << 16) | \
        ((Code) & 0xFFFF))

#define ERROR_CODE(Module, Code) 			FULL_ERROR_CODE(0x0E, 0, (Module), (Code))

#define MY_ERR(code)       					ERROR_CODE(0x15, (code))

enum
{
	_Err_Identification = 0x01,
	_Err_Handle,
	_Err_InvalidParam,
	_Err_CmdLen,
	_Err_CmdType,
	_Err_NULLPtr,
	_Err_Time,
	_Err_Mem,
	_Err_NoneCmdData,
	_Err_TimeOut,

	_Err_DDL_NoSpace = 0x50,
	_Err_DDL_NoElement,
	_Err_DDL_Exist,
	_Err_DDL_NotExist,
	_Err_DDL_Index,
	_Err_DDL_IndexNotUsing,

	_Err_SLO_NoSpace = 0x100,
	_Err_SLO_NoElement,
	_Err_SLO_Exist,
	_Err_SLO_NotExist,
	_Err_SLO_Index,
	_Err_SLO_IndexNotUsing,

	_Err_LOG_NoText = 0x110,
	_Err_LOG_Time_FMT,
	_Err_NotADir,
	_Err_JSON,
	_Err_Authentication,
	_Err_IDPS,

	_Err_Cloud_IsNotOnline = 0x120,
	_Err_Cloud_Body,
	_Err_Cloud_JSON,
	_Err_Cloud_CMD,
	_Err_Cloud_Data,
	_Err_Cloud_Authentication,
	_Err_Cloud_Save_Domain,
	_Err_Cloud_Get_Domain,

	_Err_Cloud_Result = 0x130,

	_Err_Unkown_Host = 0x140,

	_Err_File_CRC = 0x150,
	_Err_No_New_Version,

	_Err_Hash_NoSpace = 0x160,
	_Err_Hash_NoElement,
	_Err_Hash_Exist,
	_Err_Hash_NotExist,
	_Err_Hash_Index,
	_Err_Hash_IndexNotUsing,
	_Err_Hash_ToLarge,
	_Err_Hash_NotAProcess,

	_Err_WPA_Open = 0x170,
	_Err_WPA_TimeOut,
	_Err_WPA_Send,

	_Err_HTTP = 0x1000,

	_Err_SSL = 0x2000,

	_Err_SYS = 0xFF00,

	_Err_Common = 0xFFFF,
};

/*
 * 宏变量: ##__VA_ARGS__， __FILE__， __LINE__ ， __FUNCTION__
 *       1. 宏定义的作用是将括号内...的内容原样抄写在__VA_ARGS__的位置
 *       2. ##宏连接符的作用是如果变参的个数为0的话，去掉前面的，号
 */
#ifdef _DEBUG
#define PRINT(x, ...) printf("[%s:%d]: "x, __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define PRINT(x, ...)
#endif

/*
 * __VA_ARGS__只能是一些不含任何变量的字符串常量,若含有变量，则会报错
 * 1. myprintf("I love you!\n");               ok
 * 2. myprintf("s32Err = %d \n!\n", s32Err);   no
 */
#define myprintf(...) printf("[lch]:File:%s, Line:%d, Function:%s," \
     __VA_ARGS__, __FILE__, __LINE__ ,__FUNCTION__);

//test-3: 必须加y,否则一旦添加新变量，便执行报错
//y是与__VA_ARGS__中的变量有对应关系的，只有添加y，才可以在...中添加新的需要打印的变量
#define PrintDebug(y, ...) printf("[Debug]:File:%s, Line:%d, Function:%s,"y, \
      __FILE__, __LINE__ ,__FUNCTION__,##__VA_ARGS__);


void FunA (void);
void FunctionTimeTest();
void FunctionEndianTest();
int32_t ReadDataFromFile(char *u8PathName, uint16_t u16BlockNum, uint8_t u8Data[256]);


#ifdef __cplusplus
}
#endif

#endif /* PROJECTA_H_ */
