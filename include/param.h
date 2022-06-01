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
    SqFileFormatError = -1,	//�ļ���ʽ����
    SqOpenFileError = -2,//���ļ�����
    SqReadFileError = -3,//��ȡ�ļ�����
    SqWriteFileError = -4,//д���ļ�����
    SqJpegFormatError = -5,//Jpeg��ʽ����
    SqEncodeJpegError = -6,//ѹ��jpeg��ʽ����
    SqDecodeJpegError = -7,//��ѹjpeg��ʽ����
    SqSliceNumError = -8,//��Ƭ��������
    SqGetSliceRgbError = -9,//��ȡrgbСͼ����
    SqPicInfoError = -10,//picInfo��Ϣ����	
    SqGetThumbnailError = -11,//��ȡ����ͼ����
    SqPicHeadError = -12,//��ͷ��Ϣ����
    SqPathError = -13,//·������
    SqDataNullError = -14,//����Ϊ��
    SqPersonInfoError = -15, //������Ϣ����
    SqMacrographInfoError = -16,//���ͼ��Ϣ����
    SqNotExist = -17,//�����ڣ����粡����Ϣ����ͼ������������Ǵ��󣬲�Ӱ�������Ϣ��
    SqLayerIndexesError = -18,//�㼶��������
    SqSliceIndexesError = -19, //ָ��Сͼ�������� 
    SqROIRange = -20,	//ȡֵ��Χ����
    SqBlockJpeg = -21, //�Զ��彫Sdpc�п��Jpeg����
    SqExtraInfoError = -22,	//������Ϣ����
    SqTileImageHeadError = -23,	//��ϸ����Ϣͷ����
    SqTileImageConfigCheckError = -24,	//ѪҺ�����ļ�У��ʧ��
    SqTileImageConfig2JsonError = -25,	//ѪҺ�����ļ�ת��Jsonʧ��
    SqTileImageConfigNodeError = -26,	//ѪҺ�����ļ���ȡ�ڵ�ʧ��
    SqTileImageConfigHeadError = -27,	//ѪҺ�����ļ�ͷ��Ϣ����
    SqDecodeHevcError = -28,      //hevc�������
};

typedef struct SqPicHead
{
    unsigned short flag; /* ��8λΪ��ĸ��S������8λΪ��ĸ��Q ' 0x5153*/
    unsigned char version[16];
    unsigned int headSize;	/* ��ǰ�ṹ��Ĵ�С*/
    size_t fileSize;		/* �ļ���С */ //22ΪfileSize������SqPicHead�ṹ����ƫ��������Ҫ�޸�fileSize������

    unsigned int macrograph;	//���ͼ,0��ʾû��ͼ��>= 1���ʾ�ж����ź��ͼ
    unsigned int personInfor;	//������Ϣ
    unsigned int hierarchy;	/* �㼶 */
    unsigned int srcWidth;	/* ԭʼͼ���С */
    unsigned int srcHeight;
    unsigned int sliceWidth;	/* ��Ƭ��С */
    unsigned int sliceHeight;
    unsigned int thumbnailWidth;
    unsigned int thumbnailHeight;
    /* ɫ���ÿ��������ռλ��(bits per pixel)����ֵΪ��1��4��8��16��24��32 */
    unsigned char bpp;
    /* ѹ������0-100 */
    unsigned char quality;
    /* ͼ��ɫ�ʿռ� */
    J_COLOR_SPACE colrSpace;
    /* ���ű��� */
    float scale;
    double ruler;/*�����ߣ�һ�����ض�Ӧ���ĳߴ磬��λΪum*/
                 /* Ԥ���ռ� */
    unsigned int rate;/*ɨ�豶��*/
    size_t extraOffset;/*������Ϣƫ�������洢��CCM�������������Ϊ0�����ʾ������*/
    size_t tileOffset;/*��ϸ��ͼ��ƫ��������Ƕ�а�ϸ��ͼ����Ϊ0�����ʾ������*/
    ImageFormat sliceFormat; //��Ƭ��ʽ
    unsigned char headSpace[48];	//Ԥ���ռ�
}SqPicHead;

