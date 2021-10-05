#ifndef fileSystem_h
#define fileSystem_h
#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include "clusterManagement.h"
#include "fileSystemCreator.h"
using namespace std;
typedef struct
{
    char name[9];
    char extension[4];
    int8 startCluster;
} dirEntry;

//Vai para o cluster de numero clusterIndex
bool gotoCluster(FILE *file, int8 clusterIndex, FileSystem fs);

//Entra na pasta com o nome que foi passado, se ela existir no diretorio atual. Retorna o cluster atual no ponteiro clusterIndex
bool gotoDir(char *name, FILE *dir, FileSystem fs, int8 *clusterIndex);

//Faz o comando cd e retorna o cluster atual no ponteiro clusterIndex
bool CD(char *names, FILE *dir, FileSystem fs, int8 *clusterIndex);

//retorna o nome do arquivo
void getFileName(dirEntry Entry);

//faz o commando DIR em um cluster somente
void DIR(FILE *dir);

//faz o comando RM
bool remove(char *name);

void createDir(FILE *file, int8 curDir, char dirName[9], int8 clusterSize, int8 indexSize);
void createFile(FILE *file, int8 curDir, char fileName[9], char extensionName[4], int8 clusterSize, int8 indexSize);

#endif