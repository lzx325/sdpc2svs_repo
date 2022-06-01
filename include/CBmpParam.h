#pragma once

#include "baseParam.h"
/*#ifdef WINDOWS
//#include <windows.h>
#include "baseParam.h"
#elif LINUX
#include "baseParam.h"
#endif*/

#pragma pack(1)
typedef struct {
    UINT16 flag;		/* ��8λΪ��ĸ��B������8λΪ��ĸ��M */
    UINT32 file_size;		/* �ļ���С */
    UINT16 reversed_1;	/* ������1����������Ϊ0 */
    UINT16 reversed_2;	/* ������2����������Ϊ0 */
    UINT32 data_offset;	/* ͼ���������ļ���λ�ã���λ���ֽڣ�ƫ������ */
}BmpHead;

typedef struct {
    /* ��ǰ�ṹ��Ĵ�С��ͨ��54�ֽ� */
    UINT32 info_size;
    /* λͼ��ȣ���λ������ */
    UINT32 bmp_width;
    /* λͼ�߶ȣ���λ������ */
    UINT32 bmp_height;
    /* ɫ��ƽ������ֻ�� 1 Ϊ��Чֵ */
    UINT16 planes;
    /* ɫ���ÿ��������ռλ��(bits per pixel)����ֵΪ��1��4��8��16��24��32 */
    UINT16 bpp;
    /* ��ʹ�õ�ѹ��������һ����ֵΪ0������ѹ�� */
    UINT32 compression;
    /* ͼ���С��ָԭʼλͼ���ݵĴ�С��ǧ�����Ϊ���ļ���С */
    UINT32 bmp_size;
    /* ͼ��ĺ���ֱ��ʣ���λΪ����ÿ�ף�pixels-per-meter�� */
    UINT32 h_ppm;
    /* ͼ�������ֱ��ʣ���λΪ����ÿ�� */
    UINT32 v_ppm;
    /* ��ɫ�����ɫ������ֵΪ0ʱ��ʾ��ɫ��Ϊ2^ɫ��� */
    UINT32 colors;
    /* ��Ҫ��ɫ������ֵ������ɫ��ʱ�����ߵ���0ʱ������ʾ������ɫ��һ����Ҫ��ͨ������Ϊ0 */
    UINT32 i_colors;
}BmpInfo;