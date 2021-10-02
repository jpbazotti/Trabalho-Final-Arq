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

//entra na pasta com o nome que foi passado, se ela existir no diretorio atual
bool gotoDir(char *name, FILE *dir, FileSystem fs);

//faz o comando cd
bool CD(char *names, FILE *dir, FileSystem fs);

//retorna o nome do arquivo
void getFileName(dirEntry Entry);

//faz o commando DIR em um cluster somente
void DIR(FILE *dir);

//faz o comando RM
bool remove(char *name);

void createDir(FILE *file, int8 curDir, char dirName[9], int8 clusterSize, int8 indexSize);
void createFile(FILE *file, int8 curDir, char fileName[9], char extensionName[4], int8 clusterSize, int8 indexSize);

#endif