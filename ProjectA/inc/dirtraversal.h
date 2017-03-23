/****************************************************************************
 * Copyright(c), 2001-2060, ******************************* 版权所有
 ****************************************************************************
 * 文件名称             : dirtraversal.h
 * 版本                 : 0.0.1
 * 作者                 : 吕春豪
 * 创建日期             : 2017年2月16日 下午3:19:18
 * 描述                 : 
 ****************************************************************************/

#ifndef PROJECTA_INC_DIRTRAVERSAL_H_
#define PROJECTA_INC_DIRTRAVERSAL_H_

#include <stdbool.h>

typedef int32_t (*PFUN_TraversalDir_Callback)(const char *pCurPath, struct dirent *pInfo, void *pContext);
/*
 * 函数名      : AlreadyRunningUsingName
 * 功能        : 使用进程名字判断进程是否已经运行
 * 参数        : pLockFile[in] (const char *): 进程名
 * 返回值      : int32_t, 返回非负数表明PID，以说明该进程整改运行; 返回0表明改进程名没有运行; 否则表示错误
 * 作者        : 吕春豪
 */
int32_t AlreadyRunningUsingName(const char *pName);
/*
 * 函数名      : TraversalDir
 * 功能        : 遍历目录
 * 参数        : pPathName [in] (const char * 类型): 要查询的目录
 *             : boIsRecursion [in] (bool类型): 是否递归子目录
 *             : pFunCallback [in] (PFUN_TraversalDir_Callback类型): 详见类型定义
 *             : pContext [in] (void *类型): 回调函数的上下文指针
 * 返回        : 正确返回0, 错误返回错误码
 * 作者        : 吕春豪
 */
int32_t TraversalDir(const char *pPathName, bool boIsRecursion,
		PFUN_TraversalDir_Callback pFunCallback, void *pContext);

#endif /* PROJECTA_INC_DIRTRAVERSAL_H_ */
