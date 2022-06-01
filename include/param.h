#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h> 
#include "omp.h"
#include <setjmp.h>
#ifdef WINDOWS
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string> 
#include <time.h>
//#include <windows.h>
#include "JpegWindows/jpeglib.h"
#include <io.h>
#define DECLSPEC extern "C" _declspec(dllexport)
#elif LINUX
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include "JpegLinux/jpeglib.h"
#define DECLSPEC extern "C"
#endif

const int macroNum = 2;

const int ColorRange = 52;
const int ColorStep = 5;	//255/(52-1)

#define COMPRESS_QUALITY 85

const float ColorGamma = 1.0;
const float ColorRgbRate[3] = { 0.830f, 0.774f, 0.760f };
const float ColorHsvRate[3] = { 1.0, 1.0, 1.0 };
const float ColorMatrix[9] = { 1.475890, -0.472316, 0.225171,
-0.062110, 1.179542, 0.197697,
0.252455, -0.793499, 1.884747 };

#define CPU_THREADS	omp_get_num_procs()
#define NUM_THREADS	CPU_THREADS

#pragma pack(1)
typedef enum _ImageFormat
{
    Jpeg = 0x00,
    Bmp,
    Png,
    Tiff,
    Hevc
}ImageFormat;

typedef struct SqImageInfo
{
    unsigned char *jpeg;
    unsigned char *rgb;
    int width;
    int height;
    int channel;
    int jpegSize;
}SqImageInfo;

typedef struct Point
{
    int X;
    int Y;
}CPoint;

typedef struct Size
{
    int Width;
    int Height;
}CSize;

typedef struct Rectangle
{
    int X;
    int Y;
    int Width;
    int Height;
}CRectangle;

typedef struct SqColorTable
{
    unsigned char ***RedTable;
    unsigned char ***GreenTable;
    unsigned char ***BlueTable;
    unsigned char ColorRange[256];
}SqColorTable;

enum SqError
{
    SqSuccess = 0x00,
    SqFileFormatError = -1,	//文件格式错误
    SqOpenFileError = -2,//打开文件错误
    SqReadFileError = -3,//读取文件错误
    SqWriteFileError = -4,//写入文件错误
    SqJpegFormatError = -5,//Jpeg格式错误
    SqEncodeJpegError = -6,//压缩jpeg格式错误
    SqDecodeJpegError = -7,//解压jpeg格式错误
    SqSliceNumError = -8,//切片数量错误
    SqGetSliceRgbError = -9,//获取rgb小图错误
    SqPicInfoError = -10,//picInfo信息错误	
    SqGetThumbnailError = -11,//读取缩略图错误
    SqPicHeadError = -12,//包头信息错误
    SqPathError = -13,//路径错误
    SqDataNullError = -14,//数据为空
    SqPersonInfoError = -15, //病理信息错误
    SqMacrographInfoError = -16,//宏观图信息错误
    SqNotExist = -17,//不存在（假如病理信息与宏观图出现这个，不是错误，不影响后面信息）
    SqLayerIndexesError = -18,//层级索引错误
    SqSliceIndexesError = -19, //指定小图索引错误 
    SqROIRange = -20,	//取值范围错误
    SqBlockJpeg = -21, //自定义将Sdpc切块成Jpeg错误
    SqExtraInfoError = -22,	//额外信息错误
    SqTileImageHeadError = -23,	//白细胞信息头错误
    SqTileImageConfigCheckError = -24,	//血液配置文件校验失败
    SqTileImageConfig2JsonError = -25,	//血液配置文件转换Json失败
    SqTileImageConfigNodeError = -26,	//血液配置文件获取节点失败
    SqTileImageConfigHeadError = -27,	//血液配置文件头信息错误
    SqDecodeHevcError = -28,      //hevc解码错误
};

