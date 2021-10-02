#ifndef fileSystem_h
#define fileSystem_h
#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include "clusterManagement.h"
using namespace std;
typedef struct
{
    char name[9];
    char extension[4];
    int8 startCluster;
} dirEntry;

//retorna o nome do arquivo
void getFileName(dirEntry Entry);

//faz o commando DIR em um cluster somente
void DIR(FILE *dir);

//faz o comando RM
bool remove(char* name);

void createDir(FILE *file, int8 curDir, char dirName[9], int8 clusterSize, int8 indexSize);
void createFile(FILE *file, int8 curDir,char fileName[9],char extensionName[4],int8 clusterSize, int8 indexSize);

#endif