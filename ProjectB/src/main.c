/****************************************************************************
 * Copyright(c), 2001-2060, ******************************* 版权所有
 ****************************************************************************
 * 文件名称             : main.c
 * 版本                 : 0.0.1
 * 作者                 : 许龙杰
 * 创建日期             : 2014年7月11日
 * 描述                 : 
 ****************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/msg.h>
#include <sched.h>

#include "ProjectB.h"

#define MAX_NUM_OF_THREADS				32

#define CLEANUP()	{\
                        goto CLEANUP;\
                    }

bool g_boIsExit = false;                //线程退出标志

#define FUNCTION_A    1
#define FUNCTION_B    1

//函数声明
void *FunctionA(void *pArgs);
void *FunctionB(void *pArgs);

//注册线程函数
StThrdObj g_stThrdObjs[] =
{
#if FUNCTION_A
    {
        "FunctionA",
		FunctionA,
        //sched_get_priority_max(SCHED_FIFO),
		0,
        -1,
    },
#endif

#if FUNCTION_B
    {
        "FunctionB",
		FunctionB,
        //sched_get_priority_max(SCHED_FIFO)  - 2,
		2,
        -1,
    }
#endif
};
/*
 * 函数名      : RegisterMessage
 * 功能        : 注册消息队列
 * 参数        : s32Index [in] : 线程索引
 *             : boMsgQueue [in] : 是否需要消息队列
 * 返回        : 正确返回0, 错误返回-1
 * 作者        : 吕春豪 2016-2-9
 */
static int RegisterMessage(int s32Index, bool boMsgQueue)
{
    if(boMsgQueue)
    {
        g_stThrdObjs[s32Index].s32MsgQueID = msgget(IPC_PRIVATE, 0777 | IPC_CREAT);
        if(g_stThrdObjs[s32Index].s32MsgQueID < 0)
        {
            return -1;
        };
    }

    return 0;
}
/*
 * 函数名      : RegisterThreadMessages
 * 功能        : 创建线程消息队列
 * 参数        : 无
 * 描述        : 有些线程不需要消息队列，为了确保给相应g_stThrdObjs[s32Index]赋值，
 * 				 故添加语句RegisterMessage(i ++, false), 注意对应关系
 * 返回        : 正确返回0, 错误返回-1
 * 作者        : 吕春豪 2016-2-9
 */
static int RegisterThreadMessages()
{
    int i = 0;

#if FUNCTION_A
    //FunctionA
    RegisterMessage(i ++, false);
#endif

#if FUNCTION_B
    //FunctionB
    RegisterMessage(i ++, false);
#endif

    return 0;
}
/*
 * 函数名      : RegisterThreadMessages
 * 功能        : 注销线程消息队列
 * 参数        : 无
 * 返回        : 正确返回0, 错误返回-1
 * 作者        : 吕春豪 2016-2-9
 */
static int UnregisterThreadMessages()
{
	unsigned int i;

	for(i = 0; i < sizeof(g_stThrdObjs) / sizeof(g_stThrdObjs[0]); i ++)
    {
 //   	printf("IM HERE! %s\n", g_stThrdObjs[i].pThrdName);
        if(g_stThrdObjs[i].s32MsgQueID >= 0)
        {
            if(msgctl(g_stThrdObjs[i].s32MsgQueID, IPC_RMID, NULL) < 0)
            {
                printf("Failed to delete thread %s's messages queue\n", g_stThrdObjs[i].pThrdName);
            }
        }
    }

    return 0;
}
/*
 * 函数名      :  ProcessStop
 * 功能        : 信号函数
 * 参数        : 信号量
 * 返回        :
 * 作者        : 吕春豪 2016-2-9
 */
static void ProcessStop(int32_t s32Signal)
{
    printf("Signal is %d\n", s32Signal);
    g_boIsExit = true;
}
/*
 * 函数名      : MainFun
 * 功能        : 主函数，未来替代main()
 * 参数        :
 * 返回        : 正确返回0, 错误返回-1
 * 作者        : 吕春豪
 */
