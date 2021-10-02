#include <string.h>
#include "fileSystem.h"
#include "fileSystemCreator.h"

bool gotoDir(char *name, FILE *dir, FileSystem fs)
{
    bool exists = false;
    while (1)
    {
        dirEntry Entry;
        fread(&Entry, sizeof(dirEntry), 1, dir);
        if (Entry.name[0] == 28)
        {
            break;
        }
        if (strcmp(getFileName(&Entry), name))
        {
            if (strcmp(Entry.extension, "dir") == 0)
            {
                exists = true;
            }
            break;
        }
        int index = Entry.startCluster;
    }
    //fseek
}

bool CD(char *names, FILE *dir, FileSystem fs)
{
    //reseta o ponteiro do FS
    int startOffset = offSetCalc(fs.indexSize, fs.clusterSize, fs.rootStart);
    fseek(dir, startOffset, SEEK_SET);
    //Encontra a pasta a partir do path
    char *name;
    name = strtok(names, "/");
    while (name != NULL)
    {
        name = strtok(NULL, "/");
        if (!gotoDir(name, dir, fs))
        {
            return false;
        }
    }
    return true;
}

char *getFileName(dirEntry *Entry)
{
    //pastas sao arquivos, porem a sua extensao nao aparece

    if (strcmp(Entry->extension, "dir") == 0)
    {
        return strcat(Entry->name, "\n");
    }
    else
    {
        return strcat(strcat(Entry->name, "."), strcat(Entry->extension, "\n"));
    }
}

void DIR(FILE *dir)
{
    while (1)
    {
        dirEntry Entry;
        fread(&Entry, sizeof(dirEntry), 1, dir);
        if (Entry.name[0] == 28)
        {
            break;
        }
        cout << getFileName(&Entry);
    }
}

//bool remove(char* path){}

void createDir(FILE *file, int8 curDir, char dirName[9], int8 clusterSize, int8 indexSize)
{
    int8 eof = 28;
    int dirOffset = offSetCalc(indexSize, clusterSize, curDir);
    dirEntry newEntry;
    strcpy(newEntry.name, dirName);
    strcpy(newEntry.extension, "dir");
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
void createFile(FILE *file, int8 curDir, char fileName[9], char extensionName[4], int8 clusterSize, int8 indexSize)
{
    int8 eof = 28;
    int dirOffset = offSetCalc(indexSize, clusterSize, curDir);
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