typedef struct SqMacrographInfo
{
    unsigned short flag;	/*��8λΪ��ĸ��M������8λΪ��ĸ��I '0x494D*/
                         //unsigned int macrographNum;	//������
                         //unsigned int curMacrograph;	//�ڼ��ź��ͼ
    unsigned char *rgb;
    unsigned int width;
    unsigned int height;
    unsigned int chance;
    unsigned int step;
    size_t rgbSize;
    size_t encodeSize;	//jpeg��С
    unsigned char quality;
    size_t nextLayerOffset;
    unsigned int headSpace_1;
    unsigned int headSpace_2;
    unsigned char headSpace[64];
}SqMacrographInfo;

typedef struct SqPicInfo
{
    unsigned short flag; /* ��8λΪ��ĸ��F������8λΪ��ĸ��I ' 0x4649*/
    unsigned int infoSize; /* ��ǰ�ṹ��Ĵ�С*/
                           /* �ڼ��� */
    unsigned int layer;
    /* ��Ƭ���� */
    unsigned int sliceNum;
    /* ��Ƭ������������� */
    unsigned int scliceNumX;
    unsigned int scliceNumY;
    /* ͼ���С */
    size_t layerSize;
    /* ��һͼ��ṹ��ƫ���� */
    size_t nextLayerOffset;
    /* ���ű��� */
    float curScale;
    double ruler; //΢��Ϊ��λ

    unsigned int defaultX;/*���ױߣ�X��Y���һ�ŵ�X��Y���ڶ������صİױߣ�Ϊ�˹̶���Ƭ�Ĵ�С*/
    unsigned int defaultY;
    unsigned char headSpace[64];	//Ԥ���ռ�
}SqPicInfo;

typedef struct SqPersonInfo
{
    unsigned short flag;	/*��־λ ��8λΪ��ĸ��P������8λΪ��ĸ��I ' 0x4950*/
    unsigned int inforSize;	//������Ϣ��С
    unsigned char pathologyID[64];	//�����
    unsigned char name[64];
    unsigned char sex;	//�Ա�,1λ�У�0ΪŮ
    unsigned char age;
    unsigned char departments[64];	//���� 
    unsigned char hospital[64];
    unsigned char submittedSamples[1024];	//�ͼ�������Ϣ
    unsigned char clinicalDiagnosis[2048];	//�ٴ����
    unsigned char pathologicalDiagnosis[2048];	//�������
    unsigned char reportDate[64];	//��������
    unsigned char attendingDoctor[64];	//����ҽ��
    unsigned char remark[1024];	//��ע��Ϣ
    size_t nextOffset;	//��һ��ƫ����
    unsigned int reversed_1;	/* ������1����������Ϊ0 */
    unsigned int reversed_2;	/* ������2����������Ϊ0 */
    unsigned char reversed[256];	//�����ֽ�
}SqPersonInfo;

typedef struct SqExtraInfo
{
    short flag; 	/*��־λ ��8λΪ��ĸ��E������8λΪ��ĸ��I ' 0x4945*/
    unsigned int inforSize;	//������Ϣ��С
    size_t nextOffset;	//��һ��ƫ����
                         /*CCM��ɫУ׼����*/
    unsigned char model[20];    //����ͺ�
    float ccmGamma;//gamma
    float ccmRgbRate[3]; //rgb����
    float ccmHsvRate[3];//hsv����
    float ccm[9];//3*3����

    unsigned char timeConsuming[32];//ɨ��ʱ��
    unsigned int scanTime; //ɨ��ʱ��
    unsigned short stepTime[10];   //ÿ����ʱ
    unsigned char serial[32];   //���к�
    unsigned char fusionLayer;    //�ںϲ���
    float step;  //����
    unsigned short focusPoint;    //�Խ���
    unsigned short validFocusPoint;    //��Ч�Խ���

    unsigned char barCode[128];  //������

    float cameraGamma;
    float cameraExposure;
    float cameraGain;

    unsigned char reversed[433];	//�����ֽ�
}SqExtraInfo;

typedef struct SqSliceInfo
{
    size_t *sliceOffset;/*��Ƭƫ����*/
    int *sliceSize;/*��Ƭ��С*/
}SqSliceInfo;

