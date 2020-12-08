#include "tfps_app.h"

/* 获取指纹图像 */
static int FPS_Capture(void)
{
    int ret = 0;
    int  i = 0;

    printf("Please input finger...\r\n");
    for( i = 0; i < FingerWaitLoopNum; i++ )
    {
        ret = TFPSLib_Capture(FPDATA);
        if (ret != TFPRet_NoFinger)
            break;
    }
    if (ret == TFPRet_NoFinger)
        printf("TFPSLib_Capture RET = TFPRet_NoFinger\r\n");
    else
        printf("TFPSLib_Capture RET = %d\r\n", ret);

    return ret;
}
/* 制作指纹特征 */
static int FPS_MkFeat(unsigned char bufNum)
{
    int ret = 0;

    if (bufNum < 1 || bufNum > TFPS_FEATCNT_PER_REC)
    {
        ret = TFPRet_InvalidParam;
        printf("FPS_MkFeat RET = TFPRet_InvalidParam\r\n");
        return ret;
    }
    ret = FPS_Capture();
    if (ret != TFPRet_OK)
        return ret;
    ret = TFPSLib_MkFeat(FPDATA, bufNum);
    if (ret == TFPRet_OK)
        printf("TFPSLib_MkFeat RET = TFPRet_OK bufNum = %d\r\n", bufNum);
    else if (ret == TFPRet_MkFeatErr)
        printf("TFPSLib_MkFeat RET = TFPRet_MkFeatErr bufNum = %d\r\n", bufNum);
    else
        printf("TFPSLib_MkFeat RET = %d BUFNUM = %d\r\n", ret, bufNum);

    return ret;
}
/* 存储指纹模板 */
static int FPS_RegTemplate(unsigned char nRegID)
{
    int ret = 0;

    ret = TFPSLib_RegTemplate(FPDATA, nRegID);
    if (ret == TFPRet_OK)
        printf("TFPSLib_RegTemplate RET = TFPRet_OK RegID = %d\r\n", nRegID);
    else if (ret == TFPRet_InvalidParam)
        printf("TFPSLib_RegTemplate RET = TFPRet_InvalidParam nRegID = %d\r\n", nRegID);
    else
        printf("TFPSLib_RegTemplate RET = %d nRegID = %d\r\n", ret, nRegID);

    return ret;
}
/* 全局比对指纹 */
static void FPS_Search(unsigned char probeBuf, int* nUIDArr, int* nScoreArr)
{
    int ret = 0;

    ret = TFPSLib_Search(FPDATA, probeBuf, nUIDArr, nScoreArr);
    if (ret == TFPRet_OK && nUIDArr[0] >= 0)
        printf("TFPSLib_Search RET = TFPRet_OK UID = %d, Score = %d\r\n", nUIDArr[0], nScoreArr[0]);
    else if (ret == TFPRet_NotMatched)
        printf("TFPSLib_Search RET = TFPRet_NotMatched\r\n");
    else
        printf("TFPSLib_Search RET = %d UID = %d, Score = %d\r\n", ret, nUIDArr[0], nScoreArr[0]);
}
/* 清空指纹数据库 */
static void FPS_Empty(void)
{
    int ret = 0;

    ret = TFPSLib_Empty(FPDATA);
    if (ret == TFPRet_OK)
        printf("TFPSLib_Empty RET = TFPRet_OK\r\n");
    else
        printf("TFPSLib_Empty RET = %d\r\n", ret);
}
/* 获取可用的空闲模板号 */
static int FPS_GetAvailableID(unsigned char* regID)
{
    int ret = 0;

    ret = TFPSLib_GetAvailableID(FPDATA, regID);
    if (ret == TFPRet_OK)
        printf("TFPSLib_GetAvailableID RET = TFPRet_OK regID = %d\r\n", *regID);
    else if (ret == TFPRet_DBOverflow)
        printf("TFPSLib_GetAvailableID RET = TFPRet_DBOverflow\r\n");
    else
        printf("TFPSLib_GetAvailableID RET = %d regID = %d\r\n", ret, *regID);

    return ret;
}
/* 注册比对清空流程测试函数 */
void tfps_app(void)
{
    int ret = 0;
    int i;
    unsigned char regID;
    int nUIDArr[2];
    int nScoreArr[2];

    /* enroll a finger */
    printf("Enroll a finger\r\n");
    delay_ms(500);
    ret = FPS_GetAvailableID(&regID);
    if (ret != TFPRet_OK)
        return;
    for( i = 1; i <= TFPS_FEATCNT_PER_REC; )
    {
        ret = FPS_MkFeat(i);
        if (ret == TFPRet_OK)
        {
            printf("Please release finger...\r\n");
            while(TFPSLib_Capture(FPDATA) == TFPRet_OK);
            if (TFPSLib_Capture(FPDATA) != TFPRet_NoFinger)
                return;
            i++;
        }
        else if (ret == TFPRet_MkFeatErr)
        {
            printf("Please release finger...\r\n");
            while(TFPSLib_Capture(FPDATA) == TFPRet_OK);
            if (TFPSLib_Capture(FPDATA) != TFPRet_NoFinger)
                return;
        }
        else
            return;
    }
    ret = FPS_RegTemplate(regID);

    /* match a finger */
    printf("Match a finger\r\n");
    delay_ms(500);
    if (ret == TFPRet_OK)
    {
        for( i = 1; i <= TFPS_FEATCNT_PER_REC; )
        {
            ret = FPS_MkFeat(i);
            if (ret == TFPRet_OK)
            {
                FPS_Search(i, nUIDArr, nScoreArr);
                printf("Please release finger...\r\n");
                while(TFPSLib_Capture(FPDATA) == TFPRet_OK);
                if (TFPSLib_Capture(FPDATA) != TFPRet_NoFinger)
                    return;
                i++;
            }
            else if (ret == TFPRet_MkFeatErr)
            {
                printf("Please release finger...\r\n");
                while(TFPSLib_Capture(FPDATA) == TFPRet_OK);
                if (TFPSLib_Capture(FPDATA) != TFPRet_NoFinger)
                    return;
            }
            else
            {
                FPS_Empty();
                return;
            }
        }
    }

    /* empty database */
    printf("Empty database\r\n");
    delay_ms(500);
    FPS_Empty();
}
