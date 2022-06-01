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
    UINT16 flag;		/* 低8位为字母’B’，高8位为字母’M */
    UINT32 file_size;		/* 文件大小 */
    UINT16 reversed_1;	/* 保留字1，必须设置为0 */
    UINT16 reversed_2;	/* 保留字2，必须设置为0 */
    UINT32 data_offset;	/* 图象数据在文件的位置，单位：字节（偏移量） */
}BmpHead;

typedef struct {
    /* 当前结构体的大小，通常54字节 */
    UINT32 info_size;
    /* 位图宽度，单位：像素 */
    UINT32 bmp_width;
    /* 位图高度，单位：像素 */
    UINT32 bmp_height;
    /* 色彩平面数，只有 1 为有效值 */
    UINT16 planes;
    /* 色深，即每个像素所占位数(bits per pixel)典型值为：1、4、8、16、24、32 */
    UINT16 bpp;
    /* 所使用的压缩方法，一般其值为0，即不压缩 */
    UINT32 compression;
    /* 图像大小。指原始位图数据的大小，千万别以为是文件大小 */
    UINT32 bmp_size;
    /* 图像的横向分辨率，单位为像素每米（pixels-per-meter） */
    UINT32 h_ppm;
    /* 图像的纵向分辨率，单位为像素每米 */
    UINT32 v_ppm;
    /* 调色板的颜色数，其值为0时表示颜色数为2^色深个 */
    UINT32 colors;
    /* 重要颜色数，其值等于颜色数时（或者等于0时），表示所有颜色都一样重要；通常设置为0 */
    UINT32 i_colors;
}BmpInfo;