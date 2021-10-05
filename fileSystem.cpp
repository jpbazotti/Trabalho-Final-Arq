#include <string.h>
#include "fileSystem.h"
#include "fileSystemCreator.h"

char *getFileName(dirEntry *Entry)
{
    //pastas sao arquivos, porem a sua extensao nao aparece

    if (strcmp(Entry->extension, "dir") == 0)
    {
        return Entry->name;
    }
    else
    {
        return strcat(strcat(Entry->name, "."), Entry->extension);
    }
}

bool gotoDir(char *name, FILE *dir, FileSystem fs)
{
    int8 index;
    while (1)
    {
        dirEntry Entry;
        fread(&Entry, sizeof(dirEntry), 1, dir);
        if (Entry.name[0] == 28)
        {
            return false;
        }
        char* aux = getFileName(&Entry);
        if (strcmp(aux, name) == 0)
        {
            if (strcmp(Entry.extension, "dir") == 0)
            {
                index = Entry.startCluster;
                fseek(dir, offSetCalc(fs.indexSize, fs.clusterSize, index), SEEK_SET);
                return true;
            }
        }
    }
    
}

bool CD(char *names, FILE *dir, FileSystem fs, int currentClusterIndex)
{
    //get current offset
    int offset = offSetCalc(fs.indexSize, fs.clusterSize, currentClusterIndex);
    //Verifica se o path contem "root"
    char *name;
    name = strtok(names, "/");
    if(strcmp(name, "root") != 0){
        return false;
    }
    //reseta o ponteiro do FS
    int startOffset = offSetCalc(fs.indexSize, fs.clusterSize, fs.rootStart);
    fseek(dir, startOffset, SEEK_SET);
    //Encontra a pasta a partir do path
    while (1)
    {
        name = strtok(NULL, "/");
        if(name == NULL){
            return false;
            break;
        }
        if (!gotoDir(name, dir, fs))
        {
            cout << "Arquivo ou pasta nao encontrado.\n";
            fseek(dir, offset, SEEK_SET);
            return false;
            break;
        }
    }
    return true;
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
        cout << getFileName(&Entry) << "\n";
    }
}

//bool remove(char* path){}

//cria novo arquivo em um diretorio, no diretorio procura por um espaco vazio ou o final do diretorio se nao ha um disponivel
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
    } while ((current != 28)||(current != 29));
    fseek(file, -1, SEEK_CUR);
    fwrite(&newEntry, sizeof(dirEntry), 1, file);
    fwrite(&eof, sizeof(int8), 1, file);
}

//cria novo arquivo em um diretorio, no diretorio procura por um espaco vazio ou o final do diretorio se nao ha um disponivel
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
    } while ((current != 28)||(current != 29));
    fseek(file, -1, SEEK_CUR);
    fwrite(&newEntry, sizeof(dirEntry), 1, file);
    fwrite(&eof, sizeof(int8), 1, file);
}