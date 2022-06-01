// sdpc2svs.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CSvsParam.h"

int Strnicmp(const char *s1, const char *s2, size_t n)
{
#ifdef WINDOWS
    return _strnicmp(s1, s2, n);
#elif LINUX
    return strncasecmp(s1, s2, n);
#endif
}

int main(int argc, char *argv[])
{
    char *srcPath;
    char *dstPath;
    SvsConfig config;

    if (argc < 3)
    {
        printf("�������㣡\n");
        return -1;
    }
    switch (argc)
    {
    case 8:
        config.TileWidth = atoi(argv[6]);
        config.TileHeight = atoi(argv[7]);
    case 7:
    case 6:
        if (Strnicmp((const char *)argv[5], "true", 4) == 0)
            config.IsCustomTileSize = true;
        else if (Strnicmp((const char *)argv[5], "false", 5) == 0)
            config.IsCustomTileSize = false;
        else
            printf("����sdpcĬ�ϵ���Ƭ��С�洢��\n");
    case 5:
        if (Strnicmp((const char *)argv[4], "true", 4) == 0)
            config.IsColorCorrect = true;
        else if (Strnicmp((const char *)argv[4], "false", 5) == 0)
            config.IsColorCorrect = false;
        else
            printf("��ɫУ�����������ɫУ��:��(Ĭ��ֵ)\n");
    case 4:
        config.Quality = atoi(argv[3]);
    case 3:
        srcPath = argv[1];
        dstPath = argv[2];
    }
    Sdpc2Svs(srcPath, dstPath, &config);

#if false
    bool ret = true;
    char *srcPath = "../img/3_0.sdpc";
    char *dstPath = "../img/3_0_1.svs";
    SvsConfig config;
    config.Quality = 60;
    //DWORD start_time = GetTickCount();
    CSvs *csvs = InitSvs(srcPath, dstPath, &config);
    if (csvs == NULL)
    {
        printf("����sdpcͼ��ʧ�ܣ�\n");
        return false;
    }
    printf("����sdpcͼ��ɹ���׼��ת��svs��ʽ�������������ռ��ʱ��϶࣡�����ĵȴ���\n");
    int fieldCount = csvs->si->picHead->hierarchy + 3;
    Field *field = (Field *)malloc(sizeof(Field) * fieldCount);
    /*first layer image*/
    ret = SetSvsLayerImage(csvs, &field[0], PAGENUMBER(0, fieldCount), 0);
    if (ret == NULL)
    {
        printf("д���1������ʧ�ܣ�\n");
        return -1;
    }
    printf("д���1�����ݳɹ���\n");
    /*thumbnail*/
    ret = SetSvsStripImage(csvs, &field[1], E_THUMBNAIL);
    if (ret == NULL)
    {
        printf("д���2������ʧ�ܣ�\n");
        return -1;
    }
    printf("д���2�����ݳɹ���\n");
    /*other layer images*/
    for (int i = 1; i < csvs->si->picHead->hierarchy; i++)
    {
        ret = SetSvsLayerImage(csvs, &field[i + 1], PAGENUMBER(i + 1, fieldCount), i);
        if (ret == NULL)
        {
            printf("д���%d������ʧ�ܣ�\n", i + 2);
            goto Error;
        }
        printf("д���%d�����ݳɹ���\n", i + 2);
    }
    /*label*/
    ret = SetSvsStripImage(csvs, &field[csvs->si->picHead->hierarchy + 1], E_LABEL);
    if (ret == NULL)
    {
        printf("д���%d������ʧ�ܣ�\n", csvs->si->picHead->hierarchy + 2);
        goto Error;
    }
    printf("д���%d�����ݳɹ���\n", csvs->si->picHead->hierarchy + 2);
    /*macrograph*/
    ret = SetSvsStripImage(csvs, &field[csvs->si->picHead->hierarchy + 2], E_MACROGRAPH);
    if (ret == NULL)
    {
        printf("д���%d������ʧ�ܣ�\n", csvs->si->picHead->hierarchy + 3);
        goto Error;
    }
    printf("д���%d�����ݳɹ���\n", csvs->si->picHead->hierarchy + 3);
    /*overwrite image file information*/
    ret = OverwriteSvsInfo(csvs, field, fieldCount);
    if (ret == NULL)
    {
        printf("��д��Ϣʧ�ܣ�\n");
        goto Error;
    }
    printf("��д��Ϣ�ɹ���\n");
    DisposeSvs(csvs);
    free(field);
    field = NULL;
    return 0;
Error:
    if (field != NULL)
    {
        free(field);
        field = NULL;
    }
    DisposeSvs(csvs);
    //system("pause");
    return -1;
#endif
    return 0;
}

