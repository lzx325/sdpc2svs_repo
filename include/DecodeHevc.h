#pragma once
#ifdef WINDOWS
#define DECLSPEC extern "C" _declspec(dllexport)
#elif LINUX
#define DECLSPEC extern "C"
#endif

typedef enum _ColorSpace
{
    RGB24 = 0x02,
    BGR24 = 0x03,
    ARGB = 0x19,
    RGBA = 0x1a,
    ABGR = 0x1b,
    BGRA = 0x1c,
}ColorSpace;

DECLSPEC
int DecodeHevc(unsigned char *hevcStream, int width, int height, int hevcSize, unsigned char *dst, ColorSpace colorSpace);