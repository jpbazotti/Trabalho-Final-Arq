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
    if(offset < offSetCalc(fs.indexSize,fs.clusterSize,fs.indexSize+1)){
        fseek(file, offset, SEEK_SET);
        return true;
    }else{
    return false;
    }
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

bool CD(char *path, FILE *file, FileSystem fs, int8 *clusterIndex)
{
    //Verifica se o path contem "root"
    char *name;
    name = strtok(path, "/");
    if(strcmp(name, "root") != 0){
        cout << "Arquivo ou pasta nao encontrado.\n";
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

bool validPath(char* path){
    char *name;
    name = strtok(path, "/");
    if(strcmp(name, "root") != 0){
        return false;
    }
    for (int i = 0; path[i] != '\0'; i++)
    {
        if(i>0){
            if(path[i] == '/' && path[i-1] == '/'){
                return false;
                break;
            }
        }
        if(path[i] == '\0' && path[i-1] == '/'){
            return false;
            break;
        }
    }
    return true;
}

char* breakePath(char* path){
    char* name;
    int j = 0;
    for (int i = 0; path[i] != '\0'; i++)
    {
        if(path[i] == '/'){
            j = i;
        }
    }
    name = &(path[j+1]);
    path[j] = '\0';
    return name;
}

bool RM(char* path, FILE *file, FileSystem fs, int8 *clusterIndex){
    //Quebra a string para encontrar o nome do arquivo
    char* name = breakePath(path);

    int8 index = *clusterIndex;
    if(CD(path, file, fs, &index)){
        gotoCluster(file, index, fs);
        DIR(file);
        gotoCluster(file, index, fs);
        //remove from dir
        //remove from index
    }else{
        return false;
    }
    return true;
}

void createDir(FILE *file, int8 curDir, const char dirName[9], int8 clusterSize, int8 indexSize)
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
    } while ((current != 28)&&(current != 29));
    fseek(file, -1, SEEK_CUR);
    fwrite(&newEntry, sizeof(dirEntry), 1, file);
    if(current == 28){
    fwrite(&eof, sizeof(int8), 1, file);
    }
}

void createFile(FILE *file, int8 curDir, const char fileName[9],const  char extensionName[4], int8 clusterSize, int8 indexSize)
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
    } while ((current != 28)&&(current != 29));
    fseek(file, -1, SEEK_CUR);
    fwrite(&newEntry, sizeof(dirEntry), 1, file);
    if(current == 28){
    fwrite(&eof, sizeof(int8), 1, file);
    }
}

void rename(){

}