/********************ѪҺͼ������Ƕ�İ�ϸ��ͼ**********************/
/***************�ɸ���sdpc��tileoffset���ж��Ƿ����***************/
typedef struct SqTileImageHead
{
    short flag; //��־λ 0x4954
    unsigned int infoSize; //�ṹ���С
    size_t curOffset;    //��ǰƫ����
    size_t nextOffset;   //�¸�����ƫ����
    int width;   //ͼ����
    int height;  //ͼ��߶�
    int imgCount;    //ͼ����Ŀ
    unsigned char quality;    //ѹ������
    J_COLOR_SPACE colrSpace; //ͼ��ɫ�ʿռ� 
    char guid[36];
    unsigned char headSpace[(1 << 20) - 36]; //Ԥ���ռ�
}SqTileImageHead;

typedef struct SqTileImageInfo
{
    SqTileImageHead *tileImageHead;
    SqSliceInfo *tileImageOffset;
}SqTileImageInfo;
/*************************************************************/

/****************������ѪҺ�����ļ���Ϣ��sdpl��***************/
typedef struct SqTileImageHeadConfig
{
    unsigned short flag;//��8λΪ��ĸ��T������8λΪ��ĸ��H ' 0x4854
    unsigned int headSize;//�ṹ���С
    size_t configSize;//�����ļ���С
    char lnkName[128];  //�����ļ���
    int tileImageCount;  //ͼ�����
    int width;
    int height;
    unsigned char type;   //��������
    size_t nextOffset;   //ƫ����
    char guid[36];
    unsigned char headSpace[(1 << 20) - 36]; //Ԥ���ռ�
}SqTileImageHeadConfig;

/*���Ű�ϸ��ͼ�����Ϣ*/
typedef struct SqTileImageInfoConfig
{
    unsigned short flag;//��8λΪ��ĸ��T������8λΪ��ĸ��I ' 0x4954
    unsigned int id;
    unsigned int infoSize;
    // short type;
    unsigned int roiCount;
    size_t roiAddr; //Ԥ���ռ�
    size_t curOffset;    //��ǰƫ����
    size_t nextOffset;   //�¸�����ƫ����
    char note[512];  //��ע
    char headSpace[2048]; //Ԥ���ռ�
}SqTileImageInfoConfig;

/*ͼ����roi��λ���Լ���Ϣ*/
typedef struct SqTileImageRoiConfig
{
    unsigned short type; //���
    CRectangle rect;
    char text[512];  //��ע
    unsigned char headSpace[128]; //Ԥ���ռ�
}SqTileImageRoiConfig;

typedef struct TileImageRoiInfo
{
    SqTileImageInfoConfig *TileImageInfo;/*��ϸ��ͼ����Ϣ*/
    SqTileImageRoiConfig **TileImageRoi;/*ROI��Ϣ*/
}TileImageRoiInfo;

/*�����ļ�Ԥ����ĸ��ڵ�*/
/*һ����Ŀ¼����һ��ͷ��Ϣ�Լ�ͼ����Ϣ*/
/*һ�Ű�ϸ��ͼ�����N��ROI*/
typedef struct ClassifyRoot
{
    SqTileImageHeadConfig *TileImageHeadConfig;/*ͷ��Ϣ*/
    TileImageRoiInfo **ClassifyImageRoiInfo;/*ͼ��roi��Ϣ*/
}ClassifyRoot;
/*************************************************************/

/*���л�ȡ��Ϣ�Ľṹ��*/
typedef struct SqSdpcInfo
{
    char *fileName;

    SqPicHead *picHead;/*�ļ���ͷ��Ϣ*/
    SqPersonInfo *personInfo;/*������Ϣ*/
    SqExtraInfo *extra;//������Ϣ��������ɫУ����
    SqImageInfo **macrograph;/*���ͼ*/
    SqImageInfo *thumbnail;/*����ͼ*/
    SqPicInfo **sliceLayerInfo;//��Ƭ�㼶��Ϣ
    SqSliceInfo **sliceInfo;//��Ƭλ���Լ���С��Ϣ
    SqColorTable *colorTable;
#ifdef BLOOD_IMAGE
    SqTileImageInfo *tileImageInfo;//ѪҺ��ϸ����Ϣ������һϵ�е��Ű�ϸ��ͼ��
    ClassifyRoot *classifyRoot;	//ѪҺ��ϸ�������ļ���Ϣ
#endif
}SqSdpcInfo;
/************************************************************************/
/*                       ���º�������׼Cһ��                            */
/************************************************************************/
DECLSPEC void SqFreeMemory(void *buf);

