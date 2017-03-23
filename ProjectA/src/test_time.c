/*
 * test_time.c
 *
 *  Created on: 2017年1月22日
 *      Author: ubuntu
 */
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>


/*
 * 函数名      : FunctionTimeTest
 * 功能        : 关于日期时间相关系统函数测试
 * 参数        : void
 * 返回值      : void
 * 作者        : 吕春豪  2017-1-22
 */
void FunctionTimeTest()
{
	char *u8Wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

	/* 获取格林威治时间 */
	time_t timep;
	timep = time(NULL);

	/* 格林威志时间转换成真实世界所使用的日期表示方法*/
	struct tm *p;
	p = gmtime(&timep);
	printf("%d/%d/%d ", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday);
	printf("%s %d:%d:%d \n", u8Wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
	printf("%s \n", asctime(p));    //将日期和时间以字符串的形式打印出来

	/* 获得当地目前时间和日期(GTM + 时区) */
	p = localtime(&timep);
	printf("%s \n", asctime(p));    //将日期和时间以字符串的形式打印出来

	/*
	 * 获得目前时间/时区/以及夏令时
    */
	struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);
	printf("tv_sec: %ld \n", tv.tv_sec);                    //秒
	printf("tv_usec: %ld \n", tv.tv_usec);                  //微妙
	printf("tz_minuteswest: %d \n", tz.tz_minuteswest);     //和Greenwich时间差了多少分钟
	printf("tz_dsttime: %d \n", tz.tz_dsttime);             //日光节约时间的状态
}