typedef struct SqPicHead
{
    unsigned short flag; /* 低8位为字母’S’，高8位为字母’Q ' 0x5153*/
    unsigned char version[16];
    unsigned int headSize;	/* 当前结构体的大小*/
    size_t fileSize;		/* 文件大小 */ //22为fileSize变量在SqPicHead结构体中偏移量，不要修改fileSize的名字

    unsigned int macrograph;	//宏观图,0表示没有图，>= 1则表示有多少张宏观图
    unsigned int personInfor;	//病人信息
    unsigned int hierarchy;	/* 层级 */
    unsigned int srcWidth;	/* 原始图像大小 */
    unsigned int srcHeight;
    unsigned int sliceWidth;	/* 切片大小 */
    unsigned int sliceHeight;
    unsigned int thumbnailWidth;
    unsigned int thumbnailHeight;
    /* 色深，即每个像素所占位数(bits per pixel)典型值为：1、4、8、16、24、32 */
    unsigned char bpp;
    /* 压缩质量0-100 */
    unsigned char quality;
    /* 图像色彩空间 */
    J_COLOR_SPACE colrSpace;
    /* 缩放比例 */
    float scale;
    double ruler;/*比例尺，一个像素对应多大的尺寸，单位为um*/
                 /* 预留空间 */
    unsigned int rate;/*扫描倍率*/
    size_t extraOffset;/*额外信息偏移量，存储着CCM、相机参数，若为0，则表示不存在*/
    size_t tileOffset;/*白细胞图像偏移量，内嵌有白细胞图像，若为0，则表示不存在*/
    ImageFormat sliceFormat; //切片格式
    unsigned char headSpace[48];	//预留空间
}SqPicHead;

typedef struct SqMacrographInfo
{
    unsigned short flag;	/*低8位为字母’M’，高8位为字母’I '0x494D*/
                         //unsigned int macrographNum;	//共几张
                         //unsigned int curMacrograph;	//第几张宏观图
    unsigned char *rgb;
    unsigned int width;
    unsigned int height;
    unsigned int chance;
    unsigned int step;
    size_t rgbSize;
    size_t encodeSize;	//jpeg大小
    unsigned char quality;
    size_t nextLayerOffset;
    unsigned int headSpace_1;
    unsigned int headSpace_2;
    unsigned char headSpace[64];
}SqMacrographInfo;

typedef struct SqPicInfo
{
    unsigned short flag; /* 低8位为字母’F’，高8位为字母’I ' 0x4649*/
    unsigned int infoSize; /* 当前结构体的大小*/
                           /* 第几层 */
    unsigned int layer;
    /* 切片数量 */
    unsigned int sliceNum;
    /* 切片横竖坐标的数量 */
    unsigned int scliceNumX;
    unsigned int scliceNumY;
    /* 图层大小 */
    size_t layerSize;
    /* 下一图层结构体偏移量 */
    size_t nextLayerOffset;
    /* 缩放比例 */
    float curScale;
    double ruler; //微米为单位

    unsigned int defaultX;/*补白边，X或Y最后一张的X或Y存在多少像素的白边，为了固定切片的大小*/
    unsigned int defaultY;
    unsigned char headSpace[64];	//预留空间
}SqPicInfo;

typedef struct SqPersonInfo
{
    unsigned short flag;	/*标志位 低8位为字母’P’，高8位为字母’I ' 0x4950*/
    unsigned int inforSize;	//个人信息大小
    unsigned char pathologyID[64];	//病理号
    unsigned char name[64];
    unsigned char sex;	//性别,1位男，0为女
    unsigned char age;
    unsigned char departments[64];	//科室 
    unsigned char hospital[64];
    unsigned char submittedSamples[1024];	//送检样本信息
    unsigned char clinicalDiagnosis[2048];	//临床诊断
    unsigned char pathologicalDiagnosis[2048];	//病理诊断
    unsigned char reportDate[64];	//报告日期
    unsigned char attendingDoctor[64];	//主诊医生
    unsigned char remark[1024];	//备注信息
    size_t nextOffset;	//下一个偏移量
    unsigned int reversed_1;	/* 保留字1，必须设置为0 */
    unsigned int reversed_2;	/* 保留字2，必须设置为0 */
    unsigned char reversed[256];	//保留字节
}SqPersonInfo;

