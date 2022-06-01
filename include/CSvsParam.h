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
#define DEFAULT_JPEG_QUALITY 75 //���svsĬ��ͼ������

#define IS_COLOR_CORRECT true //�Ƿ���ɫУ��

#define IS_CUSTON_TILE_SIZE false //�Ƿ��Զ�����Ƭ��С����Ļ�������sdpc����Ƭ��Сֱ������������Զ������Ƭ�������

#define	TIFFTAG_SUBFILETYPE	254	//��������
#define		FILETYPE_LAYER_IMAGE	0	//�㼶ͼ
#define		FILETYPE_THUMBNAIL_IMAGE	0	//����ͼ
#define		FILETYPE_LABEL_IMAGE	1	//��ǩͼ
#define		FILETYPE_MACROGRAPH_IMAGE	9	//���ͼ

/*ͼ��洢����*/
#define	TILE_TYPE	0	//��Ƭ
#define	STRIP_TYPE	1	//����

#define	TIFFTAG_IMAGEWIDTH	256	//ͼ����
#define	TIFFTAG_IMAGELENGTH	257	//ͼ��߶�

#define	TIFFTAG_BITSPERSAMPLE	258	//ÿ������λ��
#define		DEFAULT_BITSPERSAMPLE	0x800080008	//Ĭ��RGB888

#define	TIFFTAG_COMPRESSION	259	//ѹ����ʽ	
#define		COMPRESSION_NONE 1
#define	    COMPRESSION_LZW	5
#define		COMPRESSION_JPEG 7

#define TIFFTAG_PHOTOMETRIC	262	//ɫ�ʿռ�
#define		PHOTOMETRIC_RGB	2
#define		PHOTOMETRIC_YCBCR	6

#define	TIFFTAG_IMAGEDESCRIPTION	270	//ͼ������

#define	TIFFTAG_SAMPLESPERPIXEL		277	//ͨ����
#define		DEFAULT_SAMPLESPERPIXEL	3	//3ͨ��

#define TIFFTAG_STRIPOFFSETS	273	//����ƫ����
#define	TIFFTAG_ROWSPERSTRIP		278	//ÿ������������
#define	TIFFTAG_STRIPBYTECOUNTS		279	//ÿ���������ֽ���

#define	TIFFTAG_PAGENUMBER	297	//ҳ��
#define		PAGENUMBER(page, pages)	((((UINT64)pages) << 16) | ((UINT64)page))	//pageҳ�룬pagesҳ��

#define	TIFFTAG_PREDICTOR		317	/* prediction scheme w/ LZW */
#define     PREDICTOR_NONE		1	/* no prediction scheme used */
#define     PREDICTOR_HORIZONTAL	2	/* horizontal differencing */
#define     PREDICTOR_FLOATINGPOINT	3	/* floating point predictor */

#define	TIFFTAG_TILEWIDTH	322	//��Ƭ���
#define	TIFFTAG_TILELENGTH	323	//��Ƭ�߶�

#define TIFFTAG_TILEOFFSETS	324	//��Ƭƫ����
#define TIFFTAG_TILEBYTECOUNTS	325	//��Ƭ��Сƫ����
#define	TIFFTAG_YCBCRSUBSAMPLING	530
#define	TIFFTAG_IMAGEDEPTH		32997	/* z depth of image */
#define	TIFFTAG_JPEGQUALITY		65537	//JPEGͼ��ѹ������

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
    //�Ƿ��Զ�����Ƭ��С����Ļ�������sdpc����Ƭ��Сֱ������������Զ������Ƭ�������
    bool IsCustomTileSize = IS_CUSTON_TILE_SIZE; //���false����������õ�TileWdith��TileHeight������Ч
    bool IsColorCorrect = IS_COLOR_CORRECT;
}SvsConfig;

typedef struct
{
    UINT64 DeCount;
    UINT64 CurDirOffset = 0;	//��ǰĿ¼ƫ����
    UINT64 NextDirOffset = 0;	//��һ��Ŀ¼ƫ����
    UINT64 FileType;	//��������
    Size ImageSize;	//ͼ���С
    Size TileSize;	//��Ƭ��С
    UINT64 *StreamSize;	//��������С
    UINT64 *StreamOffset;	//������ƫ����
    UINT64 StreamCounts = 1;	//����������
    char *Description;	//����
    UINT64 PageNum;	//ҳ���ҳ�� 
    UINT16 ImageSaveType;//�洢���� 
    UINT64 Compression;	//ѹ����ʽ	
    UINT64 PhotoMetric;	//��ɫ�ռ�
    UINT64 Quality;	//ѹ������
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

/************************���½���д��svs**************************/
/*��svs*/
DECLSPEC FILE *OpenSvs(char *path, char *mode);
DECLSPEC int Fseeki64Svs(FILE *svs, INT64 offset, int origin);
DECLSPEC INT64 Ftelli64Svs(FILE *svs);
/*д��IFH*/
DECLSPEC size_t SetSvsIFH(FILE *svs);
/*д��IFD*/
DECLSPEC UINT64 SetSvsIFD(FILE *svs, Field *field);
DECLSPEC size_t WriteSvsData(FILE *svs, void *data, size_t elementSize, size_t elementCount);
DECLSPEC bool WriteStreamData(FILE *svs, unsigned char **stream,
    UINT64 *streamSize, UINT64 *streamOffset, UINT64 count);
/*��дIFH��IFD*/
DECLSPEC bool OverwriteImageFileInfo(FILE *svs, Field *field, int fieldCount);
/*�ر�svs*/
DECLSPEC void CloseSvs(FILE *svs);


/***********************����Ϊ��sdpc 2 svs*********************/

/************************************************************************
*���ܣ���ʼ������
*sdpcPath:SDPC·��
*svsPath:SVS·��
*return�����󷵻�NULL
************************************************************************/
DECLSPEC CSvs *InitSvs(char *sdpcPath, char *svsPath, SvsConfig *svsConfig);
/************************************************************************
*���ܣ�д��㼶ͼ��
*csvs:csvs�ṹ��
*field:�ֶ�
*pageNum:ҳ��	#define PAGENUMBER(page, pages) ((((UINT64)pages) << 16) | ((UINT64)page))	//pageҳ�룬pagesҳ��
*layer:�㼶
*return�����󷵻�false
************************************************************************/
DECLSPEC bool SetSvsLayerImage(CSvs *csvs, Field *field, UINT64 pageNum, int layer);
/************************************************************************
*���ܣ�д��stripͼ��ֻ֧������ͼ����ǩ�Լ����ͼ��
*csvs:csvs�ṹ��
field:�ֶ�
*eImage:ͼ������
*return�����󷵻�false
************************************************************************/
DECLSPEC bool SetSvsStripImage(CSvs *csvs, Field *field, EImage eImage);
/************************************************************************
*���ܣ���дSVS��Ϣ
*csvs:csvs�ṹ��
field:�ֶ�
*fieldCount:�ֶθ���
*return�����󷵻�false
************************************************************************/
DECLSPEC bool OverwriteSvsInfo(CSvs *csvs, Field *field, int fieldCount);
/************************************************************************
*���ܣ��ͷ���Դ
*csvs:csvs�ṹ��
************************************************************************/
DECLSPEC void DisposeSvs(CSvs *svs);
/************************************************************************
*���ܣ�sdpcת����Aperio Svs��ʽ
*sdpcPath:sdpc·��
*svsPath:svs·��
************************************************************************/
DECLSPEC bool Sdpc2Svs(char *sdpcPath, char *svsPath, SvsConfig *svsConfig);