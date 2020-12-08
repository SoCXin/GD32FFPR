#ifndef TFPS_APP_H
#define TFPS_APP_H

#include <stdio.h>
#include "gd32f30x.h"
#include "systick.h"
/* 算法库头文件 */
#include "tfpslib.h"
/* 算法库需要的全局变量 */
extern unsigned char FPDATA[TPFS_Work_Size];
/* 等待手指放置时间是40个单位, 单位时间为120 ~ 140ms */
#define FingerWaitLoopNum       (40)
/* 注册比对清空流程测试函数 */
void tfps_app(void);

#endif
