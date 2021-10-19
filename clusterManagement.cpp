#include "clusterManagement.h"

int offSetCalc(int8 indexSize, int8 clusterSize, int8 curDir)
{
    return (4 + pow(2, indexSize)-1 + curDir * pow(2, clusterSize));
}

int findAvailableCluster(FILE *file, int8 indexSize, int8 clusterSize)
{
    fseek(file, 4, SEEK_SET);
    int8 current;
    int8 full = 255;
    int8 eof = 28;
    for (int i = 0; i < pow(2, indexSize)-1; i++)
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
int findNewCluster(FILE *file, int8 indexSize,int8 currentIndex)
{
    fseek(file, 4, SEEK_SET);
    int8 current;
    int8 full = 255;
    for (int i = 0; i < pow(2, indexSize)-1; i++)
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
int availableClusters(FILE *file, int8 indexSize){
    fseek(file, 4, SEEK_SET);
    int8 current;
    int emptyClusters=0;
    for (int i = 0; i < pow(2, indexSize)-1; i++)
    {
        fread(&current, sizeof(int8), 1, file);
        if (current == 0)
        {
            emptyClusters++;
        }
    }
    return emptyClusters;
}

int clusterInFile(FILE *file, int8 startCluster){
        int currentIndex=(4+startCluster);
        int fileClusters=0;
        int8 current;
        do{
            fseek(file, currentIndex, SEEK_SET);
            fread(&current,1,1,file);
            fileClusters++;
            currentIndex =4+current;
        }while(current!=255);
        return fileClusters;

}