int main(int argc, const char *argv[])
{
    int32_t i;
	void *pThrdResult;
	unsigned int u32ThrdMask = 0;
    pthread_attr_t stPthrdAttr;
    struct sched_param stSchedParam;

	printf("I will call the funcion named FunB\n");
	FunB();

    //接收停止信号终止进程
    signal(SIGINT, ProcessStop);
    signal(SIGTERM, ProcessStop);

    //初始化线程属性
    if(pthread_attr_init(&stPthrdAttr))
    {
        printf("Failed to init pthread attr!\n");
        CLEANUP();
    }
#if HAS_CROSS
    //线程调度方式由属性明确指定
    if(pthread_attr_setinheritsched(&stPthrdAttr, PTHREAD_EXPLICIT_SCHED))
    {
    	printf("Failed to set inherit schedule!\n");
        CLEANUP();
    }

    //使用FIFO调度机制
    if(pthread_attr_setschedpolicy(&stPthrdAttr, SCHED_FIFO))
    {
    	printf("Failed to set scheduled policy!\n");
        CLEANUP();
    }
#endif
    //创建线程消息队列
    if(RegisterThreadMessages() < 0)
    {
    	printf("Failed to create message queues\n");
        CLEANUP();
    }

    //创建线程
    for(i = 0; i < sizeof(g_stThrdObjs) / sizeof(g_stThrdObjs[0]); i ++)
    {
#if HAS_CROSS
        //设置线程优先级
    	g_stThrdObjs[i].u32ThrdPrio = sched_get_priority_max(SCHED_FIFO) - g_stThrdObjs[i].u32ThrdPrio;
        stSchedParam.sched_priority = g_stThrdObjs[i].u32ThrdPrio;
        if(pthread_attr_setschedparam(&stPthrdAttr, &stSchedParam))
        {
        	printf("Failed to set scheduled parameters!\n");
            CLEANUP();
        }
#endif
        //创建线程
        if(pthread_create(&g_stThrdObjs[i].stThrd, &stPthrdAttr, g_stThrdObjs[i].pThrdFxn, &g_stThrdObjs[i]))
        {
        	printf("Failed to create thread %s\n", g_stThrdObjs[i].pThrdName);
            break;
        }
        else
        {
        	printf("Create thread [%d]-%s\n", i, g_stThrdObjs[i].pThrdName);
        }

        u32ThrdMask |= (unsigned int)1 << i;
    }

    pthread_attr_destroy(&stPthrdAttr);

    while(!g_boIsExit)
    {
    	sleep(3);
    }

CLEANUP:
    //删除全部线程消息队列
    UnregisterThreadMessages();

    //等待全部线程结束
    for(i = 0; i < MAX_NUM_OF_THREADS; i ++)
    {
        //如果该线程被创建过，则等待该线程结束
        if(u32ThrdMask & ((unsigned int)1 << i))
        {
            //等待特定线程结束
            if(pthread_join(g_stThrdObjs[i].stThrd, &pThrdResult))
            {
            	printf("Thread %s join failed!\n", g_stThrdObjs[i].pThrdName);
            }
            else
            {
            	printf("Thread %s has gone with wind i = %d!\n", g_stThrdObjs[i].pThrdName, i);
            }
        }
    }

    printf("Main progress finish normal \n");
	return 0;
}

/*
 * 函数名      : FunctionA
 * 功能        : 测试A线程
 * 参数        : pArgs [in] : 线程参数
 * 返回        : 无
 * 作者        : 吕春豪 2016-2-9
 */
void *FunctionA(void *pArgs)
{
	printf("This is function A\n");
	return NULL;
}
/*
 * 函数名      : FunctionB
 * 功能        : 测试B线程
 * 参数        : pArgs [in] : 线程参数
 * 返回        : 无
 * 作者        : 吕春豪 2016-2-9
 */
void *FunctionB(void *pArgs)
{
	printf("This is function B\n");
	return NULL;
}

