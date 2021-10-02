#include "clusterManagement.h"

int8 findAvailableCluster(FILE *file, int8 indexSize, int8 clusterSize)
{
    fseek(file, 4, SEEK_SET);
    int8 current;
    int8 full = 255;
    int8 eof = 28;
    for (int i = 0; i < pow(2, indexSize); i++)
    {
        fread(&current, sizeof(int8), 1, file);
        if (current == 0)
        {
            int offSet = 4 + pow(2, indexSize) + i * pow(2, clusterSize);
            fseek(file, -1, SEEK_CUR);
            fwrite(&full, sizeof(int8), 1, file);
            fseek(file, offSet, SEEK_SET);
            fwrite(&eof, sizeof(int8), 1, file);
            return i;
        }
    }
    return -1;
}