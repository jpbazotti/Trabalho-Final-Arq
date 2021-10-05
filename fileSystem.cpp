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

bool gotoCluster(FILE *file, int8 clusterIndex, FileSystem fs){
    int offset = offSetCalc(fs.indexSize, fs.clusterSize, clusterIndex);
    //if(offset <= maxvalue){
        fseek(file, offset, SEEK_SET);
        return true;
    //}else{
    return false;
    //}
}

bool gotoDir(char *name, FILE *file, FileSystem fs, int8 *clusterIndex)
{
    while (1)
    {
        dirEntry Entry;
        fread(&Entry, sizeof(dirEntry), 1, file);
        if (Entry.name[0] == 28)
        {
            return false;
        }
        char* aux = getFileName(&Entry);
        if (strcmp(aux, name) == 0)
        {
            if (strcmp(Entry.extension, "dir") == 0)
            {
                *clusterIndex = Entry.startCluster;
                //fseek is now on CD function
                //fseek(dir, offSetCalc(fs.indexSize, fs.clusterSize, *clusterIndex), SEEK_SET);
                return true;
            }
        }
    }
    
}

bool CD(char *names, FILE *file, FileSystem fs, int8 *clusterIndex)
{
    //Verifica se o path contem "root"
    char *name;
    name = strtok(names, "/");
    if(strcmp(name, "root") != 0){
        return false;
    }
    //reseta o ponteiro para o diretorio root (necessario para o funcionamento da funcao)
    int8 index = fs.rootStart;
    int offset = offSetCalc(fs.indexSize, fs.clusterSize, index);
    fseek(file, offset, SEEK_SET);
    //Encontra a pasta a partir do path
    while (1)
    {
        name = strtok(NULL, "/");
        if(name == NULL){
            *clusterIndex = index;
            return true;
            break;
        }
        if (!gotoDir(name, file, fs, &index))
        {
            cout << "Arquivo ou pasta nao encontrado.\n";
            return false;
            break;
        }
        //fseek movido para função separada
        //offset = offSetCalc(fs.indexSize, fs.clusterSize, index);
        //fseek(dir, offset, SEEK_SET);
    }
}

void DIR(FILE *file)
{
    dirEntry Entry;
    fread(&Entry, sizeof(dirEntry), 1, file);
    if (Entry.name[0] == 28)
    {
        cout << "<vazio>\n";
    }else{
        cout << getFileName(&Entry) << "\n";
        while (1)
        {
            fread(&Entry, sizeof(dirEntry), 1, file);
            if (Entry.name[0] == 28)
            {
                break;
            }
            cout << getFileName(&Entry) << "\n";
        }
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