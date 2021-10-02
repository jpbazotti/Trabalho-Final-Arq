#ifndef directory_h
#define directory_h
#include <definitions.h>
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
char* getFileName(dirEntry Entry);

//faz o commando DIR em um cluster somente
void DIR(FILE *dir);

void createDir(FILE *file, int8 curDir, char dirName[9], int8 clusterSize, int8 indexSize);
#endif
