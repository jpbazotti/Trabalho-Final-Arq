#include "clusterManagement.h"

int offSetCalc(int8 indexSize, int8 clusterSize, int8 curDir)
{
    return (4 + pow(2, indexSize) + curDir * pow(2, clusterSize));
}

int8 findAvailableCluster(FILE *file, int8 indexSize, int8 clusterSize)
{
    fseek(file, 4, SEEK_SET);
    int8 current;
    int8 full = 255;
    int8 eof = 28;
    for (int8 i = 0; i < pow(2, indexSize); i++)
    {
        fread(&current, sizeof(int8), 1, file);
        if (current == 0)
        {
            int offSet = offSetCalc(indexSize, clusterSize, i);
            fseek(file, -1, SEEK_CUR);
            fwrite(&full, sizeof(int8), 1, file);
            fseek(file, offSet, SEEK_SET);
            fwrite(&eof, sizeof(int8), 1, file);
            return i;
        }
    }
    return -1;
}
int8 findNewCluster(FILE *file, int8 indexSize, int8 clusterSize,int8 currentIndex)
{
    fseek(file, 4, SEEK_SET);
    int8 current;
    int8 full = 255;
    for (int8 i = 0; i < pow(2, indexSize); i++)
    {
        fread(&current, sizeof(int8), 1, file);
        if (current == 0)
        {
            fseek(file, -1, SEEK_CUR);
            fwrite(&full, sizeof(int8), 1, file);
            fseek(file,currentIndex + 4, SEEK_SET);
            fwrite(&i,sizeof(int8),1,file);
            return i;
        }
    }
    return -1;
}
