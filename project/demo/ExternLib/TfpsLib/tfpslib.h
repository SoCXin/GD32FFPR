#ifndef __TOYONWAY_TFPSLIB_H__
#define __TOYONWAY_TFPSLIB_H__

//-------------------------------------
//. Types
#define     FpSDK_Feature_Size  (2000)
typedef     unsigned char       FpFeature[FpSDK_Feature_Size];
//-------------------------------------
//. Return Code
#define     TFPRet_OK               (0x0)
#define     TFPRet_ResNotEnough     (0x81)
#define     TFPRet_NoFinger         (0x82)
#define     TFPRet_SensorErr        (0x83)
#define     TFPRet_MkFeatErr        (0x84)
#define     TFPRet_NotMatched       (0x85)
#define     TFPRet_InvalidParam     (0x86)
#define     TFPRet_DBEmpty          (0x87)
#define     TFPRet_DBAccessErr      (0x88)
#define     TFPRet_DBOverflow       (0x89)
#define     TFPRet_LicErr           (0x8A)
#define     TFPRet_BadImg           (0x8C)
#define     TFPRet_CancelOK         (0x8B)
//-------------------------------------
//. API
//. initialization
int TFPSLib_Init(unsigned char* ramStart, int ramSize);

//. initialize sensor.
//. mode = 0; default
//. mode = 1; write FW(version 0042) to sensor 
int TFPSLib_CapInit(unsigned char* libdata, unsigned char mode);

//. image capture.
int TFPSLib_Capture(unsigned char* libdata);

//. extract fingerprint feature. 
int TFPSLib_MkFeat(unsigned char* libdata, unsigned char bufNum);

//. register fingerprint template
int TFPSLib_RegTemplate(unsigned char* libdata, unsigned char nRegID);

//. search fingerprint
int TFPSLib_Search(unsigned char* libdata, unsigned char probeBuf, int* nUIDArr, int* nScoreArr);

//. clean DB.
int TFPSLib_Empty(unsigned char* libdata);

//. remove template
int TFPSLib_RemoveTemplate(unsigned char* libdata, unsigned char nRegID);

//. get empty record ID
int TFPSLib_GetAvailableID(unsigned char* libdata, unsigned char* regID);

//. Get record count
int TFPSLib_GetRecCount(unsigned char* libdata, unsigned char* recCount);

//. check lic
int TFPSLib_CheckLic(unsigned char* libdata);

//. read chip uid
int TFPSLib_ReadChipID(unsigned char* libdata, unsigned char* uidCode);

//. write lic code
int TFPSLib_WriteLic(unsigned char* libdata, unsigned char* licCode);

//. write data to flash
int TFPSLib_WriteBinToFlash(unsigned char* libdata, unsigned int addr, unsigned short* wData, int wordSize);

//. rec size = feature 2000 * 6
#define     TFPS_FEATCNT_PER_REC    (5)
//. RegID = 0 ~ 9
#define     TFPS_REC_MAX            (40)

#define     SFImgWidth      112
#define     SFImgHeight     112

//. recommended DB address in flash(bank1).
// (2000 + 4) * 6 * 45 + 1K = about 128K
#define     TPFS_DB_StartAddr   (0x8080000)

//. TFPSLib ram size
#define     TPFS_Work_Size      (75*1024+10000+1024)
#define     FPData_LibInfo      (75*1024+10000)

// changing clock
#define     GD_SYS_CLOCK_IRC8M              (uint32_t)(8000000)
#define     GD_SYS_CLOCK_48M_PLL_IRC8M      (uint32_t)(48000000)
#define     GD_SYS_CLOCK_72M_PLL_IRC8M      (uint32_t)(72000000)
#define     GD_SYS_CLOCK_96M_PLL_IRC8M      (uint32_t)(96000000)
#define     GD_SYS_CLOCK_108M_PLL_IRC8M     (uint32_t)(108000000)
#define     GD_SYS_CLOCK_120M_PLL_IRC8M     (uint32_t)(120000000)
#define     GD_SYS_CLOCK_168M_PLL_IRC8M     (uint32_t)(168000000)

void GDSysClockUpdata(unsigned int SysClk);

#endif