typedef struct SqExtraInfo
{
    short flag; 	/*标志位 低8位为字母’E’，高8位为字母’I ' 0x4945*/
    unsigned int inforSize;	//额外信息大小
    size_t nextOffset;	//下一个偏移量
                         /*CCM颜色校准参数*/
    unsigned char model[20];    //相机型号
    float ccmGamma;//gamma
    float ccmRgbRate[3]; //rgb比例
    float ccmHsvRate[3];//hsv比例
    float ccm[9];//3*3矩阵

    unsigned char timeConsuming[32];//扫描时刻
    unsigned int scanTime; //扫描时间
    unsigned short stepTime[10];   //每步耗时
    unsigned char serial[32];   //序列号
    unsigned char fusionLayer;    //融合层数
    float step;  //步进
    unsigned short focusPoint;    //对焦点
    unsigned short validFocusPoint;    //有效对焦点

    unsigned char barCode[128];  //条形码

    float cameraGamma;
    float cameraExposure;
    float cameraGain;

    unsigned char reversed[433];	//保留字节
}SqExtraInfo;

typedef struct SqSliceInfo
{
    size_t *sliceOffset;/*切片偏移量*/
    int *sliceSize;/*切片大小*/
}SqSliceInfo;

/********************血液图像中内嵌的白细胞图**********************/
/***************可根据sdpc的tileoffset来判断是否存在***************/
typedef struct SqTileImageHead
{
    short flag; //标志位 0x4954
    unsigned int infoSize; //结构体大小
    size_t curOffset;    //当前偏移量
    size_t nextOffset;   //下个数据偏移量
    int width;   //图像宽度
    int height;  //图像高度
    int imgCount;    //图像数目
    unsigned char quality;    //压缩质量
    J_COLOR_SPACE colrSpace; //图像色彩空间 
    char guid[36];
    unsigned char headSpace[(1 << 20) - 36]; //预留空间
}SqTileImageHead;

typedef struct SqTileImageInfo
{
    SqTileImageHead *tileImageHead;
    SqSliceInfo *tileImageOffset;
}SqTileImageInfo;
/*************************************************************/

/****************以下是血液配置文件信息（sdpl）***************/
typedef struct SqTileImageHeadConfig
{
    unsigned short flag;//低8位为字母’T’，高8位为字母’H ' 0x4854
    unsigned int headSize;//结构体大小
    size_t configSize;//配置文件大小
    char lnkName[128];  //关联文件名
    int tileImageCount;  //图像个数
    int width;
    int height;
    unsigned char type;   //类型总数
    size_t nextOffset;   //偏移量
    char guid[36];
    unsigned char headSpace[(1 << 20) - 36]; //预留空间
}SqTileImageHeadConfig;

/*单张白细胞图像的信息*/
typedef struct SqTileImageInfoConfig
{
    unsigned short flag;//低8位为字母’T’，高8位为字母’I ' 0x4954
    unsigned int id;
    unsigned int infoSize;
    // short type;
    unsigned int roiCount;
    size_t roiAddr; //预留空间
    size_t curOffset;    //当前偏移量
    size_t nextOffset;   //下个数据偏移量
    char note[512];  //标注
    char headSpace[2048]; //预留空间
}SqTileImageInfoConfig;

/*图像中roi的位置以及信息*/
typedef struct SqTileImageRoiConfig
{
    unsigned short type; //类别
    CRectangle rect;
    char text[512];  //标注
    unsigned char headSpace[128]; //预留空间
}SqTileImageRoiConfig;

typedef struct TileImageRoiInfo
{
    SqTileImageInfoConfig *TileImageInfo;/*白细胞图像信息*/
    SqTileImageRoiConfig **TileImageRoi;/*ROI信息*/
}TileImageRoiInfo;

/*配置文件预分类的根节点*/
/*一个根目录包括一个头信息以及图像信息*/
/*一张白细胞图像包括N个ROI*/
typedef struct ClassifyRoot
{
    SqTileImageHeadConfig *TileImageHeadConfig;/*头信息*/
    TileImageRoiInfo **ClassifyImageRoiInfo;/*图像roi信息*/
}ClassifyRoot;
/*************************************************************/

