#include "file.h"

void createFile(FILE *file, int8 curDir, char fileName[9], char extensionName[4], int8 clusterSize, int8 indexSize)
{
        int8 eof = 28;
        int dirOffset = offSetCalc(indexSize, clusterSize,curDir);
        dirEntry newEntry;
        strcpy(newEntry.name, fileName);
        strcpy(newEntry.extension, "txt");
        newEntry.startCluster = findAvailableCluster(file, indexSize, clusterSize);
        fseek(file, dirOffset, SEEK_SET);
        int8 current;
        do
        {
            fread(&current, sizeof(int8), 1, file);
        } while (current != 28);
        fseek(file, -1, SEEK_CUR);
        fwrite(&newEntry, sizeof(dirEntry), 1, file);
        fwrite(&eof, sizeof(int8), 1, file);
    }