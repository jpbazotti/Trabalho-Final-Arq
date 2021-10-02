#include "file.h"

void createFile(FILE *file, int8 curDir, char fileName[9], char extensionName[4], int8 clusterSize, int8 indexSize)
{

    char validExt[4] = "txt";
    if (strcmp(extensionName, validExt) == 0)
    {
        int8 eof = 28;
        int dirOffset = 4 + pow(2, indexSize) + curDir * pow(2, clusterSize);
        dirEntry newEntry;
        strcpy(newEntry.name, fileName);
        strcpy(newEntry.extension, validExt);
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
}