/*所有获取信息的结构体*/
typedef struct SqSdpcInfo
{
    char *fileName;

    SqPicHead *picHead;/*文件包头信息*/
    SqPersonInfo *personInfo;/*病人信息*/
    SqExtraInfo *extra;//额外信息（包括颜色校正）
    SqImageInfo **macrograph;/*宏观图*/
    SqImageInfo *thumbnail;/*缩略图*/
    SqPicInfo **sliceLayerInfo;//切片层级信息
    SqSliceInfo **sliceInfo;//切片位置以及大小信息
    SqColorTable *colorTable;
#ifdef BLOOD_IMAGE
    SqTileImageInfo *tileImageInfo;//血液白细胞信息（包括一系列单张白细胞图）
    ClassifyRoot *classifyRoot;	//血液白细胞配置文件信息
#endif
}SqSdpcInfo;
/************************************************************************/
/*                       以下函数跟标准C一样                            */
/************************************************************************/
DECLSPEC void SqFreeMemory(void *buf);

DECLSPEC void *SqMemset(void *dst, int val, size_t size);

DECLSPEC FILE *SqOpenFile(char *path, char *mode);

DECLSPEC void SqCloseFile(FILE *file);

DECLSPEC inline size_t SqWriteFileData(FILE *file, void *data, size_t size);

/*获取打开的文件大小*/
DECLSPEC size_t SqGetFileSize(FILE *file);
/*获取指定路径的文件大小*/
DECLSPEC size_t SqGetFileSizeSystemCall(char *path);

DECLSPEC inline unsigned char *SqReadFileData(FILE *file);

DECLSPEC inline size_t SqReadData(void *data, size_t elementSize, size_t elementCount, FILE *file);

DECLSPEC inline size_t SqWriteData(void *data, size_t elementSize, size_t elementCount, FILE *file);

DECLSPEC void SqRewind(FILE *file);

DECLSPEC int SqFseeki64(FILE *file, size_t offset, int origin);

DECLSPEC size_t SqFtelli64(FILE *file);
/************************************************************************/
/*                       以上函数跟标准C一样                            */
/************************************************************************/

/************************************************************************
*功能：将rgb888转Jpeg
*src:rgb数据
*dst:转换的jpeg数据
*quality:压缩质量
*colrSpace:图像数据的色彩空间
*return：错误返回小于0（错误信息见SqError枚举）,正确返回压缩后Jpeg的大小
************************************************************************/
DECLSPEC GLOBAL(unsigned long)
SqEncodeJpeg(unsigned char *src, unsigned char **dst, int quality, int width, int height, J_COLOR_SPACE colrSpace);

/************************************************************************
*功能：将jpeg转rgb888
*src:转换的jpeg数据
*srcSize:转换的jpeg数据的大小
*buffer:解压后rgb的数据
*colrSpace:图像数据的色彩空间
*return：错误返回小于0（错误信息见SqError枚举）,正确返回解压后rgb的大小
************************************************************************/
DECLSPEC GLOBAL(size_t)
SqDecodeJpeg(unsigned char *src, unsigned long srcSize, unsigned char **buffer, J_COLOR_SPACE colrSpace);

/************************************************************************/
/*功能：经过CCM校准后rgb颜色映射表
/*rgbRate：输入rgbRate比例
/*hsvRate：输入hsvRate比例
/*gamma：输入gamma值
/*redTable:输出的red映射表（外部申请空间）red[256][256][256]
/*greenTable:输出的green映射表（外部申请空间）green[256][256][256]
/*blueTable:输出的blue映射表（外部申请空间）blue[256][256][256]
/************************************************************************/
DECLSPEC SqColorTable *InitColorCollectTable(float *rgbRate, float *hsvRate, float gamma, float *ccm);

/************************************************************************/
/*功能：为rgb映射表申请空间
/*return:初始化的颜色通道
/************************************************************************/
DECLSPEC void DisposeColorCorrectTable(SqColorTable *ct);

/************************************************************************/
/*功能：输出CCM校准后rgb颜色
/*srcRgb：输入原始rgb数据
/*dstRgb：输入校准后的rgb数据
/*width：图像宽度
/*height：图像高度
/*redTable:输入red映射表
/*greenTable:输入green映射表
/*blueTable:输入blue映射表
/*return:错误返回false，正确返回true
/************************************************************************/
DECLSPEC boolean RgbColorCorrect(unsigned char *srcRgb, unsigned char *dstRgb,
    int width, int height, SqColorTable *colorTable);

