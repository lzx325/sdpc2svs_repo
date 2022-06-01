#pragma once
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include "param.h"
#include "baseParam.h"
#ifdef WINDOWS
//#include <basetsd.h>
#include <fstream>
#include <iostream>
#include <io.h>
#define DECLSPEC extern "C" _declspec(dllexport)
#elif LINUX
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
//#include "baseParam.h"
#define DECLSPEC extern "C"
#endif

#define TIFF_FLAG	0x4949
#define VERSION	0x002B
#define OFFSET_SIZE	0x0008

#define DEFAULT_TILE_WIDTH	512
#define DEFAULT_TILE_HEIGHT	512

#define MIN_JPEG_QUALITY 50
#define MAX_JPEG_QUALITY 100
#define DEFAULT_JPEG_QUALITY 75 //输出svs默认图像质量

#define IS_COLOR_CORRECT true //是否颜色校正

#define IS_CUSTON_TILE_SIZE false //是否自定义瓦片大小，否的话，根据sdpc的瓦片大小直接输出，是则按自定义的瓦片参数输出

#define	TIFFTAG_SUBFILETYPE	254	//数据类型
#define		FILETYPE_LAYER_IMAGE	0	//层级图
#define		FILETYPE_THUMBNAIL_IMAGE	0	//缩略图
#define		FILETYPE_LABEL_IMAGE	1	//标签图
#define		FILETYPE_MACROGRAPH_IMAGE	9	//宏观图

/*图像存储类型*/
#define	TILE_TYPE	0	//瓦片
#define	STRIP_TYPE	1	//条带

#define	TIFFTAG_IMAGEWIDTH	256	//图像宽度
#define	TIFFTAG_IMAGELENGTH	257	//图像高度

#define	TIFFTAG_BITSPERSAMPLE	258	//每个像素位数
#define		DEFAULT_BITSPERSAMPLE	0x800080008	//默认RGB888

#define	TIFFTAG_COMPRESSION	259	//压缩方式	
#define		COMPRESSION_NONE 1
#define	    COMPRESSION_LZW	5
#define		COMPRESSION_JPEG 7

#define TIFFTAG_PHOTOMETRIC	262	//色彩空间
#define		PHOTOMETRIC_RGB	2
#define		PHOTOMETRIC_YCBCR	6

#define	TIFFTAG_IMAGEDESCRIPTION	270	//图像描述

#define	TIFFTAG_SAMPLESPERPIXEL		277	//通道数
#define		DEFAULT_SAMPLESPERPIXEL	3	//3通道

#define TIFFTAG_STRIPOFFSETS	273	//条带偏移量
#define	TIFFTAG_ROWSPERSTRIP		278	//每个条带的行数
#define	TIFFTAG_STRIPBYTECOUNTS		279	//每个条带的字节数

#define	TIFFTAG_PAGENUMBER	297	//页数
#define		PAGENUMBER(page, pages)	((((UINT64)pages) << 16) | ((UINT64)page))	//page页码，pages页数

#define	TIFFTAG_PREDICTOR		317	/* prediction scheme w/ LZW */
#define     PREDICTOR_NONE		1	/* no prediction scheme used */
#define     PREDICTOR_HORIZONTAL	2	/* horizontal differencing */
#define     PREDICTOR_FLOATINGPOINT	3	/* floating point predictor */

#define	TIFFTAG_TILEWIDTH	322	//瓦片宽度
#define	TIFFTAG_TILELENGTH	323	//瓦片高度

#define TIFFTAG_TILEOFFSETS	324	//瓦片偏移量
#define TIFFTAG_TILEBYTECOUNTS	325	//瓦片大小偏移量
#define	TIFFTAG_YCBCRSUBSAMPLING	530
#define	TIFFTAG_IMAGEDEPTH		32997	/* z depth of image */
#define	TIFFTAG_JPEGQUALITY		65537	//JPEG图像压缩质量

typedef enum {
    TIFF_NOTYPE = 0,      /* placeholder */
    TIFF_BYTE = 1,        /* 8-bit unsigned integer */
    TIFF_ASCII = 2,       /* 8-bit bytes w/ last byte null */
    TIFF_SHORT = 3,       /* 16-bit unsigned integer */
    TIFF_LONG = 4,        /* 32-bit unsigned integer */
    TIFF_RATIONAL = 5,    /* 64-bit unsigned fraction */
    TIFF_SBYTE = 6,       /* !8-bit signed integer */
    TIFF_UNDEFINED = 7,   /* !8-bit untyped data */
    TIFF_SSHORT = 8,      /* !16-bit signed integer */
    TIFF_SLONG = 9,       /* !32-bit signed integer */
    TIFF_SRATIONAL = 10,  /* !64-bit signed fraction */
    TIFF_FLOAT = 11,      /* !32-bit IEEE floating point */
    TIFF_DOUBLE = 12,     /* !64-bit IEEE floating point */
    TIFF_IFD = 13,        /* %32-bit unsigned integer (offset) */
    TIFF_LONG8 = 16,      /* BigTIFF 64-bit unsigned integer */
    TIFF_SLONG8 = 17,     /* BigTIFF 64-bit signed integer */
    TIFF_IFD8 = 18        /* BigTIFF 64-bit unsigned integer (offset) */
} TIFFDataType;

