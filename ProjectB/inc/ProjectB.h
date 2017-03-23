/****************************************************************************
 * Copyright(c), 2001-2060, ******************************* 版权所有
 ****************************************************************************
 * 文件名称             : ProjectB.h
 * 版本                 : 0.0.1
 * 作者                 : 许龙杰
 * 创建日期             : 2014年7月11日
 * 描述                 : 
 ****************************************************************************/
#ifndef PROJECTB_H_
#define PROJECTB_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <pthread.h>

typedef struct _tagStThrdObj
{
    const char    *pThrdName;
    void          *(*pThrdFxn)(void *);     //线程函数
    uint32_t      u32ThrdPrio;              //线程优先级
    int32_t       s32MsgQueID;              //本线程用于接受消息的消息队列ID
    pthread_t     stThrd;                   //线程数据结构(句柄)
}StThrdObj;

void FunB (void);

#ifdef __cplusplus
}
#endif

#endif /* PROJECTB_H_ */