DECLSPEC GLOBAL(int) SqOpenSdpc(char *sdpcPath, SqSdpcInfo **sdpcInfo);

/************************************************************************
*功能：获取指定层级的指定位置的小Jpeg图
*picInfo:指定层级信息
*jpegSlice：转换后的小Jpeg图数据
*size：小Jpeg图大小
*layer：指定的层级（默认从0开始）
*x和y：指定图片中小Jpeg图的位置
*return：错误返回小于0（错误信息见SqError枚举），正确返回0
************************************************************************/
DECLSPEC GLOBAL(int)
SqGetJpegSliceOfSpecifyLayer(SqSdpcInfo *si, unsigned char **jpegSlice, unsigned int *size, int layer, int x, int y);

DECLSPEC GLOBAL(int)
SqGetHevcSliceOfSpecifyLayer(SqSdpcInfo *si, unsigned char **hevcSlice, unsigned int *size, int layer, int x, int y);
/*************************慎用（耗内存）*********************************
*功能：将指定层所有小切片转为一张超大的RGB图片（width_height_layer.rgb）
*rgb：转换后的rgb数据
*rgbWidth：转换后的rgb宽度
*rgbHeight：转换后的rgb高度
*layer：指定的层级（默认从0开始）
*return：错误返回小于0（错误信息见SqError枚举），正确返回0
************************************************************************/
DECLSPEC GLOBAL(int)
SqSpecifyLayerJpeg2Rgb(SqSdpcInfo *si, unsigned char **rgb, int *rgbWidth, int *rgbHeight, int layer, bool isCorrect = false);

/************************************************************************
*功能：将指定层以指定像素为起点获取相应的RGB888（width_height_layer.rgb）
*rgb：转换后的rgb数据
*rgbWidth：需要获取rgb宽度
*rgbHeight：需要获取rgb高度
*xPixel和yPixel：起点坐标
*layer：指定的层级（默认从0开始）
*return：错误返回小于0（错误信息见SqError枚举），正确返回0
************************************************************************/
DECLSPEC GLOBAL(int)
SqGetRoiRgbOfSpecifyLayer(SqSdpcInfo *si, unsigned char **rgb, int rgbWidth, int rgbHeight,
    unsigned int xPixel, unsigned int yPixel, int layer, bool isCorrect = false);

/************************************************************************
*功能：将sdpc划分为xSlice * ySlice块Jpeg
*blockJpeg：要获取xSlice * ySlice块Jpeg
*quality:压缩质量
*xSlice:x方向的块数
*ySlice:y方向的块数
*colrSpace:图像数据的色彩空间
*return：错误返回小于0（错误信息见SqError枚举）
************************************************************************/
DECLSPEC GLOBAL(int)
SqGetBlockJpegFromSdpc(SqSdpcInfo *si, SqImageInfo *blockJpeg, int quality, int xSlice, int ySlice,
    int layer, J_COLOR_SPACE colrSpace, bool isCorrect = false);

DECLSPEC GLOBAL(void) SqCloseSdpc(SqSdpcInfo *si);

#ifdef BLOOD_IMAGE
/*针对血液图像，并带有sdpl*/
/************************************************************************
*功能：获取血液图像配置文件中的信息
*sdpcFileName：关联的sdpc文件，代码会通过sdpc文件名去找到关联的sdpl文件
*sdpcTileImageHeadGuid:sdpc血液图像中的tileImageHead中的guid，用于检验关联文件是否一致
*classify:配置文件中分类信息
*return：错误返回小于0（错误信息见SqError枚举）
************************************************************************/
DECLSPEC GLOBAL(int) GetSdpcClassifyConfig(char *sdpcFileName, char *sdpcTileImageHeadGuid, ClassifyRoot **classify);
/*释放血液图像配置文件所有信息*/
DECLSPEC GLOBAL(void) DisposeClassifyRoot(ClassifyRoot *root);

/*获取sdpc指定的白细胞图像,index第几张图像，从0开始*/
DECLSPEC GLOBAL(int) GetSingleTileImageFromBloodSdpc(SqSdpcInfo *si, unsigned char **jpeg, unsigned int *jpegSize, int index);
#endif