DECLSPEC void *SqMemset(void *dst, int val, size_t size);

DECLSPEC FILE *SqOpenFile(char *path, char *mode);

DECLSPEC void SqCloseFile(FILE *file);

DECLSPEC inline size_t SqWriteFileData(FILE *file, void *data, size_t size);

/*��ȡ�򿪵��ļ���С*/
DECLSPEC size_t SqGetFileSize(FILE *file);
/*��ȡָ��·�����ļ���С*/
DECLSPEC size_t SqGetFileSizeSystemCall(char *path);

DECLSPEC inline unsigned char *SqReadFileData(FILE *file);

DECLSPEC inline size_t SqReadData(void *data, size_t elementSize, size_t elementCount, FILE *file);

DECLSPEC inline size_t SqWriteData(void *data, size_t elementSize, size_t elementCount, FILE *file);

DECLSPEC void SqRewind(FILE *file);

DECLSPEC int SqFseeki64(FILE *file, size_t offset, int origin);

DECLSPEC size_t SqFtelli64(FILE *file);
/************************************************************************/
/*                       ���Ϻ�������׼Cһ��                            */
/************************************************************************/

/************************************************************************
*���ܣ���rgb888תJpeg
*src:rgb����
*dst:ת����jpeg����
*quality:ѹ������
*colrSpace:ͼ�����ݵ�ɫ�ʿռ�
*return�����󷵻�С��0��������Ϣ��SqErrorö�٣�,��ȷ����ѹ����Jpeg�Ĵ�С
************************************************************************/
DECLSPEC GLOBAL(unsigned long)
SqEncodeJpeg(unsigned char *src, unsigned char **dst, int quality, int width, int height, J_COLOR_SPACE colrSpace);

/************************************************************************
*���ܣ���jpegתrgb888
*src:ת����jpeg����
*srcSize:ת����jpeg���ݵĴ�С
*buffer:��ѹ��rgb������
*colrSpace:ͼ�����ݵ�ɫ�ʿռ�
*return�����󷵻�С��0��������Ϣ��SqErrorö�٣�,��ȷ���ؽ�ѹ��rgb�Ĵ�С
************************************************************************/
DECLSPEC GLOBAL(size_t)
SqDecodeJpeg(unsigned char *src, unsigned long srcSize, unsigned char **buffer, J_COLOR_SPACE colrSpace);

/************************************************************************/
/*���ܣ�����CCMУ׼��rgb��ɫӳ���
/*rgbRate������rgbRate����
/*hsvRate������hsvRate����
/*gamma������gammaֵ
/*redTable:�����redӳ����ⲿ����ռ䣩red[256][256][256]
/*greenTable:�����greenӳ����ⲿ����ռ䣩green[256][256][256]
/*blueTable:�����blueӳ����ⲿ����ռ䣩blue[256][256][256]
/************************************************************************/
DECLSPEC SqColorTable *InitColorCollectTable(float *rgbRate, float *hsvRate, float gamma, float *ccm);

/************************************************************************/
/*���ܣ�Ϊrgbӳ�������ռ�
/*return:��ʼ������ɫͨ��
/************************************************************************/
DECLSPEC void DisposeColorCorrectTable(SqColorTable *ct);

/************************************************************************/
/*���ܣ����CCMУ׼��rgb��ɫ
/*srcRgb������ԭʼrgb����
/*dstRgb������У׼���rgb����
/*width��ͼ����
/*height��ͼ��߶�
/*redTable:����redӳ���
/*greenTable:����greenӳ���
/*blueTable:����blueӳ���
/*return:���󷵻�false����ȷ����true
/************************************************************************/
DECLSPEC boolean RgbColorCorrect(unsigned char *srcRgb, unsigned char *dstRgb,
    int width, int height, SqColorTable *colorTable);

DECLSPEC GLOBAL(int) SqOpenSdpc(char *sdpcPath, SqSdpcInfo **sdpcInfo);