#pragma pack(1)
/*typedef struct Size
{
int Width;
int Height;
}Size;*/
typedef struct SvsConfig
{
    int Quality = DEFAULT_JPEG_QUALITY;
    int TileWidth = DEFAULT_TILE_WIDTH;
    int TileHeight = DEFAULT_TILE_HEIGHT;
    //是否自定义瓦片大小，否的话，根据sdpc的瓦片大小直接输出，是则按自定义的瓦片参数输出
    bool IsCustomTileSize = IS_CUSTON_TILE_SIZE; //如果false，上面的设置的TileWdith和TileHeight不会生效
    bool IsColorCorrect = IS_COLOR_CORRECT;
}SvsConfig;

typedef struct
{
    UINT64 DeCount;
    UINT64 CurDirOffset = 0;	//当前目录偏移量
    UINT64 NextDirOffset = 0;	//下一个目录偏移量
    UINT64 FileType;	//数据类型
    Size ImageSize;	//图像大小
    Size TileSize;	//瓦片大小
    UINT64 *StreamSize;	//数据流大小
    UINT64 *StreamOffset;	//数据流偏移量
    UINT64 StreamCounts = 1;	//数据流个数
    char *Description;	//描述
    UINT64 PageNum;	//页码和页数 
    UINT16 ImageSaveType;//存储类型 
    UINT64 Compression;	//压缩方式	
    UINT64 PhotoMetric;	//颜色空间
    UINT64 Quality;	//压缩质量
}Field;

typedef struct
{
    UINT16 Flag = TIFF_FLAG;//0x4949
    UINT16 Version = VERSION;//0x002B
    UINT16 Offset = OFFSET_SIZE;//0x0008
    UINT16 Constant = 0;//0x0000
    UINT64 FirstDirectoryOffset = 0;
}IFH;

typedef struct
{
    UINT16 Tag;
    UINT16 Type;
    UINT64 Count;
    UINT64 Data;
}DE;

typedef enum
{
    E_THUMBNAIL = 0x00,
    E_LABEL,
    E_MACROGRAPH,
}EImage;

typedef struct
{
    FILE *svs;
    SqSdpcInfo *si;
    char *srcPath;
    char *dstPath;
    int quality = 80;
    int tileWidth = 512;
    int tileHeight = 512;
    bool isCorrect = true;
}CSvs;

/************************以下仅供写入svs**************************/
/*打开svs*/
DECLSPEC FILE *OpenSvs(char *path, char *mode);
DECLSPEC int Fseeki64Svs(FILE *svs, INT64 offset, int origin);
DECLSPEC INT64 Ftelli64Svs(FILE *svs);
/*写入IFH*/
DECLSPEC size_t SetSvsIFH(FILE *svs);
/*写入IFD*/
DECLSPEC UINT64 SetSvsIFD(FILE *svs, Field *field);
DECLSPEC size_t WriteSvsData(FILE *svs, void *data, size_t elementSize, size_t elementCount);
DECLSPEC bool WriteStreamData(FILE *svs, unsigned char **stream,
    UINT64 *streamSize, UINT64 *streamOffset, UINT64 count);
/*覆写IFH和IFD*/
DECLSPEC bool OverwriteImageFileInfo(FILE *svs, Field *field, int fieldCount);
/*关闭svs*/
DECLSPEC void CloseSvs(FILE *svs);


/***********************以下为：sdpc 2 svs*********************/

/************************************************************************
*功能：初始化参数
*sdpcPath:SDPC路径
*svsPath:SVS路径
*return：错误返回NULL
************************************************************************/
DECLSPEC CSvs *InitSvs(char *sdpcPath, char *svsPath, SvsConfig *svsConfig);
/************************************************************************
*功能：写入层级图像
*csvs:csvs结构体
*field:字段
*pageNum:页码	#define PAGENUMBER(page, pages) ((((UINT64)pages) << 16) | ((UINT64)page))	//page页码，pages页数
*layer:层级
*return：错误返回false
************************************************************************/
DECLSPEC bool SetSvsLayerImage(CSvs *csvs, Field *field, UINT64 pageNum, int layer);
/************************************************************************
*功能：写入strip图像（只支持缩略图、标签以及宏观图）
*csvs:csvs结构体
field:字段
*eImage:图像类型
*return：错误返回false
************************************************************************/
DECLSPEC bool SetSvsStripImage(CSvs *csvs, Field *field, EImage eImage);
/************************************************************************
*功能：覆写SVS信息
*csvs:csvs结构体
field:字段
*fieldCount:字段个数
*return：错误返回false
************************************************************************/
DECLSPEC bool OverwriteSvsInfo(CSvs *csvs, Field *field, int fieldCount);
/************************************************************************
*功能：释放资源
*csvs:csvs结构体
************************************************************************/
DECLSPEC void DisposeSvs(CSvs *svs);
/************************************************************************
*功能：sdpc转换成Aperio Svs格式
*sdpcPath:sdpc路径
*svsPath:svs路径
************************************************************************/
DECLSPEC bool Sdpc2Svs(char *sdpcPath, char *svsPath, SvsConfig *svsConfig);