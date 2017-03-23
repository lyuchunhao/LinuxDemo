/****************************************************************************
 * Copyright(c), 2001-2060, ******************************* 版权所有
 ****************************************************************************
 * 文件名称             : dirtraversal.c
 * 版本                 : 0.0.1
 * 作者                 : 吕春豪
 * 创建日期             : 2017年2月16日 下午3:18:25
 * 描述                 : 
 ****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include "ProjectA.h"
#include "dirtraversal.h"




static int32_t IsADir(const char *pPathName)
{
	int32_t s32Tmp1 = -1, s32Tmp2 = -1, s32Tmp3 = -1, s32Tmp4 = -1;

	struct stat stStat =
	{ 0 };

	(void) s32Tmp1;
	(void) s32Tmp2;
	(void) s32Tmp3;
	(void) s32Tmp4;

	if (NULL == pPathName)
	{
		return MY_ERR(_Err_NULLPtr);
	}

	if (lstat(pPathName, &stStat) < 0)
	{
		return MY_ERR(_Err_SYS + errno);
	}

	if (0 == S_ISDIR(stStat.st_mode))
	{
		PRINT("the path is: %s", pPathName);
		return MY_ERR(_Err_NotADir);
	}
	return 0;
}
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
		PFUN_TraversalDir_Callback pFunCallback, void *pContext)
{
	DIR *pDirHandle = NULL;
	struct dirent stDirent = { 0 }, *pDirent = NULL;
	int32_t s32Err = 0;

	if (NULL == pPathName)
	{
		return MY_ERR(_Err_NULLPtr);
	}

	s32Err = IsADir(pPathName);
	if (s32Err != 0)
	{
		return s32Err;
	}

	pDirHandle = opendir(pPathName);
	if (NULL == pDirHandle)
	{
		return MY_ERR(_Err_SYS + errno);
	}
	while (NULL != (pDirent = readdir(pDirHandle)))
	{
		stDirent = *pDirent;
		s32Err = 0;
		if ((strcmp(stDirent.d_name, ".")) == 0)
		{
			continue;
		}
		if ((strcmp(stDirent.d_name, "..")) == 0)
		{
			continue;
		}
		if (pFunCallback != NULL)
		{
			if ((s32Err = pFunCallback(pPathName, &stDirent, pContext)) != 0)
			{
				return s32Err;
			}
		}
		if (
#if !HAS_CROSS
				((stDirent.d_type & DT_DIR) != 0) &&
#endif
				boIsRecursion)
		{
			char *pPathInner = (char *) malloc(_POSIX_PATH_MAX + 1);
			if (NULL == pPathInner)
			{
				return MY_ERR(_Err_Mem);
			}
			strcpy(pPathInner, pPathName);
			if (pPathInner[strlen(pPathInner) - 1] != '/')
			{
				PRINT("%s\n", pPathInner);
				strcat(pPathInner, "/");
			}
			strcat(pPathInner, stDirent.d_name);
			TraversalDir(pPathInner, boIsRecursion, pFunCallback, pContext);
			free(pPathInner);
		}
	}
	closedir(pDirHandle);
	return 0;
}
/*
 * 函数名      : GetProcessNameFromPID
 * 功能        : 通过进程号得到进程名字
 * 参数        : pNameSave [out]: (char * 类型) 将得到的名字保存到的位置
 *             : u32Size [in]: (uint32_t类型) 指示pNameSave的长度
 *             : s32ID [in]: (int32_t类型) 进程号
 * 返回值      : (int32_t类型) 0表示成功, 否则错误
 * 作者        : 吕春豪
 */
int32_t GetProcessNameFromPID(char *pNameSave, uint32_t u32Size, int32_t s32ID)
{
	char c8ProcPath[32];
	char c8ProcessName[_POSIX_PATH_MAX];
	int32_t s32Length;
	char *pTmp;
	snprintf(c8ProcPath, 32, "/proc/%d/exe", s32ID);

	s32Length = readlink(c8ProcPath, c8ProcessName, _POSIX_PATH_MAX);
	if (s32Length < 0)
	{
		return MY_ERR(_Err_SYS +errno);
	}
	c8ProcessName[s32Length] = 0;
	pTmp = strrchr(c8ProcessName, '/');
	pTmp += 1;
	s32Length = strlen(pTmp) + 1;
	if (u32Size < ((uint32_t) s32Length))
	{
		return MY_ERR(_Err_InvalidParam);
	}
	strcpy(pNameSave, pTmp);
	return 0;
}
static int32_t TraversalDirCallback(const char *pCurPath, struct dirent *pInfo, void *pContext)
{
	/* not a directory */
#if !HAS_CROSS
	if ((pInfo->d_type & DT_DIR) == 0)
	{
		return 0;
	}
#endif
	/* not a process directory */
	if ((pInfo->d_name[0] > '9') || (pInfo->d_name[0] < '0'))
	{
		return 0;
	}

	{
		char c8Name[_POSIX_PATH_MAX];
		int32_t s32Err;
		int32_t s32Pid = atoi(pInfo->d_name);
		s32Err = GetProcessNameFromPID(c8Name, _POSIX_PATH_MAX, s32Pid);
		if (s32Err != 0)
		{
			/* PRINT("s32Err: 0x%08x\n", s32Err);*/
			return 0;
		}
		/* PRINT("pInfo->d_name: %s, c8Name: %s\n", pInfo->d_name, c8Name);*/
		if (strstr(c8Name, (const char *)pContext) != NULL)
		{
			return s32Pid;
		}
	}

	return 0;
}

/*
 * 函数名      : AlreadyRunningUsingName
 * 功能        : 使用进程名字判断进程是否已经运行
 * 参数        : pLockFile[in] (const char *): 进程名
 * 返回值      : int32_t, 返回非负数表明PID，以说明该进程整改运行; 返回0表明改进程名没有运行; 否则表示错误
 * 作者        : 吕春豪
 */
int32_t AlreadyRunningUsingName(const char *pName)
{

	if (pName == NULL)
	{
		return false;
	}

	return TraversalDir("/proc/", false, TraversalDirCallback, (void *)pName);
}