/************************************************************************
*���ܣ���ȡָ���㼶��ָ��λ�õ�СJpegͼ
*picInfo:ָ���㼶��Ϣ
*jpegSlice��ת�����СJpegͼ����
*size��СJpegͼ��С
*layer��ָ���Ĳ㼶��Ĭ�ϴ�0��ʼ��
*x��y��ָ��ͼƬ��СJpegͼ��λ��
*return�����󷵻�С��0��������Ϣ��SqErrorö�٣�����ȷ����0
************************************************************************/
DECLSPEC GLOBAL(int)
SqGetJpegSliceOfSpecifyLayer(SqSdpcInfo *si, unsigned char **jpegSlice, unsigned int *size, int layer, int x, int y);

DECLSPEC GLOBAL(int)
SqGetHevcSliceOfSpecifyLayer(SqSdpcInfo *si, unsigned char **hevcSlice, unsigned int *size, int layer, int x, int y);
/*************************���ã����ڴ棩*********************************
*���ܣ���ָ��������С��ƬתΪһ�ų����RGBͼƬ��width_height_layer.rgb��
*rgb��ת�����rgb����
*rgbWidth��ת�����rgb���
*rgbHeight��ת�����rgb�߶�
*layer��ָ���Ĳ㼶��Ĭ�ϴ�0��ʼ��
*return�����󷵻�С��0��������Ϣ��SqErrorö�٣�����ȷ����0
************************************************************************/
DECLSPEC GLOBAL(int)
SqSpecifyLayerJpeg2Rgb(SqSdpcInfo *si, unsigned char **rgb, int *rgbWidth, int *rgbHeight, int layer, bool isCorrect = false);

/************************************************************************
*���ܣ���ָ������ָ������Ϊ����ȡ��Ӧ��RGB888��width_height_layer.rgb��
*rgb��ת�����rgb����
*rgbWidth����Ҫ��ȡrgb���
*rgbHeight����Ҫ��ȡrgb�߶�
*xPixel��yPixel���������
*layer��ָ���Ĳ㼶��Ĭ�ϴ�0��ʼ��
*return�����󷵻�С��0��������Ϣ��SqErrorö�٣�����ȷ����0
************************************************************************/
DECLSPEC GLOBAL(int)
SqGetRoiRgbOfSpecifyLayer(SqSdpcInfo *si, unsigned char **rgb, int rgbWidth, int rgbHeight,
    unsigned int xPixel, unsigned int yPixel, int layer, bool isCorrect = false);

/************************************************************************
*���ܣ���sdpc����ΪxSlice * ySlice��Jpeg
*blockJpeg��Ҫ��ȡxSlice * ySlice��Jpeg
*quality:ѹ������
*xSlice:x����Ŀ���
*ySlice:y����Ŀ���
*colrSpace:ͼ�����ݵ�ɫ�ʿռ�
*return�����󷵻�С��0��������Ϣ��SqErrorö�٣�
************************************************************************/
DECLSPEC GLOBAL(int)
SqGetBlockJpegFromSdpc(SqSdpcInfo *si, SqImageInfo *blockJpeg, int quality, int xSlice, int ySlice,
    int layer, J_COLOR_SPACE colrSpace, bool isCorrect = false);

DECLSPEC GLOBAL(void) SqCloseSdpc(SqSdpcInfo *si);

#ifdef BLOOD_IMAGE
/*���ѪҺͼ�񣬲�����sdpl*/
/************************************************************************
*���ܣ���ȡѪҺͼ�������ļ��е���Ϣ
*sdpcFileName��������sdpc�ļ��������ͨ��sdpc�ļ���ȥ�ҵ�������sdpl�ļ�
*sdpcTileImageHeadGuid:sdpcѪҺͼ���е�tileImageHead�е�guid�����ڼ�������ļ��Ƿ�һ��
*classify:�����ļ��з�����Ϣ
*return�����󷵻�С��0��������Ϣ��SqErrorö�٣�
************************************************************************/
DECLSPEC GLOBAL(int) GetSdpcClassifyConfig(char *sdpcFileName, char *sdpcTileImageHeadGuid, ClassifyRoot **classify);
/*�ͷ�ѪҺͼ�������ļ�������Ϣ*/
DECLSPEC GLOBAL(void) DisposeClassifyRoot(ClassifyRoot *root);

/*��ȡsdpcָ���İ�ϸ��ͼ��,index�ڼ���ͼ�񣬴�0��ʼ*/
DECLSPEC GLOBAL(int) GetSingleTileImageFromBloodSdpc(SqSdpcInfo *si, unsigned char **jpeg, unsigned int *jpegSize, int index);
#endif